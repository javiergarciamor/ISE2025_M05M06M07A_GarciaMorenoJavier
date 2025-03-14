#ifndef __SLEEP_H
#define __SLEEP_H


#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "cmsis_os2.h"  

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SleepMode_Measure(void);
void StopMode_Measure(void);
void StopUnderDriveMode_Measure(void);
void StandbyMode_Measure(void);
void StandbyRTCMode_Measure(void);
void StandbyRTCBKPSRAMMode_Measure(void);
void ETH_PhyEnterPowerDownMode(void);
void ETH_PhyExitFromPowerDownMode(void);


#endif /* __SLEEP_H */