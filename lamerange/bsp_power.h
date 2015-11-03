#ifndef __BSP__POWER__H__
#define __BSP__POWER__H__

/* высокое напряжение */

/* готовит переферию для работы с ВН */
void BSP_PWR_HV_Setup(void);

/* включить источник ВН на плате */
void BSP_PWR_HV_Enable(void);

/* выключить источник ВН на плате */ 
void BSP_PWR_HV_Disable(void);

/* возвращает 0, если на линии ВН есть высокое напряжение */
int BSP_PWR_CheckHVVoltage(void);


/* 5 В */
/* готовит переферию для работы с 5В */
void BSP_PWR_5VA_Setup(void);

/* включить источник 5В на плате */
void BSP_PWR_5VA_Enable(void);

/* выключить источник 5В на плате */ 
void BSP_PWR_5VA_Disable(void);

/* возвращает 0, если цепи 5 В запитаны */
int BSP_PWR_Check5VAVoltage(void);


#endif
