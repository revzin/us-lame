#ifndef __BSP__CLOCKS__H__
#define __BSP__CLOCKS__H__

typedef struct
{
	int hlck,
		systim,
		fclk,
		apb,
		apb_tim,
		usbck,
		sysclk;
} BRD_ClockFreqs;

/* настраивает RCC для тактирования SYSCLK на 32 МГц */
void BSP_Clock_Setup32MHz(void);

/* записывает в bcf частоты тактирования шин */
void BSP_Clock_GetFreqHz(BRD_ClockFreqs *bcf);

#endif
