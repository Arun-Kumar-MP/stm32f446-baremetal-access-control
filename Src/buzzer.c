/*
 * buzzer.c
 *
 *  Created on: Oct 3, 2026
 *      Author: Arun Kumar M P
 *      Description: Buzzer Implementation
 */

#include "buzzer.h"

void Buzzer_Init(void)
{
	GPIO_Handle_t buzzer;

	buzzer.pGPIOx = GPIOC;
	buzzer.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_8;
	buzzer.GPIO_PinConfig.PinMode = GPIO_MODE_OUT;
	buzzer.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	buzzer.GPIO_PinConfig.PinSpeed = GPIO_SPEED_SLOW;
	buzzer.GPIO_PinConfig.PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&buzzer);
}

void Buzzer_On(void)
{
	GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_8, GPIO_PIN_SET);
}

void Buzzer_Off(void)
{
	GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_8, GPIO_PIN_RESET);
}

/*
 * Test
int main(void){
	Buzzer_Init();

	while(1){
		Buzzer_On();
		delay();

		Buzzer_Off();
		delay();
	}
	return 0;
}
 */
