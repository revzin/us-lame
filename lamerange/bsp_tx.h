#ifndef __BSP__TX__H__
#define __BSP__TX__H__

/* готовит периферию для работы передатчика */
void BSP_TX_Setup(void);

/* выставляет nRXEN = 1 */
void BSP_TX_Enable(void);

/* выставляет nRXEN = 0 */
void BSP_TX_Disable(void);

/* начинает генерировать 40 кГц на HI-LO (non-blocking) */
void BSP_TX_StartPulse(void);

/* останавливает меандр на HI-LO */
void BSP_TX_StopPulse(void);

/* посылает меандр 40 кГц на HI-LO в течение 10 мс (non-blocking) */
void BSP_TX_Send10msPulse(void);

/* обработчик прерывания TIM14 */
void BSP_TX_TIM14_IRQHandler(void);

#endif
