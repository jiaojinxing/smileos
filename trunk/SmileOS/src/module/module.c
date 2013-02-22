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
** Descriptions:            �ں�ģ��
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-18
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
** ELF �ļ���ʽ��鿴:
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
** ģ��
*********************************************************************************************************/
struct module {
    struct list_head    node;
    atomic_t            ref;                                            /*  ���ü���                    */
    uint8_t            *elf;                                            /*  ELF �ļ�                    */
    size_t              size;                                           /*  �ļ���С                    */
    Elf32_Shdr        **shdrs;                                          /*  ���ײ�����ָ��              */
    uint16_t            bss_idx;                                        /*  BSS ������                  */
    uint16_t            text_idx;                                       /*  TEXT ������                 */
    char                path[PATH_MAX];                                 /*  ģ���ļ�·��                */
    int                 mode;                                           /*  ����ģʽ����                */
    bool_t              is_ko;                                          /*  �Ƿ��ں�ģ��                */
    hash_tbl_t         *symbol_tbl;
};
/*********************************************************************************************************
** ȫ�ֱ���
*********************************************************************************************************/
static LIST_HEAD(module_list);                                          /*  ģ������                    */
static mutex_t              module_lock;                                /*  ģ�������                  */

void *sys_symbol_lookup(const char *name, uint8_t type);
int sys_symbol_tbl_init(void);
/*********************************************************************************************************
** Function name:           module_init
** Descriptions:            ��ʼ��ģ����ϵͳ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_init(void)
{
    sys_symbol_tbl_init();                                              /*  ��ʼ��ϵͳ���ű�            */

    INIT_LIST_HEAD(&module_list);                                       /*  ��ʼ��ģ������              */

    return mutex_create(&module_lock);                                  /*  ����ģ�������              */
}
/*********************************************************************************************************
** Function name:           module_alloc
** Descriptions:            ����ģ��
** input parameters:        path                ģ���ļ�·��
**                          size                ģ���ļ���С
**                          mode                ����ģʽ
**                          is_ko               �Ƿ��ں�ģ��
** output parameters:       NONE
** Returned value:          ģ�� OR NULL
*********************************************************************************************************/
static module_t *module_alloc(const char *path, size_t size, int mode, bool_t is_ko)
{
    module_t *mod;

    mod = kmalloc(sizeof(module_t) + size, GFP_KERNEL);                 /*  ����ģ�鼰�仺��            */
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
** Descriptions:            ����ģ��
** input parameters:        mod                 ģ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_free(module_t *mod)
{
    if (mod->bss_idx != SHN_UNDEF) {                                    /*  �ͷŵ� .bss ����            */
        kfree((void *)mod->shdrs[mod->bss_idx]->sh_offset);
    }

    if (mod->shdrs != NULL) {                                           /*  �ͷŽ����ײ�����            */
        kfree(mod->shdrs);
    }

    if (mod->symbol_tbl != NULL) {
        hash_tbl_destroy(mod->symbol_tbl);                              /*  ���ٷ��ű�                  */
    }

    kfree(mod);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_ref_by_addr
** Descriptions:            ͨ���ں�ģ���һ����ַ�����ں�ģ��
** input parameters:        addr                ��ַ
** output parameters:       NONE
** Returned value:          �ں�ģ�� OR NULL
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
** Descriptions:            ��ȡ���ں�ģ�������
** input parameters:        mod                 �ں�ģ��
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
** Descriptions:            ����ȫ��ģ��
** input parameters:        path                ȫ��ģ���ļ�·��
** output parameters:       NONE
** Returned value:          ȫ��ģ�� OR NULL
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
** Descriptions:            ��װģ��
** input parameters:        mod                 ģ��
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
** Descriptions:            ��ģ������ɾ��ģ��
** input parameters:        mod                 ģ��
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
** Descriptions:            ̽��ģ��
** input parameters:        mod                 ģ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_probe(module_t *mod)
{
    Elf32_Ehdr *ehdr;

    ehdr = (Elf32_Ehdr *)mod->elf;                                      /*  ELF �ײ�                    */

    if ((ehdr->e_ident[EI_MAG0] != ELFMAG0) ||                          /*  ħ�����                    */
        (ehdr->e_ident[EI_MAG1] != ELFMAG1) ||
        (ehdr->e_ident[EI_MAG2] != ELFMAG2) ||
        (ehdr->e_ident[EI_MAG3] != ELFMAG3)) {
        printk(KERN_ERR"%s: ehdr->e_ident[EI_MAG0] != ELFMAG0\n", __func__);
        return -1;
    }

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {                        /*  �ļ�����: ������ 32 λĿ��  */
        printk(KERN_ERR"%s: ehdr->e_ident[EI_CLASS] != ELFCLASS32\n", __func__);
        return -1;
    }

    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {                        /*  ���ݱ��뷽ʽ: ������С��    */
        printk(KERN_ERR"%s: ehdr->e_ident[EI_DATA] != ELFDATA2LSB\n", __func__);
        return -1;
    }

    if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) {                      /*  ELF �ײ��汾��              */
        printk(KERN_ERR"%s: ehdr->e_ident[EI_VERSION] != EV_CURRENT\n", __func__);
        return -1;
    }

    /*
     * Ŀ���ļ�����: ������ ���ض�λ�ļ� �� ����Ŀ���ļ�
     */
    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN) {
        printk(KERN_ERR"%s: ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN\n", __func__);
        return -1;
    }

#if 0
    if (ehdr->e_machine != EM_ARM) {                                    /*  ��ϵ�ṹ����: ������ ARM    */
        printk(KERN_ERR"%s: ehdr->e_machine != EM_ARM\n", __func__);
        return -1;
    }
#endif

    /*
     * ��֤�����ڳ����ײ������صĶ���
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
     * ��֤���ڽ����ײ������صĶ���
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
** Descriptions:            ��ϵͳ���ű��ȫ��ģ������ҷ���
** input parameters:        name                ������
**                          type                ��������
** output parameters:       NONE
** Returned value:          ���ŵ�ַ
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
** Descriptions:            ��ʼ��ģ��ķ��ű�
** input parameters:        mod                 ģ��
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

    mod->symbol_tbl = hash_tbl_create(127);                             /*  �������ű�                  */
    if (mod->symbol_tbl == NULL) {
        printk(KERN_ERR"%s: failed to create module %s symbol hash table\n", __func__, mod->path);
        return -1;
    }

    ehdr = (Elf32_Ehdr *)mod->elf;                                      /*  ELF �ײ�                    */

    for (i = 0, shdr = mod->shdrs[0]; i < ehdr->e_shnum; i++, shdr = mod->shdrs[i]) {

        if (shdr->sh_type == SHT_SYMTAB) {                              /*  �����ײ�����: ���ű�        */
            Elf32_Shdr *symtab_shdr;
            Elf32_Shdr *strtab_shdr;
            char       *strtab;
            uint8_t    *symtab;
            Elf32_Sym  *sym;
            int         j;

            symtab_shdr = shdr;                                         /*  ���ű�����ײ�              */
            symtab      = mod->elf + symtab_shdr->sh_offset;            /*  ���ű� ��                   */

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                continue;
            }
            strtab_shdr = mod->shdrs[symtab_shdr->sh_link];             /*  �ַ���������ײ�            */
            strtab      = (char *)mod->elf + strtab_shdr->sh_offset;    /*  �ַ�����                    */

            for (j = 0,
                 sym = (Elf32_Sym *)symtab;
                 j < symtab_shdr->sh_size / symtab_shdr->sh_entsize;
                 j++, sym = (Elf32_Sym *)((char *)sym + symtab_shdr->sh_entsize)) {

                if (sym->st_shndx == mod->text_idx &&                   /*  �÷����� TEXT ��            */
                    ELF32_ST_TYPE(sym->st_info) == STT_FUNC) {          /*  �÷����Ǻ�������            */

                    func = (void *)(mod->elf +                          /*  �÷����� ELF �ļ����λ��   */
                                    mod->shdrs[mod->text_idx]->sh_offset + sym->st_value);

                    key  = bkdr_hash(sym->st_name + strtab);

                    if (hash_tbl_insert(mod->symbol_tbl, key, func) < 0) {  /*  �����żӵ����ű�        */
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
** Descriptions:            �ض�λģ��
** input parameters:        mod                 ģ��
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

    ehdr = (Elf32_Ehdr *)mod->elf;                                      /*  ELF �ײ�                    */

    /*
     * �����ײ��е��ַ����Ľ����ײ�
     */
    shstrtab_shdr = (Elf32_Shdr *)(mod->elf + ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize);
    shstrtab      = (char *)mod->elf + shstrtab_shdr->sh_offset;        /*  �����ײ����ַ�����          */

    /*
     * ��װ�����н����ײ����ֲ�����
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
         * .bss ���������� ELF �ļ���, ��Ҫ�������
         */
        if (strcmp(shdr->sh_name + shstrtab, ".bss") == 0) {
            bss_idx = i;
            shdr->sh_offset = (Elf32_Off)kzalloc(shdr->sh_size, GFP_KERNEL);
            if (shdr->sh_offset == 0) {
                goto error1;
            }
            /*
             * ˳��� .text ����Ҳ�ҳ�����
             */
        } else if (strcmp(shdr->sh_name + shstrtab, ".text") == 0) {
            text_idx = i;
        }
    }

    /*
     * ����������Ҫ�ض�λ�Ľ���
     */
    for (i = 0, shdr = shdrs[0]; i < ehdr->e_shnum; i++, shdr = shdrs[i]) {

        Elf32_Shdr *target_shdr;
        Elf32_Shdr *symtab_shdr;
        Elf32_Shdr *strtab_shdr;
        char       *strtab;
        uint8_t    *symtab;
        char       *target;

        if (shdr->sh_type == SHT_REL || shdr->sh_type == SHT_RELA) {    /*  �ý�����Ҫ�ض�λ            */

            if (shdr->sh_link == SHN_UNDEF) {
                printk(KERN_ERR"%s: shdr->sh_link == SHN_UNDEF\n", __func__);
                goto error1;
            }
            symtab_shdr = shdrs[shdr->sh_link];                         /*  ���ű�����ײ�              */

            if (shdr->sh_info == SHN_UNDEF) {
                printk(KERN_ERR"%s: shdr->sh_info == SHN_UNDEF\n", __func__);
                goto error1;
            }
            target_shdr = shdrs[shdr->sh_info];                         /*  Ŀ������Ľ����ײ�          */

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                printk(KERN_ERR"%s: symtab_shdr->sh_link == SHN_UNDEF\n", __func__);
                goto error1;
            }
            strtab_shdr = shdrs[symtab_shdr->sh_link];                  /*  �ַ���������ײ�            */

            symtab      = mod->elf + symtab_shdr->sh_offset;            /*  ���ű�                      */
            target      = (char *)mod->elf + target_shdr->sh_offset;    /*  Ŀ�����                    */
            strtab      = (char *)mod->elf + strtab_shdr->sh_offset;    /*  �ַ�����                    */
        }

        if (shdr->sh_type == SHT_REL) {                                 /*  �ض�λ����Ϊ REL            */
            Elf32_Rel *rel;
            int        j;

            /*
             * �������е� rel ��Ŀ
             */
            for (j = 0, rel = (Elf32_Rel *)(mod->elf + shdr->sh_offset);
                 j < shdr->sh_size / shdr->sh_entsize;
                 j++,   rel = (Elf32_Rel *)((char *)rel + shdr->sh_entsize)) {

                int sym_idx;

                sym_idx = ELF32_R_SYM(rel->r_info);                     /*  ��� rel ��Ŀ�ķ�������     */

                if (STN_UNDEF == sym_idx) {
                    printk(KERN_ERR"%s: STN_UNDEF == sym_idx)\n", __func__);
                    goto error1;
                } else {
                    Elf32_Sym  *sym;
                    Elf32_Addr  addr;
                    int         ret;
                                                                        /*  ��� rel ��Ŀ�ķ���         */
                    sym = (Elf32_Sym *)(symtab + sym_idx * symtab_shdr->sh_entsize);

                    if (sym->st_shndx != SHN_UNDEF) {                   /*  ������������ ELF �ļ���   */
                        /*
                         * ������������ ELF �ļ����ַ
                         */
                        if (sym->st_shndx == bss_idx) {                 /*  ������������ .bss ����    */
                            addr = (Elf32_Addr)(shdrs[bss_idx]->sh_offset + sym->st_value);
                        } else {
                            addr = (Elf32_Addr)(mod->elf + shdrs[sym->st_shndx]->sh_offset + sym->st_value);
                        }

                        /*
                         * �Ըõ� rel ��Ŀ�����ض�λ
                         */
                        ret = arch_relocate_rel(rel, addr, target);
                        if (ret < 0) {
                            printk(KERN_ERR"%s: failed to relocate rel item\n", __func__);
                            goto error1;
                        }
                    } else {
                        /*
                         * �����������
                         */
                        addr = (Elf32_Addr)symbol_lookup(sym->st_name + strtab,
                                ELF32_ST_TYPE(sym->st_info) == STT_FUNC ? SYMBOL_TEXT : SYMBOL_DATA);
                        if (addr != 0) {
                            /*
                             * �Ըõ� rel ��Ŀ�����ض�λ
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
** Descriptions:            ��ģ������ҷ���
** input parameters:        mod                 ģ��
**                          name                ������
** output parameters:       NONE
** Returned value:          ���ŵ�ַ OR NULL
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
** Descriptions:            ����ģ��
** input parameters:        path                ģ���ļ�·��
**                          argc                ��������
**                          argv                ��������
**                          mode                ����ģʽ
**                          is_ko               �Ƿ��ں�ģ��
** output parameters:       NONE
** Returned value:          ģ�� OR NULL
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

    ret = vfs_access(path, R_OK);                                       /*  ��������ļ��ܷ����        */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to access %s\n", __func__, path);
        return NULL;
    }

    fd = vfs_open(path, O_RDONLY, 0666);                                /*  ���ļ�                    */
    if (fd < 0) {
        printk(KERN_ERR"%s: failed to open %s\n", __func__, path);
        return NULL;
    }

    ret = vfs_fstat(fd, &st);                                           /*  ����ļ��Ĵ�С              */
    if (ret < 0 || st.st_size == 0) {
        printk(KERN_ERR"%s: failed to stat %s or %s is empty\n", __func__, path, path);
        return NULL;
    }

    mod = module_alloc(path, st.st_size, mode, is_ko);                  /*  ����ģ��                    */
    if (mod == NULL) {
        printk(KERN_ERR"%s: failed to alloc module %s, %d byte\n", __func__, path, st.st_size);
        vfs_close(fd);
        return NULL;
    }

    len = 0;                                                            /*  ���ļ������ݶ���������      */
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

    vfs_close(fd);                                                      /*  �ر��ļ�                    */

    ret = module_probe(mod);                                            /*  ̽��ģ��                    */
    if (ret < 0) {
        printk(KERN_ERR"%s: %s is not a module file\n", __func__, path);
        module_free(mod);
        return NULL;
    }

    ret = module_reloc(mod);                                            /*  �ض�λģ��                  */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to relocate module %s\n", __func__, path);
        module_free(mod);
        return NULL;
    }

    ret = module_symbol_tbl_init(mod);                                  /*  ��ʼ��ģ��ķ��ű�          */
    if (ret < 0) {
        printk(KERN_ERR"%s: failed to init module %s symbol table\n", __func__, path);
        module_free(mod);
        return NULL;
    }

    mutex_lock(&module_lock, 0);

    ret = module_install(mod);                                          /*  ��װģ��                    */
    if (ret < 0) {
        printk(KERN_ERR"%s: module %s exist\n", __func__, path);
        module_free(mod);
        return NULL;
    }

    if (is_ko) {
        func = module_symbol(mod, "module_init");                       /*  ִ���ں�ģ���ʼ������      */
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
** Descriptions:            �����ں�ģ��
** input parameters:        path                �ں�ģ���ļ�·��
**                          argc                ��������
**                          argv                ��������
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
** Descriptions:            ж���ں�ģ��
** input parameters:        path                �ں�ģ���ļ�·��
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

    func = module_symbol(mod, "module_exit");                           /*  ִ��ģ�������� ��         */
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
** Descriptions:            ��Ӧ��ģ��
** input parameters:        path                Ӧ��ģ���ļ�·��
**                          mode                ����ģʽ
** output parameters:       NONE
** Returned value:          ģ�� OR NULL
*********************************************************************************************************/
module_t *module_open(const char *path, int mode)
{
    return __module_load(path, 0, NULL, mode, FALSE);
}
/*********************************************************************************************************
** Function name:           module_close
** Descriptions:            �ر�Ӧ��ģ��
** input parameters:        mod                 Ӧ��ģ��
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
** Descriptions:            ������ָ����ַ��ӽ��ķ���
** input parameters:        addr                ָ����ַ
** output parameters:       pdiff               ��ֵ
** Returned value:          ������
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

        ehdr = (Elf32_Ehdr *)mod->elf;                                  /*  ELF �ײ�                    */

        for (i = 0, shdr = mod->shdrs[0]; i < ehdr->e_shnum; i++, shdr = mod->shdrs[i]) {

            if (shdr->sh_type == SHT_SYMTAB) {                          /*  �����ײ�����: ���ű�        */
                Elf32_Shdr *symtab_shdr;
                Elf32_Shdr *strtab_shdr;
                char       *strtab;
                uint8_t    *symtab;
                Elf32_Sym  *sym;
                int         j;

                symtab_shdr = shdr;                                     /*  ���ű�����ײ�              */
                symtab      = mod->elf + symtab_shdr->sh_offset;        /*  ���ű� ��                   */

                if (symtab_shdr->sh_link == SHN_UNDEF) {
                    continue;
                }
                strtab_shdr = mod->shdrs[symtab_shdr->sh_link];         /*  �ַ���������ײ�            */
                strtab      = (char *)mod->elf + strtab_shdr->sh_offset;/*  �ַ�����                    */

                for (j = 0,
                     sym = (Elf32_Sym *)symtab;
                     j < symtab_shdr->sh_size / symtab_shdr->sh_entsize;
                     j++, sym = (Elf32_Sym *)((char *)sym + symtab_shdr->sh_entsize)) {

                    if (sym->st_shndx == mod->text_idx &&               /*  �÷����� TEXT ��            */
                        ELF32_ST_TYPE(sym->st_info) == STT_FUNC) {      /*  �÷����Ǻ�������            */

                        func = (void *)(mod->elf +                      /*  �÷����� ELF �ļ����λ��   */
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
