#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include <stdio.h>
#include <time.h>
#include "RTC.h"
#include "rl_net_lib.h"
/*----------------------------------------------------------------------------
 *      Thread RTC 'Th_RTC': 
 *---------------------------------------------------------------------------*/
 /////////////////////// declaraciones del RTC ///////////////////
 RTC_HandleTypeDef RtcHandle;
 
 RTC_DateTypeDef sdatestructure;
 RTC_TimeTypeDef stimestructure;

	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
 /////////////////////////////////////////////////////////////////
 
/////////////////// declaración de funciones //////////////////////
static void init_RTC(void);
static void RTC_CalendarConfig(uint8_t h, uint8_t m, uint8_t s, uint16_t y, uint8_t mes, uint8_t d);
static void RTC_CalendarShow(void);
///////////////////////////////////////////////////////////////////

////////////////// SNTP ////////////////////////////////////////////
struct tm tiempo;
static void time_callback (uint32_t seconds, uint32_t seconds_fraction);
////////////////////////////////////////////////////////////////////
 
///////////////// BOTON AZUL ///////////////////////////////////////
static GPIO_InitTypeDef  GPIO_InitStruct;
void Button_config(void){
  __HAL_RCC_GPIOC_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
////////////////////////////////////////////////////////////////////
	

osThreadId_t tid_Th_RTC;                        // thread id
 
void Th_RTC (void *argument);                   // thread function
 
int Init_Th_RTC (void) {
 
  tid_Th_RTC = osThreadNew(Th_RTC, NULL, NULL);
  if (tid_Th_RTC == NULL) {
    return(-1);
  }
 
  return(0);
}

osThreadId_t get_id_Th_RTC(void){
	return tid_Th_RTC;
}

static void init_RTC (void){
	
	RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  __HAL_RTC_RESET_HANDLE_STATE(&RtcHandle);
	
  HAL_RTC_Init(&RtcHandle);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc){
	
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  __HAL_RCC_RTC_ENABLE();
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc){
  __HAL_RCC_RTC_DISABLE();

  HAL_PWR_DisableBkUpAccess();
  __HAL_RCC_PWR_CLK_DISABLE();
  
}

///////////////////// SNTP //////////////////////////////
void init_SNTP (void) {
 netSNTPc_GetTime (NULL, time_callback);
}

 static void time_callback (uint32_t seconds, uint32_t seconds_fraction) {
  if (seconds != 0) {
   tiempo = *localtime(&seconds);
   sdatestructure.Year = tiempo.tm_year - 100;
   sdatestructure.Month = tiempo.tm_mon + 1;
   sdatestructure.Date = tiempo.tm_mday;
   sdatestructure.WeekDay = tiempo.tm_wday;
  
   HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BIN);
   if(tiempo.tm_hour==23){
	 stimestructure.Hours = 0 ;
	 }
   stimestructure.Hours = tiempo.tm_hour + 1 ;
   stimestructure.Minutes = tiempo.tm_min;
   stimestructure.Seconds = tiempo.tm_sec;
   stimestructure.TimeFormat = RTC_HOURFORMAT_24;
   stimestructure.DayLightSaving = tiempo.tm_isdst ;
   stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
   
	 HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BIN);
	
	   /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
  }
}
///////////////////////////////////////////////////////////////////////////////////

	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin==GPIO_PIN_13){
    RTC_CalendarConfig(0,0,0,2000,1,1);
    }
  }
static void RTC_CalendarConfig(uint8_t h, uint8_t m, uint8_t s, uint16_t y, uint8_t mes, uint8_t d)
{

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2025 */
  sdatestructure.Year = y;
  sdatestructure.Month = mes;
  sdatestructure.Date = d;
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
  
	HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD);
	

  stimestructure.Hours = h;
  stimestructure.Minutes = m;
  stimestructure.Seconds = s;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

	HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD);


  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}

static void RTC_CalendarShow(void)
{

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructure, RTC_FORMAT_BIN);
  
	year = sdatestructure.Year;
	month = sdatestructure.Month;
	day = sdatestructure.Date;
	hour = stimestructure.Hours;
	min= stimestructure.Minutes;
	sec = stimestructure.Seconds;
}

void Th_RTC (void *argument) {
	
  init_RTC();
//	RTC_CalendarConfig();
	init_SNTP();
	Button_config();
	
	
  while (1) {
		
		if(min%3==0 && sec==0){
		init_SNTP();			
		} 

		RTC_CalendarShow();
  }
}


