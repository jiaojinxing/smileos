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
** File name:               module.h
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
*********************************************************************************************************/
#ifndef MODULE_H_
#define MODULE_H_

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************************************
** ģ��
*********************************************************************************************************/
struct module;
typedef struct module module_t;
/*********************************************************************************************************
** Function name:           module_load
** Descriptions:            �����ں�ģ��
** input parameters:        path                �ں�ģ���ļ�·��
**                          argc                ��������
**                          argv                ��������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_load(const char *path, int argc, char **argv);
/*********************************************************************************************************
** Function name:           module_unload
** Descriptions:            ж���ں�ģ��
** input parameters:        path                �ں�ģ���ļ�·��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unload(const char *path);
/*********************************************************************************************************
** Function name:           module_unref
** Descriptions:            ��ȡ���ں�ģ�������
** input parameters:        mod                 �ں�ģ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unref(module_t *mod);
/*********************************************************************************************************
** Function name:           module_ref_by_addr
** Descriptions:            ͨ���ں�ģ���һ����ַ�����ں�ģ��
** input parameters:        addr                ��ַ
** output parameters:       NONE
** Returned value:          �ں�ģ�� OR NULL
*********************************************************************************************************/
module_t *module_ref_by_addr(void *addr);
/*********************************************************************************************************
** Function name:           module_open
** Descriptions:            ��Ӧ��ģ��
** input parameters:        path                Ӧ��ģ���ļ�·��
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          ģ��
*********************************************************************************************************/
module_t *module_open(const char *path, int mode);
/*********************************************************************************************************
** Function name:           module_close
** Descriptions:            �ر�Ӧ��ģ��
** input parameters:        mod                 Ӧ��ģ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_close(module_t *mod);
/*********************************************************************************************************
** Function name:           module_symbol
** Descriptions:            ��ģ������ҷ���
** input parameters:        mod                 ģ��
**                          name                ������
** output parameters:       NONE
** Returned value:          ���ŵ�ַ
*********************************************************************************************************/
void *module_symbol(module_t *mod, const char *name);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  MODULE_H_                   */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
