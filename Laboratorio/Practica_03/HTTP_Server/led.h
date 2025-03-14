#ifndef __BOARD_LED_H
#define __BOARD_LED_H

#include <stdint.h>

extern int32_t  LED_Initialize_nucleo   (void); // ---> SE HA AÑADIDO A CADA FUNCION "_stm" PARA EVITAR EL ERROR DE DUPLICIDAD CON EL BOARD.
extern int32_t  LED_Uninitialize_nucleo (void);
extern int32_t  LED_On_nucleo           (uint32_t num);
extern int32_t  LED_Off_nucleo          (uint32_t num);
extern int32_t  LED_SetOut_nucleo       (uint32_t val);
extern uint32_t LED_GetCount_nucleo     (void);

#endif
