@echo off
setlocal enabledelayedexpansion

set num=0

del symbol.c

echo /********************************************************************************************************* >> symbol.c
echo ** 													>> symbol.c
echo **                                    �й������Դ��֯ 							>> symbol.c
echo **														>> symbol.c
echo **                                   Ƕ��ʽʵʱ����ϵͳ							>> symbol.c
echo **														>> symbol.c
echo **                                       SmileOS(TM)							>> symbol.c
echo **														>> symbol.c
echo **                               Copyright  All Rights Reserved						>> symbol.c
echo **														>> symbol.c
echo **--------------�ļ���Ϣ--------------------------------------------------------------------------------	>> symbol.c
echo **														>> symbol.c
echo ** ��   ��   ��: symbol.c											>> symbol.c
echo **														>> symbol.c
echo ** ��   ��   ��: MakeSymbol ����										>> symbol.c
echo **														>> symbol.c
echo ** �ļ���������: %date:~3,4% �� %date:~8,2% �� %date:~11,2% ��						>> symbol.c
echo **														>> symbol.c
echo ** ��        ��: SmileOS ���̷��ű�. (���ļ��� MakeSymbol �����Զ�����, �����޸�)			>> symbol.c
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