/*
 * buzzer.h
 *
 *  Created on: Oct 3, 2026
 *      Author: Arun Kumar M P
 *      Description: Buzzer Interface
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "stm32f446xx_gpio_driver.h"

void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);

#endif /* BUZZER_H_ */
