#include "test_sandbox.h"
#include "bsp_power.h"
#include "bsp_syscfg.h"

void SBX_main(void)
{
	BSP_RemapPA11_PA12();
	BSP_PWR_HV_Setup();
	BSP_PWR_HV_Enable();
	BSP_PWR_5VA_Setup();
	BSP_PWR_5VA_Enable();
	for (;;) {}
}
