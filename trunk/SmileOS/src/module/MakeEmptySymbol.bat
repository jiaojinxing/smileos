@echo off
setlocal enabledelayedexpansion

set num=0

del sys_symbol.c

echo /********************************************************************************************************* >> sys_symbol.c
echo ** 													>> sys_symbol.c
echo **                                    �й������Դ��֯ 							>> sys_symbol.c
echo **														>> sys_symbol.c
echo **                                   Ƕ��ʽʵʱ����ϵͳ							>> sys_symbol.c
echo **														>> sys_symbol.c
echo **                                       SmileOS(TM)							>> sys_symbol.c
echo **														>> sys_symbol.c
echo **                               Copyright  All Rights Reserved						>> sys_symbol.c
echo **														>> sys_symbol.c
echo **--------------�ļ���Ϣ--------------------------------------------------------------------------------	>> sys_symbol.c
echo **														>> sys_symbol.c
echo ** ��   ��   ��: sys_symbol.c											>> sys_symbol.c
echo **														>> sys_symbol.c
echo ** ��   ��   ��: MakeSymbol ����										>> sys_symbol.c
echo **														>> sys_symbol.c
echo ** �ļ���������: %date:~3,4% �� %date:~8,2% �� %date:~11,2% ��						>> sys_symbol.c
echo **														>> sys_symbol.c
echo ** ��        ��: SmileOS ����ϵͳ���ű�. (���ļ��� MakeSymbol �����Զ�����, �����޸�)			>> sys_symbol.c
echo *********************************************************************************************************/	>> sys_symbol.c										>> sys_symbol.c
echo #include "kern/func_config.h"                                          >> sys_symbol.c
echo #if CONFIG_MODULE_EN > 0                                        >> sys_symbol.c
echo #include "module/symbol.h"										>> sys_symbol.c
echo.														>> sys_symbol.c
echo #define SYMBOL_TABLE_BEGIN const symbol_t sys_symbol_tbl[] = { 						>> sys_symbol.c
echo.  														>> sys_symbol.c
echo #define SYMBOL_TABLE_END   {0, 0, 0} };									>> sys_symbol.c

echo SYMBOL_TABLE_BEGIN												>> sys_symbol.c

echo SYMBOL_TABLE_END												>> sys_symbol.c
echo #endif                                               >> sys_symbol.c
echo /*********************************************************************************************************	>> sys_symbol.c
echo ** END FILE													>> sys_symbol.c
echo *********************************************************************************************************/	>> sys_symbol.c

del func.txt
del obj.txt
@echo on