#include "stm32f0xx.h"

void BSP_RemapPA11_PA12(void)
{
	SET_BIT(SYSCFG->CFGR1, SYSCFG_CFGR1_PA11_PA12_RMP);
}
