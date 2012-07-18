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
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            主函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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

#include "elf.h"

/*
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
 * 重定位 ELF 文件
 */
int reloc_elf(unsigned char *content, unsigned int size)
{
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    Elf32_Shdr *shstr_shdr;
    Elf32_Shdr **shdrs;
    Elf32_Shdr *rel_shdr;
    Elf32_Shdr *symtab_shdr;
    Elf32_Shdr *strtab_shdr;
    Elf32_Sym  *sym;
    unsigned char *symtab;
    unsigned char *rel;
    char *str;
    char *shstr;
    int i;
    int sym_index;

    ehdr = (Elf32_Ehdr *)content;                                       /*  ELF 首部                    */

    /*
     * 节区首部中的字符串的节区首部
     */
    shstr_shdr = (Elf32_Shdr *)(content + ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize);
    shstr      = (char *)content + shstr_shdr->sh_offset;

    shdr = (Elf32_Shdr *)(content + ehdr->e_shoff);

    /*
     * 先装载所有节区首部到局部数组
     */
    shdrs = malloc(ehdr->e_shnum * sizeof(Elf32_Shdr *));
    if (shdrs == NULL) {
        goto error0;
    }

    for (i = 0; i < ehdr->e_shnum; i++) {
        shdrs[i] = (Elf32_Shdr *)(content + ehdr->e_shoff + ehdr->e_shentsize * i);
    }

    /*
     * 处理所有需要重定位的节区
     */
    for (i = 0, shdr = shdrs[0]; i < ehdr->e_shnum; i++, shdr = shdrs[i]) {

        printf("-------- section %d --------\n", i);
        printf("shdr->sh_name       = %s\n",    shdr->sh_name + shstr);
        printf("shdr->sh_type       = %d\n",    shdr->sh_type);
        printf("shdr->sh_flags      = %d\n",    shdr->sh_flags);
        printf("shdr->sh_addr       = %d\n",    shdr->sh_addr);
        printf("shdr->sh_offset     = %d\n",    shdr->sh_offset);
        printf("shdr->sh_size       = %d\n",    shdr->sh_size);
        printf("shdr->sh_link       = %d\n",    shdr->sh_link);
        printf("shdr->sh_info       = %d\n",    shdr->sh_info);
        printf("shdr->sh_addralign  = %d\n",    shdr->sh_addralign);
        printf("shdr->sh_entsize    = %d\n",    shdr->sh_entsize);

        if (shdr->sh_type == SHT_REL || shdr->sh_type == SHT_RELA) {

            if (shdr->sh_link == SHN_UNDEF) {
                goto error1;
            }
            symtab_shdr = shdrs[shdr->sh_link];                         /*  符号表节区首部              */

            if (shdr->sh_info == SHN_UNDEF) {
                goto error1;
            }
            rel_shdr = shdrs[shdr->sh_info];                            /*  作用于的节区的节区首部      */

            if (symtab_shdr->sh_link == SHN_UNDEF) {
                goto error1;
            }
            strtab_shdr = shdrs[symtab_shdr->sh_link];                  /*  字符串表节区首部            */

            symtab      = content + symtab_shdr->sh_offset;
            rel         = content + rel_shdr->sh_offset;
            str         = (char *)content + strtab_shdr->sh_offset;
        }

        if (shdr->sh_type == SHT_REL) {
            Elf32_Rel  *rel_entry;

            for (i = 0, rel_entry = (Elf32_Rel *)(content + shdr->sh_offset);
                 i < shdr->sh_size / shdr->sh_entsize;
                 i++,   rel_entry = (Elf32_Rel *)((char *)rel_entry + shdr->sh_entsize)) {

                sym_index = ELF32_R_SYM(rel_entry->r_info);
                if (STN_UNDEF != sym_index) {
                    sym = (Elf32_Sym *)(symtab + sym_index * symtab_shdr->sh_entsize);
                    printf("sym name=%s, type=%d, offset=%d\n",  sym->st_name + str, ELF32_R_TYPE(rel_entry->r_info), rel_entry->r_offset);
                }
            }
        } else if (shdr->sh_type == SHT_RELA) {
            Elf32_Rela *rela_entry;

            for (i = 0, rela_entry = (Elf32_Rela *)(content + shdr->sh_offset);
                 i < shdr->sh_size / shdr->sh_entsize;
                 i++,   rela_entry = (Elf32_Rela *)((char *)rela_entry + shdr->sh_entsize)) {

                sym_index = ELF32_R_SYM(rela_entry->r_info);
                if (STN_UNDEF != sym_index) {
                    sym = (Elf32_Sym *)(symtab + sym_index * symtab_shdr->sh_entsize);
                    printf("sym name=%s, type=%d, offset=%d\n",  sym->st_name + str, ELF32_R_TYPE(rela_entry->r_info), rela_entry->r_offset);
                }
            }
        }
    }

    return 0;

    error1:
    free(shdrs);

    error0:
    return -1;
}

/*
 * 加载 elf 文件
 */
int load_elf(const char *path)
{
    int fd;
    int ret;
    unsigned char *buf, *content;
    struct stat st;
    int len;

    ret = access(path, R_OK);
    if (ret < 0) {
        printf("%s: failed to access %s\n", __func__, path);
        return -1;
    }

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("%s: failed to open %s\n", __func__, path);
        return -1;
    }

    ret = fstat(fd, &st);
    if (ret < 0 || st.st_size == 0) {
        printf("%s: failed to stat %s or %s is empty\n", __func__, path, path);
        return -1;
    }

    content = buf = malloc(st.st_size);
    if (buf < 0) {
        printf("%s: failed to malloc %d byte\n", __func__, st.st_size);
        close(fd);
        return -1;
    }

    len = 0;
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

    ret = probe_elf(content, st.st_size);
    if (ret < 0) {
        printf("%s: %s is not a elf file\n", __func__, path);
        free(content);
        close(fd);
        return -1;
    }

    ret = reloc_elf(content, st.st_size);
    if (ret < 0) {
        printf("%s: failed to parse %s\n", __func__, path);
        free(content);
        close(fd);
        return -1;
    }

    free(content);
    close(fd);

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

    return load_elf(argv[1]);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
