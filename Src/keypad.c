/*
 * keypad.c
 *
 *  Created on: Oct 3, 2026
 *      Author: Arun Kumar M P
 *      Description: Keypad Implementation
 */

/*
4 × 3 Keypad

            C1(PC4) C2(PC5) C3(PC6)
             │       │       │
R1 (PC0) ────┼───────┼───────┼
R2 (PC1) ────┼───────┼───────┼
R3 (PC2) ────┼───────┼───────┼
R4 (PC3) ────┼───────┼───────┼
 */

#include <stdio.h>
#include "keypad.h"
#include "stm32f446xx_gpio_driver.h"

#include "keypad.h"

static const char Keypad_Map[KEYPAD_ROWS][KEYPAD_COLS] = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
		{'*', '0', '#'}
};

static void Keypad_Delay(void)
{
	for(volatile uint32_t i = 0; i <= 30000; i++);
}

void Keypad_Init(void)
{
	GPIO_Handle_t keypad;

	/* ROWS --> Output */

	keypad.pGPIOx = GPIOC;
	keypad.GPIO_PinConfig.PinMode = GPIO_MODE_OUT;
	keypad.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	keypad.GPIO_PinConfig.PinSpeed = GPIO_SPEED_FAST;
	keypad.GPIO_PinConfig.PinPuPdControl = GPIO_NO_PUPD;

	keypad.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_0;
	GPIO_Init(&keypad);

	keypad.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_1;
	GPIO_Init(&keypad);

	keypad.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_2;
	GPIO_Init(&keypad);

	keypad.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_3;
	GPIO_Init(&keypad);

	/* COLUMNS --> Input */

	keypad.GPIO_PinConfig.PinMode = GPIO_MODE_IN;
	keypad.GPIO_PinConfig.PinSpeed = GPIO_SPEED_FAST;
	keypad.GPIO_PinConfig.PinPuPdControl = GPIO_PIN_PU;

	keypad.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_4;
	GPIO_Init(&keypad);

	keypad.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_5;
	GPIO_Init(&keypad);

	keypad.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&keypad);
}

char Keypad_GetKey(void)
{
	uint8_t row;
	uint8_t col;

	/* Scan Each Row */

	for(row = 0; row < KEYPAD_ROWS; row++)
	{
		/* Set all Rows High */

		GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_0, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_1, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_2, GPIO_PIN_SET);
		GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_3, GPIO_PIN_SET);

		/* Activate Current Row */

		GPIO_WriteToOutputPin(GPIOC, row, GPIO_PIN_RESET);

		/* Check all Columns */

		for(col = 0; col < KEYPAD_COLS; col++)
		{
			Keypad_Delay();

			if(GPIO_ReadFromInputPin(GPIOC, col + 4) == GPIO_PIN_RESET)
			{
				return Keypad_Map[row][col];
			}
		}
	}

	return '\0';
}


/*
 * Test
int main(void){
	Keypad_Init();
	while(1){
		uint8_t value = Keypad_GetKey();
		if(value != '\0')
			printf("Key Pressed: %c", value);
	}
	return 0;
}
 */
