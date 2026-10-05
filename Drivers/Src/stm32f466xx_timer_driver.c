/*
 * stm32f466xx_timer_driver.c
 *
 *  Created on: Oct 4, 2026
 *      Author: Arun Kumar M P
 *      Description: TIM3 PWM driver for STM32F446RE
 */

#include "stm32f446xx_timer_driver.h"

void TIM3_PWM_Init(void){
	// 0. Enable the Clock (fCK_PSC / (PSC[15:0] + 1))
	TIM3_PCLK_EN();

	// 1. Configure the Timer Pre-scaler
	/* Assuming TIM3 clock = 16 MHz
     * 16 MHz / (15 + 1) = 1 MHz
     * 1 MHz = 1 us per timer tick */
	TIM3->PSC = 15U;

	// 2. Configure Timer Auto-Reload Value (20 ms PWM period → 50 Hz)
    TIM3->ARR = 19999U;

    // 3. Configure PWM Mode on Channel 1
    TIM3->CCMR1 &= ~(3U << 0);
    TIM3->CCMR1 |=  (6U << 4);

    // 4. Enable Pre-load for CCRM1
    TIM3->CCMR1 |= (1U << 3);

    // 5. Enable Channel 1 Output
    TIM3->CCER |= (1U << 0);

    // 6. Set Initial Pulse Width
    TIM3->CCR1 = 1000U;

    // 7. Enable ARR Pre-load
    TIM3->CR1 |= (1U << 7);

    // 8. Generate an Update Event
    TIM3->EGR |= (1U << 0);
}

void TIM3_PWM_Start(void)
{
    TIM3->CR1 |= (1U << 0);
}

void TIM3_PWM_SetCompare(uint32_t compare)
{
    TIM3->CCR1 = compare;
}
