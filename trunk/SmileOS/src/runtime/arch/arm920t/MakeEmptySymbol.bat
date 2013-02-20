@echo off
setlocal enabledelayedexpansion

set num=0

del symbol.c

echo /********************************************************************************************************* >> symbol.c
echo ** 													>> symbol.c
echo **                                    中国软件开源组织 							>> symbol.c
echo **														>> symbol.c
echo **                                   嵌入式实时操作系统							>> symbol.c
echo **														>> symbol.c
echo **                                       SmileOS(TM)							>> symbol.c
echo **														>> symbol.c
echo **                               Copyright  All Rights Reserved						>> symbol.c
echo **														>> symbol.c
echo **--------------文件信息--------------------------------------------------------------------------------	>> symbol.c
echo **														>> symbol.c
echo ** 文   件   名: symbol.c											>> symbol.c
echo **														>> symbol.c
echo ** 创   建   人: MakeSymbol 工具										>> symbol.c
echo **														>> symbol.c
echo ** 文件创建日期: %date:~3,4% 年 %date:~8,2% 月 %date:~11,2% 日						>> symbol.c
echo **														>> symbol.c
echo ** 描        述: SmileOS 进程符号表. (此文件由 MakeSymbol 工具自动生成, 请勿修改)			>> symbol.c
echo *********************************************************************************************************/	>> symbol.c										>> symbol.c
echo #include "kern/func_config.h"                                          >> symbol.c
echo #if CONFIG_MODULE_EN > 0                                        >> symbol.c
echo #include "module/symbol_tool.h"										>> symbol.c
echo.														>> symbol.c
echo #define SYMBOL_TABLE_BEGIN const symbol_t symbol_tbl[] = { 						>> symbol.c
echo.  														>> symbol.c
echo #define SYMBOL_TABLE_END   {0, 0, 0} };									>> symbol.c

echo SYMBOL_TABLE_BEGIN												>> symbol.c

echo SYMBOL_TABLE_END												>> symbol.c
echo #endif                                               >> symbol.c
echo /*********************************************************************************************************	>> symbol.c
echo ** END FILE													>> symbol.c
echo *********************************************************************************************************/	>> symbol.c

del func.txt
del obj.txt
@echo on