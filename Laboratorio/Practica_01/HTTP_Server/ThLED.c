#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThLED.h"
/*----------------------------------------------------------------------------
 *      Thread LEDs
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThLED;                        // thread id

void ThLED (void *argument);                   // thread function
static void LED_Init(void);
int32_t LED_SetOut (uint32_t val);
 
int Init_ThLED (void) {
 
  tid_ThLED = osThreadNew(ThLED, NULL, NULL);
  if (tid_ThLED == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThLED (void *argument) {
  LED_Init();

  while (1) {
    
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
      osDelay(200);		// en ms
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
      osDelay(800);		// en ms


  }
}

static void LED_Init(){
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	static GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
