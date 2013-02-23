/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
**
** Licensed under the Academic Free License version 2.1
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**--------------------------------------------------------------------------------------------------------
** File name:               module.c
** Last modified Date:      2012-7-18
** Last Version:            1.0.0
** Descriptions:            内核模块
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-18
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
** ELF 文件格式请查看:
**
** http://os.pku.edu.cn:8080/gaikuang/submission/TN05.ELF.Format.Summary.pdf
**
*********************************************************************************************************/
#include "kern/func_config.h"
#if CONFIG_MODULE_EN > 0
#include "kern/kern.h"
#include "kern/ipc.h"
#include "kern/atomic.h"
#include "kern/list.h"
#include "kern/hash_tbl.h"

#include "module/elf.h"
#include "module/symbol.h"
#include "module/module.h"

#include "vfs/vfs.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dlfcn.h>
/*********************************************************************************************************
** 模块
*********************************************************************************************************/
struct module {
    struct list_head        node;
    atomic_t                ref;                                        /*  引用计数                    */
    uint8_t                *elf;                                        /*  ELF 文件内容                */
    size_t                  size;                                       /*  文件大小                    */
    Elf32_Shdr            **shdrs;                                      /*  段首部数组指针              */
    uint16_t                bss_idx;                                    /*  BSS 段索引                  */
    uint16_t                text_idx;                                   /*  TEXT 段索引                 */
    char                    path[PATH_MAX];                             /*  模块文件路径                */
    int                     mode;                                       /*  加载模式　　                */
    bool_t                  is_ko;                                      /*  是否内核模块                */
    hash_tbl_t             *symbol_tbl;                                 /*　符号表                      */
};
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
static LIST_HEAD(module_list);                                          /*  模块链表                    */
static mutex_t              module_lock;                                /*  模块管理锁                  */

void *sys_symbol_lookup(const char *name, uint8_t type);
int sys_symbol_tbl_init(void);
/*********************************************************************************************************
** Function name:           module_init
** Descriptions:            初始化模块子系统
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_init(void)
{
    sys_symbol_tbl_init();                                              /*  初始化系统符号表            */

    INIT_LIST_HEAD(&module_list);                                       /*  初始化模块链表              */

    return mutex_create(&module_lock);                                  /*  创建模块管理锁              */
}
/*********************************************************************************************************
** Function name:           module_alloc
** Descriptions:            分配模块
** input parameters:        path                模块文件路径
**                          size                模块文件大小
**                          mode                加载模式
**                          is_ko               是否内核模块
** output parameters:       NONE
** Returned value:          模块 OR NULL
*********************************************************************************************************/
static module_t *module_alloc(const char *path, size_t size, int mode, bool_t is_ko)
{
    module_t *mod;

    mod = kmalloc(sizeof(module_t) + size, GFP_KERNEL);                 /*  分配模块及其缓冲            */
    if (mod != NULL) {
        mod->elf        = (uint8_t *)mod + sizeof(module_t);
        mod->size       = size;
        mod->bss_idx    = SHN_UNDEF;
        mod->text_idx   = SHN_UNDEF;
        mod->shdrs      = NULL;
        mod->mode       = mode;
        mod->is_ko      = is_ko;
        mod->symbol_tbl = NULL;

        atomic_set(&mod->ref, 0);

        vfs_path_format(mod->path, path);
    }
    return mod;
}
/*********************************************************************************************************
** Function name:           module_free
** Descriptions:            销毁模块
** input parameters:        mod                 模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_free(module_t *mod)
{
    if (mod->bss_idx != SHN_UNDEF) {                                    /*  释放掉 .bss 节区            */
        kfree((void *)mod->shdrs[mod->bss_idx]->sh_offset);
    }

    if (mod->shdrs != NULL) {                                           /*  释放节区首部数组            */
        kfree(mod->shdrs);
    }

    if (mod->symbol_tbl != NULL) {
        hash_tbl_destroy(mod->symbol_tbl);                              /*  销毁符号表                  */
    }

    kfree(mod);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_ref_by_addr
