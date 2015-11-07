#include "stm32f0xx.h"
#include "bsp_clocks.h"

void NMI_Handler(void)
{
	if (READ_BIT(RCC->CIR, RCC_CIR_CSSF))
		BSP_CSS_FailureHandler();
}
