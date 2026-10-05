/*
 * stm32f446xx_timer_driver.h
 *
 *  Created on: Oct 4, 2026
 *      Author: Arun Kumar M P
 *      Description: Timer driver interface for STM32F446RE
 */

#ifndef INC_STM32F446XX_TIMER_DRIVER_H_
#define INC_STM32F446XX_TIMER_DRIVER_H_

#include "stm32f446xx.h"

void TIM3_PWM_Init(void);
void TIM3_PWM_Start(void);
void TIM3_PWM_SetCompare(uint32_t compare);

#endif /* INC_STM32F446XX_TIMER_DRIVER_H_ */
