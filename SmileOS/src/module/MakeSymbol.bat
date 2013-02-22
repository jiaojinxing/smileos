@echo off
setlocal enabledelayedexpansion

set srcfile=../../Debug/SmileOS.elf

nm %srcfile% | find " T "    >  func.txt
nm %srcfile% | find " D "    >  obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

set srcfile=../runtime/arch/arm920t/libgcc.a

nm %srcfile% | find " T "    >> func.txt
nm %srcfile% | find " D "    >> obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

set srcfile=../runtime/arch/arm920t/libm.a

nm %srcfile% | find " T "    >> func.txt
nm %srcfile% | find " D "    >> obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

set srcfile=../runtime/arch/arm920t/libc.a

nm %srcfile% | find " T "    >> func.txt
nm %srcfile% | find " D "    >> obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

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
echo.														>> sys_symbol.c
echo #define SYMBOL_ITEM_FUNC(pcName)                \>> sys_symbol.c
echo     {   #pcName, (void *)pcName,                \>> sys_symbol.c
echo         SYMBOL_TEXT                             \>> sys_symbol.c
echo     },													>> sys_symbol.c
echo.														>> sys_symbol.c
echo #define SYMBOL_ITEM_OBJ(pcName)                 \>> sys_symbol.c
echo     {   #pcName, (void *)^&pcName,               \>> sys_symbol.c
echo         SYMBOL_DATA                             \>> sys_symbol.c
echo     },													>> sys_symbol.c
echo.														>> sys_symbol.c
echo /*********************************************************************************************************	>> sys_symbol.c
echo ** ȫ�ֶ�������												>> sys_symbol.c
echo *********************************************************************************************************/	>> sys_symbol.c

for /f "tokens=3 delims= " %%i in (func.txt) do @(
        echo extern int  %%i^(^); >> sys_symbol.c
        set /a num+=1
)

for /f "tokens=3 delims= " %%i in (obj.txt) do @(
        echo extern int  %%i; >> sys_symbol.c
        set /a num+=1
)

echo.
echo /*********************************************************************************************************	>> sys_symbol.c
echo ** ϵͳ��̬���ű�												>> sys_symbol.c
echo *********************************************************************************************************/	>> sys_symbol.c
echo SYMBOL_TABLE_BEGIN												>> sys_symbol.c

for /f "tokens=3 delims= " %%i in (func.txt) do @(
        echo     SYMBOL_ITEM_FUNC^(%%i^) >> sys_symbol.c
)

for /f "tokens=3 delims= " %%i in (obj.txt) do @(
        echo     SYMBOL_ITEM_OBJ^(%%i^) >> sys_symbol.c
)
echo SYMBOL_TABLE_END												>> sys_symbol.c
echo #endif                                               >> sys_symbol.c
echo /*********************************************************************************************************	>> sys_symbol.c
echo ** END FILE													>> sys_symbol.c
echo *********************************************************************************************************/	>> sys_symbol.c

del func.txt
del obj.txt
@echo on