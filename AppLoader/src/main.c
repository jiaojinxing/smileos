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
** File name:               main.c
** Last modified Date:      2012-7-18
** Last Version:            1.0.0
** Descriptions:            ������
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
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "elf.h"

/*
 * ELF �ļ���ʽ��鿴:
 *
 * http://os.pku.edu.cn:8080/gaikuang/submission/TN05.ELF.Format.Summary.pdf
 */
/*
 * ̽�� ELF �ļ�
 */
int probe_elf(unsigned char *content, unsigned int size)
{
    Elf32_Ehdr *ehdr;

    ehdr = (Elf32_Ehdr *)content;                                       /*  ELF �ײ�                    */

    if ((ehdr->e_ident[EI_MAG0] != ELFMAG0) ||                          /*  ħ�����                    */
        (ehdr->e_ident[EI_MAG1] != ELFMAG1) ||
        (ehdr->e_ident[EI_MAG2] != ELFMAG2) ||
        (ehdr->e_ident[EI_MAG3] != ELFMAG3)) {
        printf("ehdr->e_ident[EI_MAG0] != ELFMAG0\n");
        return -1;
    }

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {                        /*  �ļ�����: ������ 32 λĿ��  */
        printf("ehdr->e_ident[EI_CLASS] != ELFCLASS32\n");
        return -1;
    }

    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {                        /*  ���ݱ��뷽ʽ: ������С��    */
        printf("ehdr->e_ident[EI_DATA] != ELFDATA2LSB\n");
        return -1;
    }

    if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) {                      /*  ELF �ײ��汾��              */
        printf("ehdr->e_ident[EI_VERSION] != EV_CURRENT\n");
        return -1;
    }

    /*
     * Ŀ���ļ�����: ������ ���ض�λ�ļ� �� ����Ŀ���ļ�
     */
    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN) {
        printf("ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN\n");
        return -1;
    }

    if (ehdr->e_machine != EM_ARM) {                                    /*  ��ϵ�ṹ����: ������ ARM    */
        printf("ehdr->e_machine != EM_ARM\n");
        return -1;
    }

    /*
     * ��֤�����ڳ����ײ������صĶ���
     */
    if (ehdr->e_phoff != 0) {
        printf("ehdr->e_phoff != 0\n");
        return -1;
    }

    if (ehdr->e_phentsize != 0) {
        printf("ehdr->e_phentsize != 0\n");
        return -1;
    }

    if (ehdr->e_phnum != 0) {
        printf("ehdr->e_phnum != 0\n");
        return -1;
    }

    /*
     * ��֤���ڽ����ײ������صĶ���
     */
    if (ehdr->e_shoff == 0) {
        printf("ehdr->e_shoff == 0\n");
    }

    if (ehdr->e_shentsize == 0) {
        printf("ehdr->e_shentsize == 0\n");
    }

    if (ehdr->e_shnum == 0) {
        printf("ehdr->e_shnum == 0\n");
    }

    return 0;
}

/*
 * ��ָ���� rel ��Ŀ�����ض�λ
 */
int arm_reloc_rel(Elf32_Rel *rel, Elf32_Addr addr, unsigned char *target)
{
    Elf32_Addr *where, tmp;
    Elf32_Sword addend;

    where = (Elf32_Addr *)(target + rel->r_offset);

    switch (ELF32_R_TYPE(rel->r_info)) {
    case R_ARM_NONE:
        break;

    case R_ARM_ABS32:
        *where += (Elf32_Addr)addr;
        //printf("R_ARM_ABS32: %x -> %x\n", where, *where);
        break;

    case R_ARM_PC24:
    case R_ARM_PLT32:
    case R_ARM_CALL:
    case R_ARM_JUMP24:
        addend = *where & 0x00ffffff;
        if (addend & 0x00800000) {
            addend |= 0xff000000;
        }
        tmp = addr - (Elf32_Addr)where + (addend << 2);
        tmp >>= 2;
        *where = (*where & 0xff000000) | (tmp & 0x00ffffff);
        //printf("R_ARM_PC24: %x -> %x\n", where, *where);
        break;

    default:
        return -1;
    }
    return 0;
}

/*
 * ��ϵͳ�����ָ���ķ���
 */
unsigned int symbol_lookup(const char *name, unsigned int type);

