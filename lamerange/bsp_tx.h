#ifndef __BSP__TX__H__
#define __BSP__TX__H__

void BSP_TX_Setup(void);

void BSP_TX_Enable(void);

void BSP_TX_Disable(void);

void BSP_TX_StartPulse(void);

void BSP_TX_StopPulse(void);

void BSP_TX_Send10msPulse(void);

void BSP_TX_TIM5_IRQHandler(void);

#endif
