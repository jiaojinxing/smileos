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

#include "module/elf.h"
#include "module/symbol_tool.h"
#include "module/module.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
/*********************************************************************************************************
** 模块
*********************************************************************************************************/
struct module {
    struct list_head    mod_list;
    uint32_t            key;                                            /*  ELF 文件路径 key            */
    atomic_t            ref;                                            /*  引用计数                    */
    uint8_t            *elf;                                            /*  ELF 文件                    */
    size_t              size;                                           /*  文件大小                    */
    Elf32_Shdr        **shdrs;                                          /*  段首部数组指针              */
    uint16_t            bss_idx;                                        /*  BSS 段索引                  */
    uint16_t            text_idx;                                       /*  TEXT 段索引                 */
};
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
static LIST_HEAD(module_list);                                          /*  模块链表                    */
static mutex_t              mod_mgr_lock;                               /*  模块管理锁                  */
/*********************************************************************************************************
** Function name:           symbol_lookup
** Descriptions:            查找符号
** input parameters:        name                符号名
**                          type                符号类型
** output parameters:       NONE
** Returned value:          符号地址 OR NULL
*********************************************************************************************************/
void *symbol_lookup(const char *name, uint8_t type);
/*********************************************************************************************************
** Function name:           symbol_init
** Descriptions:            初始化符号表
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int symbol_init(void);
/*********************************************************************************************************
** Function name:           module_init
** Descriptions:            初始化内核模块子系统
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_init(void)
{
    symbol_init();                                                      /*  初始化符号表                */

    INIT_LIST_HEAD(&module_list);                                       /*  初始化模块链表              */

    return mutex_create(&mod_mgr_lock);                                 /*  创建模块管理锁              */
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
    mutex_lock(&mod_mgr_lock, 0);

    list_add_tail(&mod->mod_list, &module_list);

    mutex_unlock(&mod_mgr_lock);

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
    module_t *_mod;
    struct list_head *item, *save;
    int ret = -1;

    mutex_lock(&mod_mgr_lock, 0);

    list_for_each_safe(item, save, &module_list) {
        _mod = list_entry(item, module_t, mod_list);
        if (_mod == mod) {
            list_del(&mod->mod_list);
            ret = 0;
            break;
        }
    }

    mutex_unlock(&mod_mgr_lock);

    return ret;
}
/*********************************************************************************************************
** Function name:           module_lookup_by_key
** Descriptions:            查找模块
** input parameters:        key                 ELF 文件路径 KEY
** output parameters:       NONE
** Returned value:          模块 OR NULL
*********************************************************************************************************/
static module_t *module_lookup_by_key(uint32_t key)
{
    module_t *mod;
    struct list_head *item;

    mutex_lock(&mod_mgr_lock, 0);

    list_for_each(item, &module_list) {
        mod = list_entry(item, module_t, mod_list);
        if (key == mod->key) {
            mutex_unlock(&mod_mgr_lock);
            return mod;
        }
    }

    mutex_unlock(&mod_mgr_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           module_lookup
** Descriptions:            查找模块
** input parameters:        path                ELF 文件路径
** output parameters:       NONE
** Returned value:          模块 OR NULL
*********************************************************************************************************/
module_t *module_lookup(const char *path)
{
    uint32_t key;

    if (path == NULL) {
        return NULL;
    }

    key = bkdr_hash(path);

    return module_lookup_by_key(key);
}
/*********************************************************************************************************
** Function name:           module_ref_by_addr
** Descriptions:            通过模块的一个地址引用模块
** input parameters:        addr                地址
** output parameters:       NONE
** Returned value:          模块 OR NULL
*********************************************************************************************************/
module_t *module_ref_by_addr(void *addr)
{
    module_t *mod;
    struct list_head *item;

    mutex_lock(&mod_mgr_lock, 0);

    list_for_each(item, &module_list) {
        mod = list_entry(item, module_t, mod_list);
        if (((char *)addr >= (char *)mod + sizeof(module_t)) &&
            ((char *)addr <  (char *)mod + sizeof(module_t) + mod->size)) {
            atomic_inc(&mod->ref);
            mutex_unlock(&mod_mgr_lock);
            return mod;
        }
    }

    mutex_unlock(&mod_mgr_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           module_unref
** Descriptions:            解取对模块的引用
** input parameters:        mod                 模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unref(module_t *mod)
{
    if (mod != NULL) {
        atomic_dec(&mod->ref);
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           module_alloc
** Descriptions:            分配模块
** input parameters:        path                文件路径
**                          size                文件大小
** output parameters:       NONE
** Returned value:          模块 OR NULL
*********************************************************************************************************/
static module_t *module_alloc(const char *path, size_t size)
{
    module_t *mod;

    mod = kmalloc(sizeof(module_t) + size, GFP_KERNEL);                 /*  分配模块及其缓冲            */
    if (mod != NULL) {
        /*
         * 初始化模块
         */
        mod->key        = bkdr_hash(path);
        mod->elf        = (uint8_t *)mod + sizeof(module_t);
        mod->size       = size;
        mod->bss_idx    = SHN_UNDEF;
        mod->text_idx   = SHN_UNDEF;
        mod->shdrs      = NULL;
        atomic_set(&mod->ref, 0);
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

    kfree(mod);

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
                goto error1;
            }
            symtab_shdr = shdrs[shdr->sh_link];                         /*  符号表节区首部              */

            if (shdr->sh_info == SHN_UNDEF) {
                goto error1;
            }
            target_shdr = shdrs[shdr->sh_info];                         /*  目标节区的节区首部          */

            if (symtab_shdr->sh_link == SHN_UNDEF) {
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

                if (STN_UNDEF != sym_idx) {
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
                            goto error1;
                        }
                    } else {
                        /*
                         * 在系统里查找这个符号
                         */
                        addr = (Elf32_Addr)symbol_lookup(sym->st_name + strtab,
                                ELF32_ST_TYPE(sym->st_info) == STT_FUNC ? SYMBOL_TEXT : SYMBOL_DATA);
                        if (addr == 0) {
                            printk(KERN_ERR"%s: symbol %s no found\n", __func__, sym->st_name + strtab);
                            goto error1;
                        } else {
                            /*
                             * 对该的 rel 条目进行重定位
                             */
                            ret = arch_relocate_rel(rel, addr, target);
                            if (ret < 0) {
                                goto error1;
                            }
                        }
                    }
                } else {
                    goto error1;
                }
            }
        } else if (shdr->sh_type == SHT_RELA) {
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
** Function name:           module_exec
** Descriptions:            执行模块
** input parameters:        mod                 模块
**                          func_name           函数名
**                          argc                参数个数
**                          argv                参数数组
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_exec(module_t *mod, const char *func_name, int argc, char **argv)
{
    int         i;
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    int (*func)(int argc, char **argv);

    ehdr = (Elf32_Ehdr *)mod->elf;                                      /*  ELF 首部                    */

    /*
     * 在模块的符号表里查找这个函数
     */
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

                    if (strcmp(sym->st_name + strtab, func_name) == 0) {/*  该符号名字匹配　            */

                        func = (int (*)(int, char **))(mod->elf +       /*  该符号在 ELF 文件里的位置   */
                                mod->shdrs[mod->text_idx]->sh_offset + sym->st_value);

                        return func(argc, argv);                        /*  执行该函数                  */
                    }
                }
            }
        }
    }
    return -1;
}
/*********************************************************************************************************
** Function name:           module_load
** Descriptions:            加载 ELF 文件
** input parameters:        path                ELF 文件路径
**                          argc                参数个数
**                          argv                参数数组
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_load(const char *path, int argc, char **argv)
{
    int          fd;
    int          ret;
    int          len;
    uint8_t     *buf;
    struct stat  st;
    module_t    *mod;

    if (path == NULL) {
        printk(KERN_ERR"%s: path = NULL\n", __func__);
        return -1;
    }

    ret = access(path, R_OK);                                           /*  看下这个文件能否访问        */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to access %s\n", __func__, path);
        return -1;
    }

    fd = open(path, O_RDONLY);                                          /*  打开文件                    */
    if (fd < 0) {
        printk(KERN_ERR"%s: failed to open %s\n", __func__, path);
        return -1;
    }

    ret = fstat(fd, &st);                                               /*  获得文件的大小              */
    if (ret < 0 || st.st_size == 0) {
        printk(KERN_ERR"%s: failed to stat %s or %s is empty\n", __func__, path, path);
        return -1;
    }

    mod = module_alloc(path, st.st_size);                               /*  分配模块                    */
    if (mod == NULL) {
        printk(KERN_ERR"%s: failed to alloc module %s, %d byte\n", __func__, path, st.st_size);
        close(fd);
        return -1;
    }

    len = 0;                                                            /*  将文件的内容读到缓冲区      */
    buf = mod->elf;
    while (st.st_size - len > 0) {
        ret = read(fd, buf, st.st_size - len);
        if (ret < 0) {
            printk(KERN_ERR"%s: failed to read module %s %d byte at %d\n", __func__, path, st.st_size - len, len);
            module_free(mod);
            close(fd);
            return -1;
        }
        buf += ret;
        len += ret;
    }

    close(fd);                                                          /*  关闭文件                    */

    ret = module_probe(mod);                                            /*  探测模块                    */
    if (ret < 0) {
        printk(KERN_ERR"%s: %s is not a module file\n", __func__, path);
        module_free(mod);
        return -1;
    }

    ret = module_reloc(mod);                                            /*  重定位模块                  */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to reloc module %s\n", __func__, path);
        module_free(mod);
        return -1;
    }

    mutex_lock(&mod_mgr_lock, 0);

    if (module_lookup_by_key(mod->key) != NULL) {                       /*  看下模块是否已经存在      　*/
        printk(KERN_ERR"%s: module %s exist\n", __func__, path);
        mutex_unlock(&mod_mgr_lock);
        module_free(mod);
        return -1;
    }

    module_install(mod);                                                /*  安装模块                    */

    mutex_unlock(&mod_mgr_lock);

    ret = module_exec(mod, "module_init", argc, argv);                  /*  执行模块初始化函数          */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to exec module %s's module_init\n", __func__, path);
        module_remove(mod);
        module_free(mod);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           module_unload
** Descriptions:            卸载模块
** input parameters:        path                ELF 文件路径
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unload(const char *path)
{
    module_t *mod;
    int ret;

    if (path == NULL) {
        printk(KERN_ERR"%s: path = NULL\n", __func__);
        return -1;
    }

    mod = module_lookup(path);
    if (mod == NULL) {
        printk(KERN_ERR"%s: module %s no found\n", __func__, path);
        return -1;
    }

    ret = module_exec(mod, "module_exit", 0, NULL);
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to exec module %s's module_exit\n", __func__, path);
        return -1;
    }

    if (atomic_read(&mod->ref) != 0) {
        printk(KERN_ERR"%s: module %s is busy\n", __func__, path);
        return -1;
    }

    module_remove(mod);

    module_free(mod);

    return 0;
}
#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
