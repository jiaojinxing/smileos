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
** Descriptions:            主函数
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
*********************************************************************************************************/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "elf.h"

/*
 * ELF 文件格式请查看:
 *
 * http://os.pku.edu.cn:8080/gaikuang/submission/TN05.ELF.Format.Summary.pdf
 */
/*
 * 探测 ELF 文件
 */
int probe_elf(unsigned char *content, unsigned int size)
{
    Elf32_Ehdr *ehdr;

    ehdr = (Elf32_Ehdr *)content;                                       /*  ELF 首部                    */

    if ((ehdr->e_ident[EI_MAG0] != ELFMAG0) ||                          /*  魔数检查                    */
        (ehdr->e_ident[EI_MAG1] != ELFMAG1) ||
        (ehdr->e_ident[EI_MAG2] != ELFMAG2) ||
        (ehdr->e_ident[EI_MAG3] != ELFMAG3)) {
        printf("ehdr->e_ident[EI_MAG0] != ELFMAG0\n");
        return -1;
    }

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {                        /*  文件类型: 必须是 32 位目标  */
        printf("ehdr->e_ident[EI_CLASS] != ELFCLASS32\n");
        return -1;
    }

    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {                        /*  数据编码方式: 必须是小端    */
        printf("ehdr->e_ident[EI_DATA] != ELFDATA2LSB\n");
        return -1;
    }

    if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) {                      /*  ELF 首部版本号              */
        printf("ehdr->e_ident[EI_VERSION] != EV_CURRENT\n");
        return -1;
    }

    /*
     * 目标文件类型: 必须是 可重定位文件 或 共享目标文件
     */
    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN) {
        printf("ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN\n");
        return -1;
    }

    if (ehdr->e_machine != EM_ARM) {                                    /*  体系结构类型: 必须是 ARM    */
        printf("ehdr->e_machine != EM_ARM\n");
        return -1;
    }

    /*
     * 保证不存在程序首部表格相关的东西
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
     * 保证存在节区首部表格相关的东西
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
 * 对指定的 rel 条目进行重定位
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
 * 在系统里查找指定的符号
 */
unsigned int symbol_lookup(const char *name, unsigned int type);

