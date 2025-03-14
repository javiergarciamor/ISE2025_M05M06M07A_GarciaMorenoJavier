#ifndef __RTC_H
#define __RTC_H


#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "cmsis_os2.h"  

#define FLAG_GET_HOUR		 1

#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */


osThreadId_t get_id_Th_rtc(void);
int Init_Th_RTC (void);
void Button_config(void) ;


#endif /* __RTC_H */