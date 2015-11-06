#include "stm32f0xx.h"

#include "bsp_tx.h"


#define READHILO (READ_BIT(GPIOF->ODR, GPIO_ODR_8))

void toggle_tx_switch(void)
{
	if (READHILO)
		SET_BIT(GPIOF->BSRRH, GPIO_BSRR_BR_8);
	else
		SET_BIT(GPIOF->BSRRL, GPIO_BSRR_BS_8);
}
	

void lowside(void)
{
	SET_BIT(GPIOB->BSRRH, GPIO_BSRR_BR_8);
}

void highside(void)
{
	SET_BIT(GPIOB->BSRRL, GPIO_BSRR_BS_8);
}

void BSP_TX_Setup(void)
{
	/* PB8 = HI-LO, PB8 = 1 -> верхний ключ, PB8 = 0 -> нижний ключ */
	/* PF1 = nRXEN; nRXEN = 1 -> режим передачи, nRXEN = 0 -> режим приёма */
	
	/* --------------------- готовим PF1 ----------------------- */
	/* тактирование */
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOFEN);
	/* ставим nRXEN в 0 */
	SET_BIT(GPIOF->BSRRH, GPIO_BSRR_BR_1);
	/* режим вывода */
	SET_BIT(GPIOF->MODER, GPIO_MODER_MODER1_0);
	CLEAR_BIT(GPIOF->MODER, GPIO_MODER_MODER1_1);
	/* тянитолкай */
	CLEAR_BIT(GPIOF->OTYPER, GPIO_OTYPER_OT_1);
	/* по умолчанию включен приём (NRXEN = PF1 = 0) */
	SET_BIT(GPIOF->BSRRH, GPIO_BSRR_BR_1);
	
	
	/* --------------------- готовим PB8 ----------------------- */
	/* тактирование */
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOFEN);
	/* ставим в 0 */
	SET_BIT(GPIOB->BSRRH, GPIO_BSRR_BR_8);
	/* режим вывода */
	SET_BIT(GPIOB->MODER, GPIO_MODER_MODER8_0);
	CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER8_1);
	/* тянитолкай */
	CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_8);
	
	
}

void BSP_TX_Enable(void)
{
	SET_BIT(GPIOF->BSRRL, GPIO_BSRR_BS_1);
	lowside();
	highside();
}

void BSP_TX_Disable(void);

void BSP_TX_StartPulse(void);

void BSP_TX_StopPulse(void);

void BSP_TX_Send10msPulse(void);

void BSP_TX_TIM5_IRQHandler(void);
