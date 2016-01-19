#include "stm32f0xx.h"

#include "bsp_tx.h"
#include "bsp_clocks.h"
#include "ownassert.h"

#define RELAY_CTL_FIX
/* в плате V1_0 ошибка полярности сигнала, управляющего реле (nRXEN), поэтому он прокинут на PA5 */
/* в плате V1_1 это исправлено и дефайн нужно убрать */

/* TODO всё это можно переделать так, чтобы TIM16 сам дёргал HI-LO, но позже */

#define READHILO (READ_BIT(GPIOB->ODR, GPIO_ODR_8))

void lowside(void)
{
	CLEAR_BIT(GPIOB->ODR, GPIO_ODR_8); // wtf
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
	/* высокая скорость переключения */
	SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEEDR8);
	
#ifdef RELAY_CTL_FIX
	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN);
	CLEAR_BIT(GPIOA->ODR, GPIO_ODR_5);
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER5_0);
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER5_1);
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_5);
	/* высокая скорость переключения */
	SET_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEEDR5);
#endif
	
	
	/* ----------------------- TIM14 --------------------------- */
	/* TIM14 готовим к генерации 80 кГц, чтобы переключать HI-LO */
	/* что даст период переключения HI-LO в 40 кГц */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);
	/* Делим 32 МГц на 200 и получаем 160 кГц */
	WRITE_REG(TIM14->PSC, 200 - 1); /* prescaler = PSC + 1 */
	/* Заставляем считать до 1 - частота делится на 2 = 80 кГц*/
	WRITE_REG(TIM14->ARR, 1);
	/* включаем прерывание */
	WRITE_REG(TIM14->DIER, TIM_DIER_UIE);


	/* в отладке не считать! */
	SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_TIM14_STOP);
	
	NVIC_SetPriority(TIM14_IRQn, 10);
}

void BSP_TX_Enable(void)
{
#ifdef RELAY_CTL_FIX
	CLEAR_BIT(GPIOA->ODR, GPIO_ODR_5);
#endif
	
	SET_BIT(GPIOF->BSRRL, GPIO_BSRR_BS_1);
}

void BSP_TX_Disable(void)
{
	lowside();
	CLEAR_BIT(GPIOF->ODR, GPIO_ODR_1);
	BSP_TX_StopPulse();
	
#ifdef RELAY_CTL_FIX
	SET_BIT(GPIOA->ODR, GPIO_ODR_5);
#endif
}

void BSP_TX_StartPulse(void)
{
	TIM14->CNT = 0;
	NVIC_EnableIRQ(TIM14_IRQn);
	SET_BIT(TIM14->CR1, TIM_CR1_CEN);
}

void BSP_TX_StopPulse(void)
{
	NVIC_ClearPendingIRQ(TIM14_IRQn);
	NVIC_DisableIRQ(TIM14_IRQn);
	lowside();
	CLEAR_BIT(TIM14->CR1, TIM_CR1_CEN);
}

void BSP_TX_Send10msPulse(void);

void BSP_TX_TIM14_IRQHandler(void)
{
	/* очищаем запрос на прерывание */
	CLEAR_BIT(TIM14->SR, TIM_SR_UIF);
	toggle_tx_switch();
}

typedef enum
{
	TXC_STATE_IDLE,
	TXC_STATE_BURST,
	TXC_STATE_DAMP,
	TXC_STATE_RX
} TXC_State_n;

typedef void (*callback)(void);

TXC_State_n g_bsp_tx_state = TXC_STATE_IDLE;

callback g_bsp_on_burst_callback = 0;
callback g_bsp_on_damp_callback = 0;
callback g_bsp_on_rx_callback = 0;
callback g_bsp_on_tx_callback = 0;

/* 200 тактов -- 1 период передачи -- 25 мкс @ 40 кГц */
//#define TICKS_BURST 11000
//#define TICKS_DAMP 2000
//#define TICKS_RX   100000
  #define TICKS_BURST 100
#define TICKS_DAMP 200
#define TICKS_RX   100

void BSP_TX_StartTxSequence(void)
{
//	assert(g_bsp_tx_state == TXC_STATE_IDLE, "Tx sequence already active");
	
	g_bsp_tx_state = TXC_STATE_BURST;
	
	BSP_TX_Enable();
	BSP_TX_StartPulse();
	
	SysTick_Config(TICKS_BURST);
	//NVIC_EnableIRQ(SysTick_IRQn);
}

void BSP_TX_SysTickHandler(void)
{
	NVIC_DisableIRQ(SysTick_IRQn);
	
	switch (g_bsp_tx_state) {
		case TXC_STATE_BURST:
		{
			g_bsp_tx_state = TXC_STATE_DAMP;
			BSP_TX_StopPulse();
			SysTick_Config(TICKS_DAMP);
			return;
		}
		case TXC_STATE_DAMP:
		{
			g_bsp_tx_state = TXC_STATE_RX;
			BSP_TX_Disable();
			SysTick_Config(TICKS_RX);
			return;
		}
		case TXC_STATE_RX:
		{
			g_bsp_tx_state = TXC_STATE_IDLE;
			BSP_TX_StartTxSequence();
			return;
		}
	}
}	







