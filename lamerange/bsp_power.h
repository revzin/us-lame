#ifndef __BSP__POWER__H__
#define __BSP__POWER__H__

/* высокое напряжение */

void BSP_PWR_HV_Setup(void);

void BSP_PWR_HV_Enable(void);

void BSP_PWR_HV_Disable(void);

int BSP_PWR_CheckHVVoltage(void);


/* 5 В */

void BSP_PWR_5VA_Setup(void);

void BSP_PWR_5VA_Enable(void);

void BSP_PWR_5VA_Disable(void);

int BSP_PWR_Check5VAVoltage(void);


#endif
