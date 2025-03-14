#include "stm32f4xx_hal.h"
#include "Board_LED.h"

/* GPIO Pin identifier */
typedef struct _GPIO_PIN {
  GPIO_TypeDef *port;
  uint16_t      pin;
  uint16_t      reserved;
} GPIO_PIN;

/* LED GPIO Pins */
static const GPIO_PIN LED_PIN[] = {
{ GPIOB, GPIO_PIN_0, 0},
  { GPIOB, GPIO_PIN_7, 0},
  { GPIOB, GPIO_PIN_14,0},
};

#define LED_COUNT_nucleo (sizeof(LED_PIN)/sizeof(GPIO_PIN))

int32_t LED_Initialize_nucleo (void) {
  GPIO_InitTypeDef GPIO_InitStruct;
	uint32_t i;

  /* GPIO Ports Clock Enable */
  __GPIOG_CLK_ENABLE();
	
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;


  for (i = 0; i < LED_COUNT_nucleo; i++){
    GPIO_InitStruct.Pin = LED_PIN[i].pin;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

  return 0;
}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Uninitialize_nucleo (void) {

    uint32_t i;

  for (i = 0; i < LED_COUNT_nucleo; i++) {
    HAL_GPIO_DeInit(LED_PIN[i].port, LED_PIN[i].pin);
  }

  return 0;
}


int32_t LED_On_nucleo (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT_nucleo) {
    HAL_GPIO_WritePin(LED_PIN[num].port, LED_PIN[num].pin, GPIO_PIN_SET);
  }
  else {
    retCode = -1;
  }

  return retCode;
}


int32_t LED_Off_nucleo (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT_nucleo) {
    HAL_GPIO_WritePin(LED_PIN[num].port, LED_PIN[num].pin, GPIO_PIN_RESET);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_SetOut_nucleo (uint32_t val) {
  uint32_t n;

  for (n = 0; n < LED_COUNT_nucleo; n++) {
    if (val & (1 << n)) LED_On_nucleo (n);
    else                LED_Off_nucleo(n);
  }

  return 0;
}

/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
uint32_t LED_GetCount_nucleo (void) {

  return LED_COUNT_nucleo;
}
