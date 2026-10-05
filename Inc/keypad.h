/*
 * keypad.h
 *
 *  Created on: Oct 3, 2026
 *      Author: Arun Kumar M P
 *      Description: Keypad Interface
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "stm32f446xx_gpio_driver.h"

#define KEYPAD_ROWS     4U
#define KEYPAD_COLS     3U

void Keypad_Init(void);
char Keypad_GetKey(void);

#endif /* KEYPAD_H_ */
