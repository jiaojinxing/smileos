/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <jiaojinxing1987@gmail.com>
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
** File name:               crt0.c
** Last modified Date:      2013-3-13
** Last Version:            1.0.0
** Descriptions:            进入点
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-3-13
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

#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

// Code Red - if CMSIS is being used, then SystemInit() routine
// will be called by startup code rather than in application's main()
#ifdef __USE_CMSIS
#include "system_LPC13xx.h"
#endif

//*****************************************************************************
//
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will 
// automatically take precedence over these weak definitions
//
//*****************************************************************************
     void Reset_Handler(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void SVCall_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);

//*****************************************************************************
//
// Forward declaration of the specific IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take 
// precedence over these weak definitions
//
//*****************************************************************************
void I2C_IRQHandler (void) ALIAS(IntDefaultHandler);
void WWDG_IRQHandler (void) ALIAS(IntDefaultHandler);
void PVD_IRQHandler (void) ALIAS(IntDefaultHandler);
void TAMPER_IRQHandler (void) ALIAS(IntDefaultHandler);
void RTC_IRQHandler (void) ALIAS(IntDefaultHandler);
void FLASH_IRQHandler (void) ALIAS(IntDefaultHandler);
void RCC_IRQHandler (void) ALIAS(IntDefaultHandler);
void EXTI0_IRQHandler (void) ALIAS(IntDefaultHandler);
void EXTI1_IRQHandler (void) ALIAS(IntDefaultHandler);
void EXTI2_IRQHandler (void) ALIAS(IntDefaultHandler);
void EXTI3_IRQHandler (void) ALIAS(IntDefaultHandler);
void EXTI4_IRQHandler (void) ALIAS(IntDefaultHandler);
void DMAChannel1_IRQHandler (void) ALIAS(IntDefaultHandler);
void DMAChannel2_IRQHandler (void) ALIAS(IntDefaultHandler);
void DMAChannel3_IRQHandler (void) ALIAS(IntDefaultHandler);
void DMAChannel4_IRQHandler (void) ALIAS(IntDefaultHandler);
void DMAChannel5_IRQHandler (void) ALIAS(IntDefaultHandler);
void DMAChannel6_IRQHandler (void) ALIAS(IntDefaultHandler);
void DMAChannel7_IRQHandler (void) ALIAS(IntDefaultHandler);
void ADC_IRQHandler (void) ALIAS(IntDefaultHandler);
void USB_HP_CAN_TX_IRQHandler (void) ALIAS(IntDefaultHandler);
void USB_LP_CAN_RX0_IRQHandler (void) ALIAS(IntDefaultHandler);
void CAN_RX1_IRQHandler (void) ALIAS(IntDefaultHandler);
void CAN_SCE_IRQHandler (void) ALIAS(IntDefaultHandler);
void EXTI9_5_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIM1_BRK_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIM1_UP_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIM1_TRG_COM_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIM1_CC_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIM2_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIM3_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIM4_IRQHandler (void) ALIAS(IntDefaultHandler);
void I2C1_EV_IRQHandler (void) ALIAS(IntDefaultHandler);
void I2C1_ER_IRQHandler (void) ALIAS(IntDefaultHandler);
void I2C2_EV_IRQHandler (void) ALIAS(IntDefaultHandler);
void I2C2_ER_IRQHandler (void) ALIAS(IntDefaultHandler);
void SPI1_IRQHandler (void) ALIAS(IntDefaultHandler);
void SPI2_IRQHandler (void) ALIAS(IntDefaultHandler);
void USART1_IRQHandler (void) ALIAS(IntDefaultHandler);
void USART2_IRQHandler (void) ALIAS(IntDefaultHandler);
void USART3_IRQHandler (void) ALIAS(IntDefaultHandler);
void EXTI15_10_IRQHandler (void) ALIAS(IntDefaultHandler);
void RTCAlarm_IRQHandler (void) ALIAS(IntDefaultHandler);
void USBWakeUp_IRQHandler (void) ALIAS(IntDefaultHandler);

//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern WEAK void __libc_init_array(void);

//*****************************************************************************
//
// The entry point for the application.
// __main() is the entry point for redlib based applications
// main() is the entry point for newlib based applications
//
//*****************************************************************************
extern WEAK void __main(void);
extern WEAK void main(void);
//*****************************************************************************
//
// External declaration for the pointer to the stack top from the Linker Script
//
//*****************************************************************************
extern void _vStackTop;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
	// Core Level - CM3
	(void *)&_vStackTop,					// The initial stack pointer
	Reset_Handler,							// The reset handler
	NMI_Handler,							// The NMI handler
	HardFault_Handler,						// The hard fault handler
	MemManage_Handler,						// The MPU fault handler
	BusFault_Handler,						// The bus fault handler
	UsageFault_Handler,						// The usage fault handler
	0,										// Reserved
	0,										// Reserved
	0,										// Reserved
	0,										// Reserved
	SVCall_Handler,							// SVCall handler
	DebugMon_Handler,						// Debug monitor handler
	0,										// Reserved
	PendSV_Handler,							// The PendSV handler
	SysTick_Handler,						// The SysTick handler

    WWDG_IRQHandler,
    PVD_IRQHandler,
    TAMPER_IRQHandler,
    RTC_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMAChannel1_IRQHandler,
    DMAChannel2_IRQHandler,
    DMAChannel3_IRQHandler,
    DMAChannel4_IRQHandler,
    DMAChannel5_IRQHandler,
    DMAChannel6_IRQHandler,
    DMAChannel7_IRQHandler,
    ADC_IRQHandler,
    USB_HP_CAN_TX_IRQHandler,
    USB_LP_CAN_RX0_IRQHandler,
    CAN_RX1_IRQHandler,
    CAN_SCE_IRQHandler,
    EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler,
    TIM1_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM4_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_IRQHandler,
    EXTI15_10_IRQHandler,
    RTCAlarm_IRQHandler,
    USBWakeUp_IRQHandler,
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
Reset_Handler(void)
{
    unsigned long *pulSrc, *pulDest;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for (pulDest = &_data; pulDest < &_edata; ) {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.  This is done with inline assembly since this
    // will clear the value of pulDest if it is not kept in a register.
    //
    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");

    //
    // Call C++ library initialization, if present
    //
	if (__libc_init_array)
		__libc_init_array();

#ifdef __USE_CMSIS
	SystemInit();
#endif

	//
	// Call the application's entry point.
	// __main() is the entry point for redlib based applications (which calls main())
	// main() is the entry point for newlib based applications
	//
	if (__main)
		__main();
	else
		main();

	//
	// main() shouldn't return, but if it does, we'll just enter an infinite loop 
	//
	while (1) {
		;
	}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
void NMI_Handler(void)
{
    while (1) {
    }
}

void HardFault_Handler(void)
{
    while (1) {
    }
}

void MemManage_Handler(void)
{
    while (1) {
    }
}

void BusFault_Handler(void)
{
    while (1) {
    }
}

void UsageFault_Handler(void)
{
    while (1) {
    }
}

void SVCall_Handler(void)
{
    while (1) {
    }
}

void DebugMon_Handler(void)
{
    while (1) {
    }
}

void PendSV_Handler(void)
{
    while (1) {
    }
}

void SysTick_Handler(void) 
{
    while (1) {
    }
}

//*****************************************************************************
//
// Processor ends up here if an unexpected interrupt occurs or a handler
// is not present in the application code.
//
//*****************************************************************************
static void IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1) {
    }
}
