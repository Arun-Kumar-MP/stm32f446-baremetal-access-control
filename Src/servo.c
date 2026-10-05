/*
 * servo.c
 *
 *  Created on: Oct 4, 2026
 *      Author: Arun Kumar M P
 *      Description: SG90 Servo Implementation
 */

#include "servo.h"

void Servo_Init(void)
{
    GPIO_Handle_t servo;

    servo.pGPIOx = GPIOB;

    servo.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_4;
    servo.GPIO_PinConfig.PinMode = GPIO_MODE_ALTFN;
    servo.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
    servo.GPIO_PinConfig.PinSpeed = GPIO_SPEED_FAST;
    servo.GPIO_PinConfig.PinPuPdControl = GPIO_NO_PUPD;
    servo.GPIO_PinConfig.PinAltFunMode = 2;       // AF2 → TIM3_CH1

    GPIO_Init(&servo);

    TIM3_PWM_Init();
    TIM3_PWM_Start();
}

void Servo_SetAngle(uint8_t angle)
{
    uint32_t pulse_width;

    if(angle > 180U)
        angle = 180U;

    /* 0°   → 1000 us
     * 180° → 2000 us */

    pulse_width = 1000U + (((uint32_t)angle * 1000U) / 180U);

    TIM3_PWM_SetCompare(pulse_width);
}

/*
 * Testing
int main(void){
	Servo_Init();
	for(uint8_t i = 0; i <= 360; i += 10){
		delay();
		Servo_SetAngle(i);
	}

	while(1);
	return 0;
}
 */
