#include "stm32f0xx.h"

/* высокое напряжение */
/* PA7 = HVEN */
/* PB1 = HV_PGOOD_ADC */
void BSP_PWR_HV_Setup(void)
{
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN); /* тактирование */
	
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER7_0);
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER7_1); /* General Purpose Output */
	
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_7); /* Push-Pull */
	SET_BIT(GPIOA->BSRRL, GPIO_BSRR_BR_7); /* ставим в гарантированный 0 */
	
	/* фичи PB1 реализуем позже */
}

void BSP_PWR_HV_Enable(void)
{
	SET_BIT(GPIOA->BSRRL, GPIO_BSRR_BS_7); /* HV_ENABLE = 1, ВН включено */
}

void BSP_PWR_HV_Disable(void)
{
	SET_BIT(GPIOA->BSRRL, GPIO_BSRR_BR_7); /* HV_ENABLE = 0, ВН выключено */
}

int BSP_PWR_CheckHVVoltage(void);


/* 5 В */

void BSP_PWR_5VA_Setup(void);

void BSP_PWR_5VA_Enable(void);

void BSP_PWR_5VA_Disable(void);

int BSP_PWR_Check5VAVoltage(void);
