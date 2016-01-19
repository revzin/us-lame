#include "test_sandbox.h"
#include "bsp_power.h"
#include "bsp_syscfg.h"
#include "bsp_tx.h"
#include "bsp_clocks.h"

void SBX_main(void)
{
	BSP_Clock_Freqs bcf;
	BSP_Clock_GetFreqHz(&bcf);
	BSP_Clock_Setup32MHz();
	BSP_Clock_GetFreqHz(&bcf);
	
	BSP_RemapPA11_PA12();
	BSP_PWR_HV_Setup();
	BSP_PWR_HV_Enable();
	BSP_PWR_5VA_Setup();
	BSP_PWR_5VA_Enable();
	
	BSP_TX_Setup();
	//BSP_TX_Enable();
	//BSP_TX_StartPulse();
	
	BSP_TX_StartTxSequence();
	
	for (;;) {}

	
		
	BSP_TX_Disable();
}
