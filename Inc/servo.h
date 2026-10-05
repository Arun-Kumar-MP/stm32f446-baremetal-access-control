/*
 * servo.h
 *
 *  Created on: Oct 4, 2026
 *      Author: Arun Kumar M P
 *      Description: SG90 Servo Interface
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_timer_driver.h"

void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);

#endif /* SERVO_H_ */