/*
 * �ض�λ ELF �ļ�
 */
int reloc_elf(unsigned char *content, unsigned int size, int argc, char *argv[])
{
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    Elf32_Shdr **shdrs;
    Elf32_Shdr *shstrtab_shdr;
    char *shstrtab;
    int i;
    int bss_idx;
    int text_idx;
    int (*entry)(int, char **);
//    unsigned char **contents;

    ehdr = (Elf32_Ehdr *)content;                                       /*  ELF �ײ�                    */

    /*
     * �����ײ��е��ַ����Ľ����ײ�
     */
    shstrtab_shdr = (Elf32_Shdr *)(content + ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize);
    shstrtab      = (char *)content + shstrtab_shdr->sh_offset;

    /*
     * ��װ�����н����ײ����ֲ�����
     */
    shdrs = malloc(ehdr->e_shnum * sizeof(Elf32_Shdr *));
    if (shdrs == NULL) {
        goto error0;
    }

//    contents = malloc(ehdr->e_shnum * sizeof(unsigned char *));
//    if (contents == NULL) {
//        goto error1;
//    }

    bss_idx  = SHN_UNDEF;
    text_idx = SHN_UNDEF;

    for (i = 0; i < ehdr->e_shnum; i++) {
        shdrs[i] = shdr = (Elf32_Shdr *)(content + ehdr->e_shoff + ehdr->e_shentsize * i);

        /*
         * .bss ���������� ELF �ļ���, ��Ҫ�������
         */
        if (strcmp(shdr->sh_name + shstrtab, ".bss") == 0) {
            bss_idx = i;
            shdr->sh_offset = (Elf32_Off)calloc(1, shdr->sh_size);
            if (shdr->sh_offset == 0) {
                goto error1;
            }
        } else if (strcmp(shdr->sh_name + shstrtab, ".text") == 0) {    /*  ˳��� .text ����Ҳ�ҳ����� */
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
        char *strtab;
        unsigned char *symtab;
        unsigned char *target;

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

            symtab      = content + symtab_shdr->sh_offset;
            target      = content + target_shdr->sh_offset;
            strtab      = (char *)content + strtab_shdr->sh_offset;
        }

        if (shdr->sh_type == SHT_REL) {
            Elf32_Rel *rel;
            int j;

            /*
             * �������е� rel ��Ŀ
             */
            for (j = 0, rel = (Elf32_Rel *)(content + shdr->sh_offset);
                 j < shdr->sh_size / shdr->sh_entsize;
                 j++,   rel = (Elf32_Rel *)((char *)rel + shdr->sh_entsize)) {

                int sym_idx;

                sym_idx = ELF32_R_SYM(rel->r_info);                     /*  ��� rel ��Ŀ�ķ�������     */

                if (STN_UNDEF != sym_idx) {
                    Elf32_Sym  *sym;
                    Elf32_Addr  addr;
                    int ret;
                                                                        /*  ��� rel ��Ŀ�ķ���         */
                    sym = (Elf32_Sym *)(symtab + sym_idx * symtab_shdr->sh_entsize);

                    if (sym->st_shndx != SHN_UNDEF) {                   /*  ������������ ELF �ļ���   */
                        /*
                         * ������������ ELF �ļ����ַ
                         */
                        if (sym->st_shndx == bss_idx) {                 /*  ������������ .bss ����    */
                            addr = (uint32_t)(shdrs[bss_idx]->sh_offset + sym->st_value);
                        } else {
                            addr = (uint32_t)(content + shdrs[sym->st_shndx]->sh_offset + sym->st_value);
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
                        addr = symbol_lookup(sym->st_name + strtab, ELF32_ST_TYPE(sym->st_info));
                        if (addr == 0) {
                            printf("symbol %s no found\n", sym->st_name + strtab);
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
     * TODO:
     *
     * �ղſ����Ĵ��뻹�� D-Cache, ��������ʱ��ȡ I-Cache ������, �����ǲ�һ�µ�!
     * ���Ա���Ҫ��� D-Cache, ���д����, ��Ҫ��Ч I-Cache,
     * ��Ч D-Cache ���ó� D-Cache, �����������̵� D-Cache �������� D-Cache Miss, ���������ٶ�
     *
     * ARM ���ݷ��ʵĻ�������ͼ:
     * http://www.ibm.com/developerworks/cn/linux/l-skyeye/part3/s3/index.html
     */

    /*
     * ִ�� main ����
     */
    for (i = 0, shdr = shdrs[0]; i < ehdr->e_shnum; i++, shdr = shdrs[i]) {

        if (shdr->sh_type == SHT_SYMTAB) {
            Elf32_Shdr *symtab_shdr;
            Elf32_Shdr *strtab_shdr;
            char *strtab;
            unsigned char *symtab;
            Elf32_Sym  *sym;
            int j;

            symtab_shdr = shdr;
            symtab      = content + symtab_shdr->sh_offset;

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                break;
            }
            strtab_shdr = shdrs[symtab_shdr->sh_link];                  /*  �ַ���������ײ�            */
            strtab      = (char *)content + strtab_shdr->sh_offset;

            for (j = 0,
                 sym = (Elf32_Sym *)symtab;
                 j < symtab_shdr->sh_size / symtab_shdr->sh_entsize;
                 j++, sym = (Elf32_Sym *)((char *)sym + symtab_shdr->sh_entsize)) {

                if (sym->st_shndx == text_idx &&
                    ELF32_ST_TYPE(sym->st_info) == STT_FUNC) {
                    if (strcmp(sym->st_name + strtab, "main") == 0) {
                        entry = (int (*)(int, char **))(content + shdrs[text_idx]->sh_offset + sym->st_value);
                        entry(argc, argv);
                    }
                }
            }
        }
    }

    /*
     * �ͷŵ� .bss ����
     */
    if (bss_idx != SHN_UNDEF) {
        free((void *)shdrs[bss_idx]->sh_offset);
    }

    /*
     * �ͷŽ����ײ�����
     */
    free(shdrs);

    return 0;

    error1:
    free(shdrs);

    error0:
    return -1;
}

/*
 * ���� ELF �ļ�
 */
int load_elf(const char *path, int argc, char *argv[])
{
    int fd;
    int ret;
    unsigned char *buf, *content;
    struct stat st;
    int len;

    ret = access(path, R_OK);                                           /*  ��������ļ��ܷ����        */
    if (ret < 0) {
        printf("%s: failed to access %s\n", __func__, path);
        return -1;
    }

    fd = open(path, O_RDONLY);                                          /*  ���ļ�                    */
    if (fd < 0) {
        printf("%s: failed to open %s\n", __func__, path);
        return -1;
    }

    ret = fstat(fd, &st);                                               /*  ����ļ��Ĵ�С              */
    if (ret < 0 || st.st_size == 0) {
        printf("%s: failed to stat %s or %s is empty\n", __func__, path, path);
        return -1;
    }

    content = buf = malloc(st.st_size);                                 /*  ���仺��                    */
    if (buf < 0) {
        printf("%s: failed to malloc %d byte\n", __func__, st.st_size);
        close(fd);
        return -1;
    }

    len = 0;                                                            /*  ���ļ������ݶ���������      */
    while (st.st_size - len > 0) {
        ret = read(fd, buf, st.st_size - len);
        if (ret < 0) {
            printf("%s: failed to read %s %d byte at %d\n", __func__, path, st.st_size - len, len);
            free(content);
            close(fd);
            return -1;
        }
        buf += ret;
        len += ret;
    }

    close(fd);                                                          /*  �ر��ļ�                    */

    ret = probe_elf(content, st.st_size);                               /*  ̽�� ELF �ļ�               */
    if (ret < 0) {
        printf("%s: %s is not a elf file\n", __func__, path);
        free(content);
        return -1;
    }

    ret = reloc_elf(content, st.st_size, argc, argv);                   /*  �ض�λ ELF �ļ�             */
    if (ret < 0) {
        printf("%s: failed to reloc %s\n", __func__, path);
        free(content);
        return -1;
    }

    free(content);                                                      /*  �ͷŻ�����                  */

    return 0;
}

/*
 * main ����
 */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s file.elf\n", argv[0]);
        return -1;
    }

    extern int symbol_add(void);
    symbol_add();                                                       /*  ����ϵͳ�ķ���              */

    return load_elf(argv[1], argc - 1, &argv[1]);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