** Descriptions:            通过内核模块的一个地址引用内核模块
** input parameters:        addr                地址
** output parameters:       NONE
** Returned value:          内核模块 OR NULL
*********************************************************************************************************/
module_t *module_ref_by_addr(void *addr)
{
    module_t *mod;
    struct list_head *item;

    if (addr == NULL) {
        printk(KERN_ERR"%s: address == NULL\n", __func__);
        return NULL;
    }

    mutex_lock(&module_lock, 0);

    list_for_each(item, &module_list) {
        mod = list_entry(item, module_t, node);
        if (mod->is_ko) {
            if (((char *)addr >= (char *)mod + sizeof(module_t)) &&
                ((char *)addr <  (char *)mod + sizeof(module_t) + mod->size)) {
                atomic_inc(&mod->ref);
                mutex_unlock(&module_lock);
                return mod;
            }
        }
    }

    mutex_unlock(&module_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           module_unref
** Descriptions:            解取对内核模块的引用
** input parameters:        mod                 内核模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unref(module_t *mod)
{
    if (mod == NULL) {
        printk(KERN_ERR"%s: module == NULL\n", __func__);
        return -1;
    }

    atomic_dec(&mod->ref);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_lookup
** Descriptions:            查找全局模块
** input parameters:        path                全局模块文件路径
** output parameters:       NONE
** Returned value:          全局模块 OR NULL
*********************************************************************************************************/
static module_t *module_lookup(const char *path)
{
    module_t *mod;
    struct list_head *item;

    if (path == NULL) {
        printk(KERN_ERR"%s: path == NULL\n", __func__);
        return NULL;
    }

    mutex_lock(&module_lock, 0);

    list_for_each(item, &module_list) {
        mod = list_entry(item, module_t, node);
        if (mod->mode & RTLD_GLOBAL) {
            if (strcmp(mod->path, path) == 0) {
                mutex_unlock(&module_lock);
                return mod;
            }
        }
    }

    mutex_unlock(&module_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           module_install
** Descriptions:            安装模块
** input parameters:        mod                 模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_install(module_t *mod)
{
    mutex_lock(&module_lock, 0);

    if (mod->mode & RTLD_GLOBAL) {
        if (module_lookup(mod->path)) {
            mutex_unlock(&module_lock);
            return -1;
        }
    }

    list_add_tail(&mod->node, &module_list);

    mutex_unlock(&module_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_remove
** Descriptions:            从模块链表删除模块
** input parameters:        mod                 模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_remove(module_t *mod)
{
    mutex_lock(&module_lock, 0);

    list_del(&mod->node);

    mutex_unlock(&module_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_probe
** Descriptions:            探测模块
** input parameters:        mod                 模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_probe(module_t *mod)
{
    Elf32_Ehdr *ehdr;

    ehdr = (Elf32_Ehdr *)mod->elf;                                      /*  ELF 首部                    */

    if ((ehdr->e_ident[EI_MAG0] != ELFMAG0) ||                          /*  魔数检查                    */
        (ehdr->e_ident[EI_MAG1] != ELFMAG1) ||
        (ehdr->e_ident[EI_MAG2] != ELFMAG2) ||
        (ehdr->e_ident[EI_MAG3] != ELFMAG3)) {
        printk(KERN_ERR"%s: ehdr->e_ident[EI_MAG0] != ELFMAG0\n", __func__);
        return -1;
    }

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {                        /*  文件类型: 必须是 32 位目标  */
        printk(KERN_ERR"%s: ehdr->e_ident[EI_CLASS] != ELFCLASS32\n", __func__);
        return -1;
    }

    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {                        /*  数据编码方式: 必须是小端    */
        printk(KERN_ERR"%s: ehdr->e_ident[EI_DATA] != ELFDATA2LSB\n", __func__);
        return -1;
    }

    if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) {                      /*  ELF 首部版本号              */
        printk(KERN_ERR"%s: ehdr->e_ident[EI_VERSION] != EV_CURRENT\n", __func__);
        return -1;
    }

    /*
     * 目标文件类型: 必须是 可重定位文件 或 共享目标文件
     */
    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN) {
        printk(KERN_ERR"%s: ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN\n", __func__);
        return -1;
    }

#if 0
    if (ehdr->e_machine != EM_ARM) {                                    /*  体系结构类型: 必须是 ARM    */
        printk(KERN_ERR"%s: ehdr->e_machine != EM_ARM\n", __func__);
        return -1;
    }
#endif

    /*
     * 保证不存在程序首部表格相关的东西
     */
    if (ehdr->e_phoff != 0) {
        printk(KERN_ERR"%s: ehdr->e_phoff != 0\n", __func__);
        return -1;
    }

    if (ehdr->e_phentsize != 0) {
        printk(KERN_ERR"%s: ehdr->e_phentsize != 0\n", __func__);
        return -1;
    }

    if (ehdr->e_phnum != 0) {
        printk(KERN_ERR"%s: ehdr->e_phnum != 0\n", __func__);
        return -1;
    }

    /*
     * 保证存在节区首部表格相关的东西
     */
    if (ehdr->e_shoff == 0) {
        printk(KERN_ERR"%s: ehdr->e_shoff == 0\n", __func__);
        return -1;
    }

    if (ehdr->e_shentsize == 0) {
        printk(KERN_ERR"%s: ehdr->e_shentsize == 0\n", __func__);
        return -1;
    }

    if (ehdr->e_shnum == 0) {
        printk(KERN_ERR"%s: ehdr->e_shnum == 0\n", __func__);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           symbol_lookup
** Descriptions:            在系统符号表和全局模块里查找符号
** input parameters:        name                符号名
**                          type                符号类型
** output parameters:       NONE
** Returned value:          符号地址
*********************************************************************************************************/
static Elf32_Addr symbol_lookup(const char *name, uint8_t type)
{
    Elf32_Addr addr;
    module_t *mod;
    struct list_head *item;

    addr = (Elf32_Addr)sys_symbol_lookup(name, type);
    if (addr != 0) {
        return addr;
    }

    if (type != SYMBOL_TEXT) {
        return 0;
    }

    mutex_lock(&module_lock, 0);

    list_for_each(item, &module_list) {
        mod = list_entry(item, module_t, node);
        if (mod->mode & RTLD_GLOBAL) {
            addr = (Elf32_Addr)module_symbol(mod, name);
            if (addr != 0) {
                mutex_unlock(&module_lock);
                return addr;
            }
        }
    }

    mutex_unlock(&module_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_symbol_tbl_init
** Descriptions:            初始化模块的符号表
** input parameters:        mod                 模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_symbol_tbl_init(module_t *mod)
{
    int             i;
    Elf32_Ehdr     *ehdr;
    Elf32_Shdr     *shdr;
    void           *func;
    unsigned int    key;

    mod->symbol_tbl = hash_tbl_create(127);                             /*  创建符号表                  */
    if (mod->symbol_tbl == NULL) {
        printk(KERN_ERR"%s: failed to create module %s symbol hash table\n", __func__, mod->path);
        return -1;
    }

    ehdr = (Elf32_Ehdr *)mod->elf;                                      /*  ELF 首部                    */

    for (i = 0, shdr = mod->shdrs[0]; i < ehdr->e_shnum; i++, shdr = mod->shdrs[i]) {

        if (shdr->sh_type == SHT_SYMTAB) {                              /*  节区首部类型: 符号表        */
            Elf32_Shdr *symtab_shdr;
            Elf32_Shdr *strtab_shdr;
            char       *strtab;
            uint8_t    *symtab;
            Elf32_Sym  *sym;
            int         j;

            symtab_shdr = shdr;                                         /*  符号表节区首部              */
            symtab      = mod->elf + symtab_shdr->sh_offset;            /*  符号表 　                   */

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                continue;
            }
            strtab_shdr = mod->shdrs[symtab_shdr->sh_link];             /*  字符串表节区首部            */
            strtab      = (char *)mod->elf + strtab_shdr->sh_offset;    /*  字符串表                    */

            for (j = 0,
                 sym = (Elf32_Sym *)symtab;
                 j < symtab_shdr->sh_size / symtab_shdr->sh_entsize;
                 j++, sym = (Elf32_Sym *)((char *)sym + symtab_shdr->sh_entsize)) {

                if (sym->st_shndx == mod->text_idx &&                   /*  该符号在 TEXT 段            */
                    ELF32_ST_TYPE(sym->st_info) == STT_FUNC) {          /*  该符号是函数　　            */

                    func = (void *)(mod->elf +                          /*  该符号在 ELF 文件里的位置   */
                                    mod->shdrs[mod->text_idx]->sh_offset + sym->st_value);

                    key  = bkdr_hash(sym->st_name + strtab);

                    if (hash_tbl_insert(mod->symbol_tbl, key, func) < 0) {  /*  将符号加到符号表        */
                        printk(KERN_ERR"%s: failed to insert symbol %s to module %s symbol hash table\n",
                                __func__, sym->st_name + strtab, mod->path);
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           module_reloc
** Descriptions:            重定位模块
** input parameters:        mod                 模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_reloc(module_t *mod)
{
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    Elf32_Shdr **shdrs;
    Elf32_Shdr *shstrtab_shdr;
    char       *shstrtab;
    int         i;
    int         bss_idx;
    int         text_idx;

    ehdr = (Elf32_Ehdr *)mod->elf;                                      /*  ELF 首部                    */

    /*
     * 节区首部中的字符串的节区首部
     */
    shstrtab_shdr = (Elf32_Shdr *)(mod->elf + ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize);
    shstrtab      = (char *)mod->elf + shstrtab_shdr->sh_offset;        /*  节区首部的字符串表          */

    /*
     * 先装载所有节区首部到局部数组
     */
    shdrs = kmalloc(ehdr->e_shnum * sizeof(Elf32_Shdr *), GFP_KERNEL);
    if (shdrs == NULL) {
        goto error0;
    }

    bss_idx  = SHN_UNDEF;
    text_idx = SHN_UNDEF;

    for (i = 0; i < ehdr->e_shnum; i++) {
        shdrs[i] = shdr = (Elf32_Shdr *)(mod->elf + ehdr->e_shoff + ehdr->e_shentsize * i);
        /*
         * .bss 节区不存在 ELF 文件中, 需要分配出来
         */
        if (strcmp(shdr->sh_name + shstrtab, ".bss") == 0) {
            bss_idx = i;
            shdr->sh_offset = (Elf32_Off)kzalloc(shdr->sh_size, GFP_KERNEL);
            if (shdr->sh_offset == 0) {
                goto error1;
            }
            /*
             * 顺便把 .text 节区也找出来吧
             */
        } else if (strcmp(shdr->sh_name + shstrtab, ".text") == 0) {
            text_idx = i;
        }
    }

    /*
     * 处理所有需要重定位的节区
     */
    for (i = 0, shdr = shdrs[0]; i < ehdr->e_shnum; i++, shdr = shdrs[i]) {

        Elf32_Shdr *target_shdr;
        Elf32_Shdr *symtab_shdr;
        Elf32_Shdr *strtab_shdr;
        char       *strtab;
        uint8_t    *symtab;
        char       *target;

        if (shdr->sh_type == SHT_REL || shdr->sh_type == SHT_RELA) {    /*  该节区需要重定位            */

            if (shdr->sh_link == SHN_UNDEF) {
                printk(KERN_ERR"%s: shdr->sh_link == SHN_UNDEF\n", __func__);
                goto error1;
            }
            symtab_shdr = shdrs[shdr->sh_link];                         /*  符号表节区首部              */

            if (shdr->sh_info == SHN_UNDEF) {
                printk(KERN_ERR"%s: shdr->sh_info == SHN_UNDEF\n", __func__);
                goto error1;
            }
            target_shdr = shdrs[shdr->sh_info];                         /*  目标节区的节区首部          */

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                printk(KERN_ERR"%s: symtab_shdr->sh_link == SHN_UNDEF\n", __func__);
                goto error1;
            }
            strtab_shdr = shdrs[symtab_shdr->sh_link];                  /*  字符串表节区首部            */

            symtab      = mod->elf + symtab_shdr->sh_offset;            /*  符号表                      */
            target      = (char *)mod->elf + target_shdr->sh_offset;    /*  目标节区                    */
            strtab      = (char *)mod->elf + strtab_shdr->sh_offset;    /*  字符串表                    */
        }

        if (shdr->sh_type == SHT_REL) {                                 /*  重定位类型为 REL            */
            Elf32_Rel *rel;
            int        j;

            /*
             * 处理所有的 rel 条目
             */
            for (j = 0, rel = (Elf32_Rel *)(mod->elf + shdr->sh_offset);
                 j < shdr->sh_size / shdr->sh_entsize;
                 j++,   rel = (Elf32_Rel *)((char *)rel + shdr->sh_entsize)) {

                int sym_idx;

                sym_idx = ELF32_R_SYM(rel->r_info);                     /*  这个 rel 条目的符号索引     */

                if (STN_UNDEF == sym_idx) {
                    printk(KERN_ERR"%s: STN_UNDEF == sym_idx)\n", __func__);
                    goto error1;
                } else {
                    Elf32_Sym  *sym;
                    Elf32_Addr  addr;
                    int         ret;
                                                                        /*  这个 rel 条目的符号         */
                    sym = (Elf32_Sym *)(symtab + sym_idx * symtab_shdr->sh_entsize);

                    if (sym->st_shndx != SHN_UNDEF) {                   /*  如果这个符号在 ELF 文件里   */
                        /*
                         * 获得这个符号在 ELF 文件里地址
                         */
                        if (sym->st_shndx == bss_idx) {                 /*  如果这个符号在 .bss 节区    */
                            addr = (Elf32_Addr)(shdrs[bss_idx]->sh_offset + sym->st_value);
                        } else {
                            addr = (Elf32_Addr)(mod->elf + shdrs[sym->st_shndx]->sh_offset + sym->st_value);
                        }

                        /*
                         * 对该的 rel 条目进行重定位
                         */
                        ret = arch_relocate_rel(rel, addr, target);
                        if (ret < 0) {
                            printk(KERN_ERR"%s: failed to relocate rel item\n", __func__);
                            goto error1;
                        }
                    } else {
                        /*
                         * 查找这个符号
                         */
                        addr = (Elf32_Addr)symbol_lookup(sym->st_name + strtab,
                                ELF32_ST_TYPE(sym->st_info) == STT_FUNC ? SYMBOL_TEXT : SYMBOL_DATA);
                        if (addr != 0) {
                            /*
                             * 对该的 rel 条目进行重定位
                             */
                            ret = arch_relocate_rel(rel, addr, target);
                            if (ret < 0) {
                                printk(KERN_ERR"%s: failed to relocate rel item\n", __func__);
                                goto error1;
                            }
                        }
                        printk(KERN_ERR"%s: symbol %s no found\n", __func__, sym->st_name + strtab);
                        goto error1;
                    }
                }
            }
        } else if (shdr->sh_type == SHT_RELA) {
            printk(KERN_ERR"%s: failed to relocate rela item\n", __func__);
            goto error1;
        }
    }

    mod->shdrs    = shdrs;
    mod->bss_idx  = bss_idx;
    mod->text_idx = text_idx;

    return 0;

    error1:
    kfree(shdrs);

    error0:
    return -1;
}
/*********************************************************************************************************
** Function name:           module_symbol
** Descriptions:            在模块里查找符号
** input parameters:        mod                 模块
**                          name                符号名
** output parameters:       NONE
** Returned value:          符号地址 OR NULL
*********************************************************************************************************/
void *module_symbol(module_t *mod, const char *name)
{
    hash_node_t *node;

    if (mod == NULL || mod->symbol_tbl == NULL) {
        printk(KERN_ERR"%s: module == NULL\n", __func__);
        return NULL;
    }

    if (name == NULL) {
        printk(KERN_ERR"%s: symbol name == NULL\n", __func__);
        return NULL;
    }

    node = hash_tbl_lookup(mod->symbol_tbl, bkdr_hash(name));
    if (node != NULL) {
        return node->data;
    }
    return NULL;
}
/*********************************************************************************************************
** Function name:           __module_load
** Descriptions:            加载模块
** input parameters:        path                模块文件路径
**                          argc                参数个数
**                          argv                参数数组
**                          mode                加载模式
**                          is_ko               是否内核模块
** output parameters:       NONE
** Returned value:          模块 OR NULL
*********************************************************************************************************/
static module_t *__module_load(const char *path, int argc, char **argv, int mode, bool_t is_ko)
{
    int          fd;
    int          ret;
    int          len;
    uint8_t     *buf;
    struct stat  st;
    module_t    *mod;
    int (*func)(int argc, char **argv);

    if (path == NULL) {
        printk(KERN_ERR"%s: path == NULL\n", __func__);
        return NULL;
    }

    ret = vfs_access(path, R_OK);                                       /*  看下这个文件能否访问        */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to access %s\n", __func__, path);
        return NULL;
    }

    fd = vfs_open(path, O_RDONLY, 0666);                                /*  打开文件                    */
    if (fd < 0) {
        printk(KERN_ERR"%s: failed to open %s\n", __func__, path);
        return NULL;
    }

    ret = vfs_fstat(fd, &st);                                           /*  获得文件的大小              */
    if (ret < 0 || st.st_size == 0) {
        printk(KERN_ERR"%s: failed to stat %s or %s is empty\n", __func__, path, path);
        return NULL;
    }

    mod = module_alloc(path, st.st_size, mode, is_ko);                  /*  分配模块                    */
    if (mod == NULL) {
        printk(KERN_ERR"%s: failed to alloc module %s, %d byte\n", __func__, path, st.st_size);
        vfs_close(fd);
        return NULL;
    }

    len = 0;                                                            /*  将文件的内容读到缓冲区      */
    buf = mod->elf;
    while (st.st_size - len > 0) {
        ret = vfs_read(fd, buf, st.st_size - len);
        if (ret < 0) {
            printk(KERN_ERR"%s: failed to read module %s %d byte at %d\n", __func__, path, st.st_size - len, len);
            module_free(mod);
            vfs_close(fd);
            return NULL;
        }
        buf += ret;
        len += ret;
    }

    vfs_close(fd);                                                      /*  关闭文件                    */

    ret = module_probe(mod);                                            /*  探测模块                    */
    if (ret < 0) {
        printk(KERN_ERR"%s: %s is not a module file\n", __func__, path);
        module_free(mod);
        return NULL;
    }

    ret = module_reloc(mod);                                            /*  重定位模块                  */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to relocate module %s\n", __func__, path);
        module_free(mod);
        return NULL;
    }

    ret = module_symbol_tbl_init(mod);                                  /*  初始化模块的符号表          */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to init module %s symbol table\n", __func__, path);
        module_free(mod);
        return NULL;
    }

    mutex_lock(&module_lock, 0);

    ret = module_install(mod);                                          /*  安装模块                    */
    if (ret < 0) {
        printk(KERN_ERR"%s: module %s exist\n", __func__, path);
        module_free(mod);
        return NULL;
    }

    if (is_ko) {
        func = module_symbol(mod, "module_init");                       /*  执行内核模块初始化函数      */
        if (func != NULL) {
            ret = func(argc, argv);
            if (ret < 0) {
                printk(KERN_ERR"%s: failed to exec module %s's module_init\n", __func__, path);
                module_remove(mod);
                mutex_unlock(&module_lock);
                module_free(mod);
                return NULL;
            }
        }
    }

    mutex_unlock(&module_lock);

    return mod;
}
/*********************************************************************************************************
** Function name:           module_load
** Descriptions:            加载内核模块
** input parameters:        path                内核模块文件路径
**                          argc                参数个数
**                          argv                参数数组
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_load(const char *path, int argc, char **argv)
{
    if (__module_load(path, argc, argv, RTLD_GLOBAL, TRUE) != NULL) {
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           module_unload
** Descriptions:            卸载内核模块
** input parameters:        path                内核模块文件路径
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unload(const char *path)
{
    module_t *mod;
    int ret;
    int (*func)(void);

    mutex_lock(&module_lock, 0);

    mod = module_lookup(path);
    if (mod == NULL) {
        mutex_unlock(&module_lock);
        printk(KERN_ERR"%s: module %s no found\n", __func__, path);
        return -1;
    }

    if (!mod->is_ko) {
        mutex_unlock(&module_lock);
        printk(KERN_ERR"%s: module %s is not a kernel module\n", __func__, path);
        return -1;
    }

    func = module_symbol(mod, "module_exit");                           /*  执行模块清理函数 　         */
    if (func != NULL) {
        ret = func();
        if (ret < 0) {
            mutex_unlock(&module_lock);
            printk(KERN_ERR"%s: failed to exec module %s's module_exit\n", __func__, mod->path);
            return -1;
        }
    }

    if (atomic_read(&mod->ref) != 0) {
        mutex_unlock(&module_lock);
        printk(KERN_ERR"%s: module %s is busy\n", __func__, mod->path);
        return -1;
    }

    module_remove(mod);

    mutex_unlock(&module_lock);

    module_free(mod);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_open
** Descriptions:            打开应用模块
** input parameters:        path                应用模块文件路径
**                          mode                加载模式
** output parameters:       NONE
** Returned value:          模块 OR NULL
*********************************************************************************************************/
module_t *module_open(const char *path, int mode)
{
    return __module_load(path, 0, NULL, mode, FALSE);
}
/*********************************************************************************************************
** Function name:           module_close
** Descriptions:            关闭应用模块
** input parameters:        mod                 应用模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_close(module_t *mod)
{
    if (mod == NULL) {
        printk(KERN_ERR"%s: module = NULL\n", __func__);
        return -1;
    }

    module_remove(mod);

    module_free(mod);

    return 0;
}
/*********************************************************************************************************
** Function name:           symbol_name
** Descriptions:            查找与指定地址最前近的符号
** input parameters:        addr                指定地址
** output parameters:       pdiff               差值
** Returned value:          符号名
*********************************************************************************************************/
const char *symbol_name(mem_ptr_t addr, mem_ptr_t *pdiff)
{
    symbol_t       *symbol;
    symbol_t       *symbol_found;
    mem_ptr_t       diff;
    int             i;
    Elf32_Ehdr     *ehdr;
    Elf32_Shdr     *shdr;
    void           *func;
    module_t       *mod;
    struct list_head *item;

    diff = (mem_ptr_t)-1;

    symbol_found = NULL;

    extern symbol_t sys_symbol_tbl[];
    for (symbol = sys_symbol_tbl; symbol->name != NULL; symbol++) {

        if (symbol->flags == SYMBOL_TEXT) {
            if ((mem_ptr_t)symbol->addr == addr) {
                *pdiff = 0;
                return symbol->name;
            }

            if ((mem_ptr_t)symbol->addr < addr) {
                if (addr - (mem_ptr_t)symbol->addr < diff) {
                    diff = addr - (mem_ptr_t)symbol->addr;
                    symbol_found = symbol;
                }
            }
        }
    }

    mutex_lock(&module_lock, 0);

    list_for_each(item, &module_list) {
        mod = list_entry(item, module_t, node);

        ehdr = (Elf32_Ehdr *)mod->elf;                                  /*  ELF 首部                    */

        for (i = 0, shdr = mod->shdrs[0]; i < ehdr->e_shnum; i++, shdr = mod->shdrs[i]) {

            if (shdr->sh_type == SHT_SYMTAB) {                          /*  节区首部类型: 符号表        */
                Elf32_Shdr *symtab_shdr;
                Elf32_Shdr *strtab_shdr;
                char       *strtab;
                uint8_t    *symtab;
                Elf32_Sym  *sym;
                int         j;

                symtab_shdr = shdr;                                     /*  符号表节区首部              */
                symtab      = mod->elf + symtab_shdr->sh_offset;        /*  符号表 　                   */

                if (symtab_shdr->sh_link == SHN_UNDEF) {
                    continue;
                }
                strtab_shdr = mod->shdrs[symtab_shdr->sh_link];         /*  字符串表节区首部            */
                strtab      = (char *)mod->elf + strtab_shdr->sh_offset;/*  字符串表                    */

                for (j = 0,
                     sym = (Elf32_Sym *)symtab;
                     j < symtab_shdr->sh_size / symtab_shdr->sh_entsize;
                     j++, sym = (Elf32_Sym *)((char *)sym + symtab_shdr->sh_entsize)) {

                    if (sym->st_shndx == mod->text_idx &&               /*  该符号在 TEXT 段            */
                        ELF32_ST_TYPE(sym->st_info) == STT_FUNC) {      /*  该符号是函数　　            */

                        func = (void *)(mod->elf +                      /*  该符号在 ELF 文件里的位置   */
                                        mod->shdrs[mod->text_idx]->sh_offset + sym->st_value);

                        if ((mem_ptr_t)func == addr) {
                            *pdiff = 0;
                            mutex_unlock(&module_lock);
                            return sym->st_name + strtab;
                        }

                        if ((mem_ptr_t)func < addr) {
                            if (addr - (mem_ptr_t)func < diff) {
                                static symbol_t __symbol;
                                diff = addr - (mem_ptr_t)func;
                                __symbol.name = sym->st_name + strtab;
                                symbol_found = &__symbol;
                            }
                        }
                    }
                }
            }
        }
    }

    mutex_unlock(&module_lock);

    if (symbol_found != NULL) {
        *pdiff = diff;
        return symbol_found->name;
    } else {
        *pdiff = 0;
        return "no found";
    }
}
#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
