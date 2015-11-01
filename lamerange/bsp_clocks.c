#include <string.h>

#include "stm32f0xx.h"

#include "bsp_clocks.h"
#include "misc.h" // в Cortex-M0 нет CLZ и RBIT, здесь их софтовая эмуляция (для POSITION_VAL)


#define HSE_VALUE 26000000
#define HSI_VALUE 8000000
#define HSI48_VALUE 48000000
#define HSI14_VALUE 14000000


void BSP_Clock_Setup32MHz(void);


void BSP_Clock_GetFreqHz(BRD_ClockFreqs *bcf)
{
	memset(bcf, 0, sizeof(BRD_ClockFreqs));
	
	int sysclk_source = READ_BIT(RCC->CFGR, RCC_CFGR_SWS) >> POSITION_VAL(RCC_CFGR_SWS);	
	
	/* табличка делителей APB (биты RCC_CFGR_PPRE) 	*/ 
	/* 										000 	001 	010 	011 	100 	101		110		111 */
	static const int apb_div_lookup[] = {	1,		1,		1,		1,		2,		4,		8,		16};
	
	/* табличка делителей AHB (биты RCC_CFGR_HPRE) 	*/ 
	static const int ahb_div_lookup[] = {
		/* 0000 */ 1,
		/* 0001 */ 1,
		/* 0010 */ 1,
		/* 0011 */ 1,
		/* 0100 */ 1,
		/* 0101 */ 1,
		/* 0110 */ 1,
		/* 0111 */ 1,
		/* 1000 */ 2,
		/* 1001 */ 4,
		/* 1010 */ 8,
		/* 1011 */ 16,
		/* 1100 */ 64,
		/* 1101 */ 128,
		/* 1110 */ 256,
		/* 1111 */ 512
	};
		
	switch (sysclk_source)
	{
		case 0x00: /* HSI */
		{
			bcf->sysclk = HSI_VALUE;
			break;
		}
		case 0x01: /* HSE */
		{
			bcf->sysclk = HSE_VALUE;
			break;
		}
		case 0x02: /* PLL */
		{
			/* RM0091 стр. 118 */
			int pll_source = READ_BIT(RCC->CFGR, RCC_CFGR_PLLSRC) >> POSITION_VAL(RCC_CFGR_PLLSRC);
			int prediv = (READ_BIT(RCC->CFGR2, RCC_CFGR2_PREDIV) >> POSITION_VAL(RCC_CFGR2_PREDIV)) + 1;
			int pllmult = (READ_BIT(RCC->CFGR, RCC_CFGR_PLLMUL) >> POSITION_VAL(RCC_CFGR_PLLMUL)) + 2; 
			if (pllmult >= 16) 
				pllmult = 16;
			
			switch (pll_source)
			{
				case 0x00: /* HSI / 2 */
				{
					bcf->sysclk = HSI_VALUE / 2 * pllmult;
					break;
				}
				case 0x01: /* HSI / prediv */
				{
					bcf->sysclk = HSI_VALUE / prediv * pllmult;
					break;
				}
				case 0x02: /* HSE / prediv */
				{
					bcf->sysclk = HSE_VALUE / prediv * pllmult;
					break;
				}
				case 0x03: /* HSI48 / prediv */
				{
					bcf->sysclk = HSI48_VALUE / prediv * pllmult;
					break;
				}
			}
		}
		case 0x03: /* HSI48 */
		{
			bcf->sysclk = HSI48_VALUE;
			break;
		}
	}
	
	int ahb_prescaler  = ahb_div_lookup[READ_BIT(RCC->CFGR, RCC_CFGR_HPRE) >> POSITION_VAL(RCC_CFGR_HPRE)];
	int apb_prescaler = apb_div_lookup[READ_BIT(RCC->CFGR, RCC_CFGR_PPRE) >> POSITION_VAL(RCC_CFGR_PPRE)];
	
	bcf->hlck = bcf->sysclk / ahb_prescaler;
	bcf->apb1 = bcf->sysclk / apb_prescaler;
	bcf->apb1_tim = bcf->apb1;
	bcf->systim = bcf->hlck / 8;
	bcf->fclk = bcf->hlck;
}




