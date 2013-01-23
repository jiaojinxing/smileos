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
** Descriptions:            �ں�ģ��֧��
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
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/mmu.h"
#include "kern/ipc.h"
#include "vfs/utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <syslimits.h>

#include "module/elf.h"
#include "module/symbol_tool.h"
#include "module/module.h"
/*********************************************************************************************************
** ELF �ļ���ʽ��鿴:
**
** http://os.pku.edu.cn:8080/gaikuang/submission/TN05.ELF.Format.Summary.pdf
*********************************************************************************************************/

/*********************************************************************************************************
** ȫ�ֱ���
*********************************************************************************************************/
/*
 * ģ������
 */
static module_t    *mod_list;

/*
 * ģ�������
 */
mutex_t             mod_mgr_lock;
/*********************************************************************************************************
** Function name:           arm_reloc_rel
** Descriptions:            ��ָ���� rel ��Ŀ�����ض�λ
** input parameters:        rel                 REL ��Ŀ
**                          addr                ��ַ
**                          target              Ŀ¼����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int arm_reloc_rel(Elf32_Rel *rel, Elf32_Addr addr, uint8_t *target)
{
    Elf32_Addr *where, tmp;
    Elf32_Sword addend;

    where = (Elf32_Addr *)(target + rel->r_offset);

    switch (ELF32_R_TYPE(rel->r_info)) {
    case R_ARM_NONE:
        break;

    case R_ARM_ABS32:
        *where += (Elf32_Addr)addr;
        break;

    case R_ARM_PC24:
    case R_ARM_PLT32:
    case R_ARM_CALL:
    case R_ARM_JUMP24:
        addend = *where & 0x00ffffff;
        if (addend & 0x00800000) {
            addend |= 0xff000000;
        }
        tmp    = addr - (Elf32_Addr)where + (addend << 2);
        tmp  >>= 2;
        *where = (*where & 0xff000000) | (tmp & 0x00ffffff);
        break;

    default:
        return -1;
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           module_probe
** Descriptions:            ̽��ģ��
** input parameters:        module              ģ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_probe(module_t *module)
{
    Elf32_Ehdr *ehdr;

    ehdr = (Elf32_Ehdr *)module->elf;                                   /*  ELF �ײ�                    */

    if ((ehdr->e_ident[EI_MAG0] != ELFMAG0) ||                          /*  ħ�����                    */
        (ehdr->e_ident[EI_MAG1] != ELFMAG1) ||
        (ehdr->e_ident[EI_MAG2] != ELFMAG2) ||
        (ehdr->e_ident[EI_MAG3] != ELFMAG3)) {
        printk("ehdr->e_ident[EI_MAG0] != ELFMAG0\n");
        return -1;
    }

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {                        /*  �ļ�����: ������ 32 λĿ��  */
        printk("ehdr->e_ident[EI_CLASS] != ELFCLASS32\n");
        return -1;
    }

    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {                        /*  ���ݱ��뷽ʽ: ������С��    */
        printk("ehdr->e_ident[EI_DATA] != ELFDATA2LSB\n");
        return -1;
    }

    if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) {                      /*  ELF �ײ��汾��              */
        printk("ehdr->e_ident[EI_VERSION] != EV_CURRENT\n");
        return -1;
    }

    /*
     * Ŀ���ļ�����: ������ ���ض�λ�ļ� �� ����Ŀ���ļ�
     */
    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN) {
        printk("ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN\n");
        return -1;
    }

    if (ehdr->e_machine != EM_ARM) {                                    /*  ��ϵ�ṹ����: ������ ARM    */
        printk("ehdr->e_machine != EM_ARM\n");
        return -1;
    }

    /*
     * ��֤�����ڳ����ײ������صĶ���
     */
    if (ehdr->e_phoff != 0) {
        printk("ehdr->e_phoff != 0\n");
        return -1;
    }

    if (ehdr->e_phentsize != 0) {
        printk("ehdr->e_phentsize != 0\n");
        return -1;
    }

    if (ehdr->e_phnum != 0) {
        printk("ehdr->e_phnum != 0\n");
        return -1;
    }

    /*
     * ��֤���ڽ����ײ������صĶ���
     */
    if (ehdr->e_shoff == 0) {
        printk("ehdr->e_shoff == 0\n");
    }

    if (ehdr->e_shentsize == 0) {
        printk("ehdr->e_shentsize == 0\n");
    }

    if (ehdr->e_shnum == 0) {
        printk("ehdr->e_shnum == 0\n");
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           module_reloc
** Descriptions:            �ض�λģ��
** input parameters:        module              ģ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_reloc(module_t *module)
{
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    Elf32_Shdr **shdrs;
    Elf32_Shdr *shstrtab_shdr;
    char       *shstrtab;
    int         i;
    int         bss_idx;
    int         text_idx;

    ehdr = (Elf32_Ehdr *)module->elf;                                   /*  ELF �ײ�                    */

    /*
     * �����ײ��е��ַ����Ľ����ײ�
     */
    shstrtab_shdr = (Elf32_Shdr *)(module->elf + ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize);
    shstrtab      = (char *)module->elf + shstrtab_shdr->sh_offset;

    /*
     * ��װ�����н����ײ����ֲ�����
     */
    shdrs = kmalloc(ehdr->e_shnum * sizeof(Elf32_Shdr *));
    if (shdrs == NULL) {
        goto error0;
    }

    bss_idx  = SHN_UNDEF;
    text_idx = SHN_UNDEF;

    for (i = 0; i < ehdr->e_shnum; i++) {
        shdrs[i] = shdr = (Elf32_Shdr *)(module->elf + ehdr->e_shoff + ehdr->e_shentsize * i);
        /*
         * .bss ���������� ELF �ļ���, ��Ҫ�������
         */
        if (strcmp(shdr->sh_name + shstrtab, ".bss") == 0) {
            bss_idx = i;
            shdr->sh_offset = (Elf32_Off)kcalloc(1, shdr->sh_size);
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
        uint8_t    *target;

        if (shdr->sh_type == SHT_REL || shdr->sh_type == SHT_RELA) {

            if (shdr->sh_link == SHN_UNDEF) {
                goto error1;
            }
            symtab_shdr = shdrs[shdr->sh_link];                         /*  ���ű�����ײ�              */

            if (shdr->sh_info == SHN_UNDEF) {
                goto error1;
            }
            target_shdr = shdrs[shdr->sh_info];                         /*  �����ڵĽ����Ľ����ײ�      */

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                goto error1;
            }
            strtab_shdr = shdrs[symtab_shdr->sh_link];                  /*  �ַ���������ײ�            */

            symtab      = module->elf + symtab_shdr->sh_offset;
            target      = module->elf + target_shdr->sh_offset;
            strtab      = (char *)module->elf + strtab_shdr->sh_offset;
        }

        if (shdr->sh_type == SHT_REL) {
            Elf32_Rel *rel;
            int        j;

            /*
             * �������е� rel ��Ŀ
             */
            for (j = 0, rel = (Elf32_Rel *)(module->elf + shdr->sh_offset);
                 j < shdr->sh_size / shdr->sh_entsize;
                 j++,   rel = (Elf32_Rel *)((char *)rel + shdr->sh_entsize)) {

                int sym_idx;

                sym_idx = ELF32_R_SYM(rel->r_info);                     /*  ��� rel ��Ŀ�ķ�������     */

                if (STN_UNDEF != sym_idx) {
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
                            addr = (uint32_t)(shdrs[bss_idx]->sh_offset + sym->st_value);
                        } else {
                            addr = (uint32_t)(module->elf + shdrs[sym->st_shndx]->sh_offset + sym->st_value);
                        }

                        /*
                         * �Ըõ� rel ��Ŀ�����ض�λ
                         */
                        ret = arm_reloc_rel(rel, addr, target);
                        if (ret < 0) {
                            goto error1;
                        }
                    } else {
                        /*
                         * ��ϵͳ������������
                         */
                        extern uint32_t symbol_lookup(const char *name, uint32_t type);
                        addr = symbol_lookup(sym->st_name + strtab, ELF32_ST_TYPE(sym->st_info));
                        if (addr == 0) {
                            printk("symbol %s no found\n", sym->st_name + strtab);
                            goto error1;
                        } else {
                            /*
                             * �Ըõ� rel ��Ŀ�����ض�λ
                             */
                            ret = arm_reloc_rel(rel, addr, target);
                            if (ret < 0) {
                                goto error1;
                            }
                        }
                    }
                }
            }
        } else if (shdr->sh_type == SHT_RELA) {
            goto error1;
        }
    }

    /*
     * �ղſ����Ĵ��뻹�� D-Cache, ģ������ʱ��ȡ I-Cache ������, �����ǲ�һ�µ�!
     * ���Ա���Ҫ��� D-Cache, ���д����, ��Ҫ��Ч I-Cache,
     * ��Ч D-Cache ���ó� D-Cache, �����������̵� D-Cache �������� D-Cache Miss, ���������ٶ�
     *
     * ARM ���ݷ��ʵĻ�������ͼ:
     * http://www.ibm.com/developerworks/cn/linux/l-skyeye/part3/s3/index.html
     */
    {
        int i;

        /*
         * �������Ч D-Cache
         */
        for (i = 0; i < (module->size + 31) / 32; i++) {
            mmu_clean_invalidate_dcache_mva((((uint32_t)module->elf) & (~31)) + i * 32);
        }

        /*
         * ���д����
         */
        mmu_drain_write_buffer();

        /*
         * ��Ч I-Cache
         */
        for (i = 0; i < (module->size + 31) / 32; i++) {
            mmu_invalidate_icache_mva((((uint32_t)module->elf) & (~31)) + i * 32);
        }
    }

    module->shdrs    = shdrs;
    module->bss_idx  = bss_idx;
    module->text_idx = text_idx;

    return 0;

    error1:
    kfree(shdrs);

    error0:
    return -1;
}
/*********************************************************************************************************
** Function name:           module_exec
** Descriptions:            ִ��ģ��
** input parameters:        module              ģ��
**                          argc                ��������
**                          argv                ��������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_exec(module_t *module, const char *func_name, int argc, char **argv)
{
    int         i;
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    int (*entry)(int argc, char **argv);

    ehdr = (Elf32_Ehdr *)module->elf;                                   /*  ELF �ײ�                    */

    /*
     * ִ�� module_init ����
     */
    for (i = 0, shdr = module->shdrs[0]; i < ehdr->e_shnum; i++, shdr = module->shdrs[i]) {

        if (shdr->sh_type == SHT_SYMTAB) {
            Elf32_Shdr *symtab_shdr;
            Elf32_Shdr *strtab_shdr;
            char       *strtab;
            uint8_t    *symtab;
            Elf32_Sym  *sym;
            int         j;

            symtab_shdr = shdr;
            symtab      = module->elf + symtab_shdr->sh_offset;

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                break;
            }
            strtab_shdr = module->shdrs[symtab_shdr->sh_link];          /*  �ַ���������ײ�            */
            strtab      = (char *)module->elf + strtab_shdr->sh_offset;

            for (j = 0,
                 sym = (Elf32_Sym *)symtab;
                 j < symtab_shdr->sh_size / symtab_shdr->sh_entsize;
                 j++, sym = (Elf32_Sym *)((char *)sym + symtab_shdr->sh_entsize)) {

                if (sym->st_shndx == module->text_idx &&
                    ELF32_ST_TYPE(sym->st_info) == STT_FUNC) {
                    if (strcmp(sym->st_name + strtab, func_name) == 0) {
                        entry = (int (*)(int, char **))(module->elf + module->shdrs[module->text_idx]->sh_offset + sym->st_value);
                        return entry(argc, argv);
                    }
                }
            }
        }
    }
    return -1;
}
/*********************************************************************************************************
** Function name:           module_install
** Descriptions:            ��װģ��
** input parameters:        dev                 ģ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_install(module_t *mod)
{
    mutex_lock(&mod_mgr_lock, 0);
    mod->next = mod_list;
    mod_list  = mod;
    mutex_unlock(&mod_mgr_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_lookup
** Descriptions:            ����ģ��
** input parameters:        name                ģ����
** output parameters:       NONE
** Returned value:          ģ�� OR NULL
*********************************************************************************************************/
module_t *module_lookup(const char *name)
{
    module_t *mod;
    unsigned int key;

    if (name == NULL) {
        return NULL;
    }

    key = BKDRHash(name);

    mutex_lock(&mod_mgr_lock, 0);
    mod = mod_list;
    while (mod != NULL) {
        if (key == mod->key) {
            break;
        }
        mod = mod->next;
    }
    mutex_unlock(&mod_mgr_lock);

    return mod;
}
/*********************************************************************************************************
** Function name:           module_lookup_by_key
** Descriptions:            ����ģ��
** input parameters:        name                ģ����
** output parameters:       NONE
** Returned value:          ģ�� OR NULL
*********************************************************************************************************/
static module_t *module_lookup_by_key(unsigned int key)
{
    module_t *mod;

    mutex_lock(&mod_mgr_lock, 0);
    mod = mod_list;
    while (mod != NULL) {
        if (key == mod->key) {
            break;
        }
        mod = mod->next;
    }
    mutex_unlock(&mod_mgr_lock);

    return mod;
}
/*********************************************************************************************************
** Function name:           module_ref_by_addr
** Descriptions:            ����ģ��ͨ��ģ���һ����ַ
** input parameters:        addr                ��ַ
** output parameters:       NONE
** Returned value:          ģ�� OR NULL
*********************************************************************************************************/
module_t *module_ref_by_addr(void *addr)
{
    module_t *mod;

    mutex_lock(&mod_mgr_lock, 0);
    mod = mod_list;
    while (mod != NULL) {
        if (((char *)addr >= (char *)mod + sizeof(module_t)) && ((char *)addr < (char *)mod + sizeof(module_t) + mod->size)) {
            break;
        }
        mod = mod->next;
    }


    if (mod != NULL) {
        atomic_ref(&mod->ref);
    }
    mutex_unlock(&mod_mgr_lock);

    return mod;
}
/*********************************************************************************************************
** Function name:           module_remove
** Descriptions:            ɾ��ģ��
** input parameters:        name                ģ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int module_remove(unsigned int key)
{
    module_t *mod, *prev;
    int ret = -1;

    mutex_lock(&mod_mgr_lock, 0);
    prev = NULL;
    mod  = mod_list;
    while (mod != NULL) {
        if (key == mod->key) {
            break;
        }
        prev = mod;
        mod  = mod->next;
    }

    if (mod != NULL) {
        if (prev != NULL) {
            prev->next = mod->next;
        } else {
            mod_list = mod->next;
        }
        kfree(mod);
        ret = 0;
    }
    mutex_unlock(&mod_mgr_lock);

    return ret;
}
/*********************************************************************************************************
** Function name:           module_exec
** Descriptions:            ���� ELF �ļ�
** input parameters:        module              ģ��
**                          argc                ��������
**                          argv                ��������
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
    module_t    *module;

    if (path == NULL) {
        printk("%s: path = NULL\n", __func__);
        return -1;
    }

    ret = access(path, R_OK);                                           /*  ��������ļ��ܷ����        */
    if (ret < 0) {
        printk("%s: failed to access %s\n", __func__, path);
        return -1;
    }

    fd = open(path, O_RDONLY);                                          /*  ���ļ�                    */
    if (fd < 0) {
        printk("%s: failed to open %s\n", __func__, path);
        return -1;
    }

    ret = fstat(fd, &st);                                               /*  ����ļ��Ĵ�С              */
    if (ret < 0 || st.st_size == 0) {
        printk("%s: failed to stat %s or %s is empty\n", __func__, path, path);
        return -1;
    }

    module = kmalloc(sizeof(module_t) + st.st_size);                    /*  ���仺��                    */
    if (module == NULL) {
        printk("%s: failed to kmalloc %d byte\n", __func__, st.st_size);
        close(fd);
        return -1;
    }

    strcpy(module->name, path);
    module->key  = BKDRHash(path);
    buf          = (uint8_t *)module + sizeof(module_t);
    module->elf  = buf;
    module->size = st.st_size;
    atomic_set(&module->ref, 0);

    len = 0;                                                            /*  ���ļ������ݶ���������      */
    while (st.st_size - len > 0) {
        ret = read(fd, buf, st.st_size - len);
        if (ret < 0) {
            printk("%s: failed to read %s %d byte at %d\n", __func__, path, st.st_size - len, len);
            kfree(module);
            close(fd);
            return -1;
        }
        buf += ret;
        len += ret;
    }

    close(fd);                                                          /*  �ر��ļ�                    */

    ret = module_probe(module);                                         /*  ̽��ģ��                    */
    if (ret < 0) {
        printk("%s: %s is not a module file\n", __func__, path);
        kfree(module);
        return -1;
    }

    ret = module_reloc(module);                                         /*  �ض�λģ��                  */
    if (ret < 0) {
        printk("%s: failed to reloc %s\n", __func__, path);
        kfree(module);
        return -1;
    }

    mutex_lock(&mod_mgr_lock, 0);

    if (module_lookup_by_key(module->key) != NULL) {

        mutex_unlock(&mod_mgr_lock);

        if (module->bss_idx != SHN_UNDEF) {                             /*  �ͷŵ� .bss ����            */
            kfree((void *)module->shdrs[module->bss_idx]->sh_offset);
        }

        kfree(module->shdrs);                                           /*  �ͷŽ����ײ�����            */

        kfree(module);
        return -1;
    }

    ret = module_exec(module, "module_init", argc, argv);               /*  ִ��ģ��                    */
    if (ret < 0) {

        mutex_unlock(&mod_mgr_lock);

        printk("%s: failed to exec %s module_init\n", __func__, path);

        if (module->bss_idx != SHN_UNDEF) {                             /*  �ͷŵ� .bss ����            */
            kfree((void *)module->shdrs[module->bss_idx]->sh_offset);
        }

        kfree(module->shdrs);                                           /*  �ͷŽ����ײ�����            */

        kfree(module);

        return -1;
    }

    module_install(module);

    mutex_unlock(&mod_mgr_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           module_unload
** Descriptions:            ж��ģ��
** input parameters:        module              ģ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unload(const char *path)
{
    module_t *module;
    int ret;

    if (path == NULL) {
        printk("%s: path = NULL\n", __func__);
        return -1;
    }

    module = module_lookup(path);
    if (module == NULL) {
        printk("%s: module %s no found\n", __func__, path);
        return -1;
    }

    ret = module_exec(module, "module_exit", 0, NULL);
    if (ret < 0) {
        printk("%s: failed to exec %s module_exit\n", __func__, path);
        return -1;
    }

    if (atomic_read(&module->ref) != 0) {
        printk("%s: module %s is busy\n", __func__, path);
        return -1;
    }

    /*
     * �ͷŵ� .bss ����
     */
    if (module->bss_idx != SHN_UNDEF) {
        kfree((void *)module->shdrs[module->bss_idx]->sh_offset);
    }

    /*
     * �ͷŽ����ײ�����
     */
    kfree(module->shdrs);

    module_remove(module->key);

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
