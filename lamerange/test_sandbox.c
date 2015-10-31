#include "test_sandbox.h"
#include "bsp_power.h"

void SBX_main(void)
{
	BSP_PWR_HV_Setup();
	BSP_PWR_HV_Enable();
	BSP_PWR_5VA_Setup();
	BSP_PWR_5VA_Enable();
	for (;;) {}
}
