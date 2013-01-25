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

set srcfile=../runtime/libgcc.a

nm %srcfile% | find " T "    >> func.txt
nm %srcfile% | find " D "    >> obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

set srcfile=../runtime/libm.a

nm %srcfile% | find " T "    >> func.txt
nm %srcfile% | find " D "    >> obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

set srcfile=../runtime/libc.a

nm %srcfile% | find " T "    >> func.txt
nm %srcfile% | find " D "    >> obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

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
echo ** ��        ��: SmileOS ����ϵͳ���ű�. (���ļ��� MakeSymbol �����Զ�����, �����޸�)			>> symbol.c
echo *********************************************************************************************************/	>> symbol.c										>> symbol.c
echo #include "module/symbol_tool.h"										>> symbol.c
echo.														>> symbol.c
echo #define SYMBOL_TABLE_BEGIN SYMBOL smileos_symbol_table[] = { 						>> symbol.c
echo.  														>> symbol.c
echo #define SYMBOL_TABLE_END   {0, 0, 0} };									>> symbol.c
echo.														>> symbol.c
echo #define SYMBOL_ITEM_FUNC(pcName)                \>> symbol.c
echo     {   #pcName, (char *)pcName,                \>> symbol.c
echo         SYMBOL_TEXT                             \>> symbol.c
echo     },													>> symbol.c
echo.														>> symbol.c
echo #define SYMBOL_ITEM_OBJ(pcName)                 \>> symbol.c
echo     {   #pcName, (char *)^&pcName,               \>> symbol.c
echo         SYMBOL_DATA                             \>> symbol.c
echo     },													>> symbol.c
echo.														>> symbol.c
echo /*********************************************************************************************************	>> symbol.c
echo ** ȫ�ֶ�������												>> symbol.c
echo *********************************************************************************************************/	>> symbol.c

for /f "tokens=3 delims= " %%i in (func.txt) do @(
        echo extern int  %%i^(^); >> symbol.c
        set /a num+=1
)

for /f "tokens=3 delims= " %%i in (obj.txt) do @(
        echo extern int  %%i; >> symbol.c
        set /a num+=1
)

echo.
echo /*********************************************************************************************************	>> symbol.c
echo ** ϵͳ��̬���ű�												>> symbol.c
echo *********************************************************************************************************/	>> symbol.c
echo SYMBOL_TABLE_BEGIN												>> symbol.c

for /f "tokens=3 delims= " %%i in (func.txt) do @(
        echo     SYMBOL_ITEM_FUNC^(%%i^) >> symbol.c
)

for /f "tokens=3 delims= " %%i in (obj.txt) do @(
        echo     SYMBOL_ITEM_OBJ^(%%i^) >> symbol.c
)
echo SYMBOL_TABLE_END												>> symbol.c
echo /*********************************************************************************************************	>> symbol.c
echo ** END FILE													>> symbol.c
echo *********************************************************************************************************/	>> symbol.c

del func.txt
del obj.txt
@echo on