/*
 * 重定位 ELF 文件
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

    ehdr = (Elf32_Ehdr *)content;                                       /*  ELF 首部                    */

    /*
     * 节区首部中的字符串的节区首部
     */
    shstrtab_shdr = (Elf32_Shdr *)(content + ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize);
    shstrtab      = (char *)content + shstrtab_shdr->sh_offset;

    /*
     * 先装载所有节区首部到局部数组
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
         * .bss 节区不存在 ELF 文件中, 需要分配出来
         */
        if (strcmp(shdr->sh_name + shstrtab, ".bss") == 0) {
            bss_idx = i;
            shdr->sh_offset = (Elf32_Off)calloc(1, shdr->sh_size);
            if (shdr->sh_offset == 0) {
                goto error1;
            }
        } else if (strcmp(shdr->sh_name + shstrtab, ".text") == 0) {    /*  顺便把 .text 节区也找出来吧 */
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
        char *strtab;
        unsigned char *symtab;
        unsigned char *target;

        if (shdr->sh_type == SHT_REL || shdr->sh_type == SHT_RELA) {

            if (shdr->sh_link == SHN_UNDEF) {
                goto error1;
            }
            symtab_shdr = shdrs[shdr->sh_link];                         /*  符号表节区首部              */

            if (shdr->sh_info == SHN_UNDEF) {
                goto error1;
            }
            target_shdr = shdrs[shdr->sh_info];                         /*  作用于的节区的节区首部      */

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                goto error1;
            }
            strtab_shdr = shdrs[symtab_shdr->sh_link];                  /*  字符串表节区首部            */

            symtab      = content + symtab_shdr->sh_offset;
            target      = content + target_shdr->sh_offset;
            strtab      = (char *)content + strtab_shdr->sh_offset;
        }

        if (shdr->sh_type == SHT_REL) {
            Elf32_Rel *rel;
            int j;

            /*
             * 处理所有的 rel 条目
             */
            for (j = 0, rel = (Elf32_Rel *)(content + shdr->sh_offset);
                 j < shdr->sh_size / shdr->sh_entsize;
                 j++,   rel = (Elf32_Rel *)((char *)rel + shdr->sh_entsize)) {

                int sym_idx;

                sym_idx = ELF32_R_SYM(rel->r_info);                     /*  这个 rel 条目的符号索引     */

                if (STN_UNDEF != sym_idx) {
                    Elf32_Sym  *sym;
                    Elf32_Addr  addr;
                    int ret;
                                                                        /*  这个 rel 条目的符号         */
                    sym = (Elf32_Sym *)(symtab + sym_idx * symtab_shdr->sh_entsize);

                    if (sym->st_shndx != SHN_UNDEF) {                   /*  如果这个符号在 ELF 文件里   */
                        /*
                         * 获得这个符号在 ELF 文件里地址
                         */
                        if (sym->st_shndx == bss_idx) {                 /*  如果这个符号在 .bss 节区    */
                            addr = (uint32_t)(shdrs[bss_idx]->sh_offset + sym->st_value);
                        } else {
                            addr = (uint32_t)(content + shdrs[sym->st_shndx]->sh_offset + sym->st_value);
                        }

                        /*
                         * 对该的 rel 条目进行重定位
                         */
                        ret = arm_reloc_rel(rel, addr, target);
                        if (ret < 0) {
                            goto error1;
                        }
                    } else {
                        /*
                         * 在系统里查找这个符号
                         */
                        addr = symbol_lookup(sym->st_name + strtab, ELF32_ST_TYPE(sym->st_info));
                        if (addr == 0) {
                            printf("symbol %s no found\n", sym->st_name + strtab);
                            goto error1;
                        } else {
                            /*
                             * 对该的 rel 条目进行重定位
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
     * 刚才拷贝的代码还在 D-Cache, 进程运行时读取 I-Cache 或主存, 它们是不一致的!
     * 所以必须要清除 D-Cache, 清除写缓冲, 还要无效 I-Cache,
     * 无效 D-Cache 可让出 D-Cache, 减少其它进程的 D-Cache 被换出和 D-Cache Miss, 提升运行速度
     *
     * ARM 数据访问的基本流程图:
     * http://www.ibm.com/developerworks/cn/linux/l-skyeye/part3/s3/index.html
     */

    /*
     * 执行 main 函数
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
            strtab_shdr = shdrs[symtab_shdr->sh_link];                  /*  字符串表节区首部            */
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
                        break;
                    }
                }
            }
        }
    }

    /*
     * 释放掉 .bss 节区
     */
    if (bss_idx != SHN_UNDEF) {
        free((void *)shdrs[bss_idx]->sh_offset);
    }

    /*
     * 释放节区首部数组
     */
    free(shdrs);

    return 0;

    error1:
    free(shdrs);

    error0:
    return -1;
}

/*
 * 加载 ELF 文件
 */
int load_elf(const char *path, int argc, char *argv[])
{
    int fd;
    int ret;
    unsigned char *buf, *content;
    struct stat st;
    int len;

    ret = access(path, R_OK);                                           /*  看下这个文件能否访问        */
    if (ret < 0) {
        printf("%s: failed to access %s\n", __func__, path);
        return -1;
    }

    fd = open(path, O_RDONLY);                                          /*  打开文件                    */
    if (fd < 0) {
        printf("%s: failed to open %s\n", __func__, path);
        return -1;
    }

    ret = fstat(fd, &st);                                               /*  获得文件的大小              */
    if (ret < 0 || st.st_size == 0) {
        printf("%s: failed to stat %s or %s is empty\n", __func__, path, path);
        return -1;
    }

    content = buf = malloc(st.st_size);                                 /*  分配缓冲                    */
    if (buf < 0) {
        printf("%s: failed to malloc %d byte\n", __func__, st.st_size);
        close(fd);
        return -1;
    }

    len = 0;                                                            /*  将文件的内容读到缓冲区      */
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

    close(fd);                                                          /*  关闭文件                    */

    ret = probe_elf(content, st.st_size);                               /*  探测 ELF 文件               */
    if (ret < 0) {
        printf("%s: %s is not a elf file\n", __func__, path);
        free(content);
        return -1;
    }

    ret = reloc_elf(content, st.st_size, argc, argv);                   /*  重定位 ELF 文件             */
    if (ret < 0) {
        printf("%s: failed to reloc %s\n", __func__, path);
        free(content);
        return -1;
    }

    free(content);                                                      /*  释放缓冲区                  */

    return 0;
}

/*
 * main 函数
 */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s file.elf\n", argv[0]);
        return -1;
    }

    extern int symbol_add(void);
    symbol_add();                                                       /*  增加系统的符号              */

    return load_elf(argv[1], argc - 1, &argv[1]);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
