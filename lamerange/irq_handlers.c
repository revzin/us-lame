#include "stm32f0xx.h"
#include "bsp_clocks.h"
#include "bsp_tx.h"


void NMI_Handler(void)
{
	if (READ_BIT(RCC->CIR, RCC_CIR_CSSF))
		BSP_CSS_FailureHandler();
}

void TIM14_IRQHandler(void)
{
	BSP_TX_TIM14_IRQHandler();
}

void SysTick_Handler(void)
{
	BSP_TX_SysTickHandler();
}
