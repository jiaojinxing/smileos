@echo off
setlocal enabledelayedexpansion

set srcfile=libruntime.a

nm %srcfile% | find " T " > func.txt
nm %srcfile% | find " D " > obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

set srcfile=libc.a

nm %srcfile% | find " T "    >> func.txt
nm %srcfile% | find " D "    >> obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

set srcfile=libm.a

nm %srcfile% | find " T "    >> func.txt
nm %srcfile% | find " D "    >> obj.txt
nm %srcfile% | find " B "    >> obj.txt
nm %srcfile% | find " R "    >> obj.txt
nm %srcfile% | find " S "    >> obj.txt
nm %srcfile% | find " C "    >> obj.txt
nm %srcfile% | find " W "    >> obj.txt
nm %srcfile% | find " V "    >> obj.txt

set srcfile=libgcc.a

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
del symbol.h

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
echo ** �ļ���������: %date:~0,4% �� %date:~5,2% �� %date:~8,2% ��						>> symbol.c
echo **														>> symbol.c
echo ** ��        ��: SmileOS ����ϵͳ���ű�. (���ļ��� MakeSymbol �����Զ�����, �����޸�)			>> symbol.c
echo *********************************************************************************************************/	>> symbol.c
echo #ifdef SMILEOS_SYMBOL_TOOL											>> symbol.c
echo #include "symbol_tool.h"											>> symbol.c
echo.														>> symbol.c
echo #define SYMBOL_TABLE_BEGIN SYMBOL smileos_symbol_table[] = { 						>> symbol.c
echo.  														>> symbol.c
echo #define SYMBOL_TABLE_END };										>> symbol.c
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
echo   ȫ�ֶ�������												>> symbol.c
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
echo   ϵͳ��̬���ű�												>> symbol.c
echo *********************************************************************************************************/	>> symbol.c
echo SYMBOL_TABLE_BEGIN												>> symbol.c

for /f "tokens=3 delims= " %%i in (func.txt) do @(
        echo     SYMBOL_ITEM_FUNC^(%%i^) >> symbol.c
)

for /f "tokens=3 delims= " %%i in (obj.txt) do @(
        echo     SYMBOL_ITEM_OBJ^(%%i^) >> symbol.c
)
echo SYMBOL_TABLE_END												>> symbol.c
echo #endif													>> symbol.c
echo /*********************************************************************************************************	>> symbol.c
echo   END													>> symbol.c
echo *********************************************************************************************************/	>> symbol.c


echo /*********************************************************************************************************	>> symbol.h
echo **														>> symbol.h
echo **                                    �й������Դ��֯							>> symbol.h
echo **														>> symbol.h
echo **                                   Ƕ��ʽʵʱ����ϵͳ							>> symbol.h
echo **														>> symbol.h
echo **                                       SmileOS(TM)							>> symbol.h
echo **														>> symbol.h
echo **                               Copyright  All Rights Reserved						>> symbol.h
echo **														>> symbol.h
echo **--------------�ļ���Ϣ--------------------------------------------------------------------------------	>> symbol.h
echo **														>> symbol.h
echo ** ��   ��   ��: symbol.h											>> symbol.h
echo **														>> symbol.h
echo ** ��   ��   ��: MakeSymbol ����										>> symbol.h
echo **														>> symbol.h
echo ** �ļ���������: %date:~0,4% �� %date:~5,2% �� %date:~8,2% ��						>> symbol.h
echo **														>> symbol.h
echo ** ��        ��: SmileOS ����ϵͳ���ű�. (���ļ��� MakeSymbol �����Զ�����, �����޸�)			>> symbol.h
echo *********************************************************************************************************/	>> symbol.h
echo #ifndef __SYMBOL_H												>> symbol.h
echo #define __SYMBOL_H												>> symbol.h
echo.														>> symbol.h
echo #include "symbol_tool.h"											>> symbol.h
echo.														>> symbol.h
echo #define SYM_TABLE_SIZE %num%										>> symbol.h
echo extern SYMBOL smileos_symbol_table[SYM_TABLE_SIZE];							>> symbol.h
echo.														>> symbol.h	
echo #endif                                                                  /*  __SYMBOL_H                  */	>> symbol.h
echo /*********************************************************************************************************	>> symbol.h
echo   END													>> symbol.h
echo *********************************************************************************************************/	>> symbol.h

del func.txt
del obj.txt
@echo on