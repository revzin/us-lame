#include "stm32f0xx.h"

#include "bsp_tx.h"
#include "bsp_clocks.h"
#include "ownassert.h"

/* TODO всё это можно переделать так, чтобы TIM16 сам дёргал HI-LO, но позже */

#define READHILO (READ_BIT(GPIOF->ODR, GPIO_ODR_8))

void lowside(void)
{
	SET_BIT(GPIOB->BSRRH, GPIO_BSRR_BR_8);
}

void highside(void)
{
	SET_BIT(GPIOB->BSRRL, GPIO_BSRR_BS_8);
}

void toggle_tx_switch(void)
{
	if (READHILO)
		lowside();
	else
		highside();
}
	
void BSP_TX_Setup(void)
{
	BSP_Clock_Freqs bcf;
	BSP_Clock_GetFreqHz(&bcf);
	assert(bcf.apb_tim == 32000000u, "TX setup: wrong clock frequency");
	
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
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOBEN);
	/* ставим в 0 */
	SET_BIT(GPIOB->BSRRH, GPIO_BSRR_BR_8);
	/* режим вывода */
	SET_BIT(GPIOB->MODER, GPIO_MODER_MODER8_0);
	CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER8_1);
	/* тянитолкай */
	CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_8);
	
	
	/* ----------------------- TIM14 --------------------------- */
	/* TIM14 готовим к генерации 80 кГц, чтобы переключать HI-LO */
	/* что даст период переключения 40 кГц */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);
	/* Делим 32 МГц на 32000 и получаем 1 кГц */
	WRITE_REG(TIM14->PSC, 32000 - 1); /* prescaler = PSC + 1 */
	/* Заставляем считать до 80 */
	WRITE_REG(TIM14->ARR, 80);
	/* включаем прерывание */
	WRITE_REG(TIM14->DIER, TIM_DIER_UIE);
	NVIC_EnableIRQ(TIM14_IRQn);
	
	/* в отладке не считать! */
	SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_TIM14_STOP);
	
}

void BSP_TX_Enable(void)
{
	SET_BIT(GPIOF->BSRRL, GPIO_BSRR_BS_1);
}

void BSP_TX_Disable(void)
{
	SET_BIT(GPIOF->BSRRH, GPIO_BSRR_BR_1);
}

void BSP_TX_StartPulse(void)
{
	SET_BIT(TIM14->CR1, TIM_CR1_CEN);
}

void BSP_TX_StopPulse(void)
{
	CLEAR_BIT(TIM14->CR1, TIM_CR1_CEN);
}

void BSP_TX_Send10msPulse(void);

void BSP_TX_TIM14_IRQHandler(void)
{
	toggle_tx_switch();
	/* очищаем запрос на прерывание */
	CLEAR_BIT(TIM14->SR, TIM_SR_UIF);
}
