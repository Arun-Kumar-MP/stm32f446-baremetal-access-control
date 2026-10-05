/*
 * stm32f446xx_gpio_driver.c
 *
 *  Created on: Oct 3, 2026
 *      Author: Arun Kumar M P
 *      Description: GPIO driver implementation for STM32F446RE
 */

#include <stdint.h>
#include "stm32f446xx_gpio_driver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if	   (pGPIOx == GPIOA)
			GPIOA_PCLK_EN();
		else if(pGPIOx == GPIOB)
			GPIOB_PCLK_EN();
		else if(pGPIOx == GPIOC)
			GPIOC_PCLK_EN();
		else if(pGPIOx == GPIOD)
			GPIOD_PCLK_EN();
		else if(pGPIOx == GPIOE)
			GPIOE_PCLK_EN();
		else if(pGPIOx == GPIOF)
			GPIOF_PCLK_EN();
		else if(pGPIOx == GPIOG)
			GPIOG_PCLK_EN();
		else if(pGPIOx == GPIOH)
			GPIOH_PCLK_EN();

	}else if(EnorDi == DISABLE){
		if	   (pGPIOx == GPIOA)
			GPIOA_PCLK_DI();
		else if(pGPIOx == GPIOB)
			GPIOB_PCLK_DI();
		else if(pGPIOx == GPIOC)
			GPIOC_PCLK_DI();
		else if(pGPIOx == GPIOD)
			GPIOD_PCLK_DI();
		else if(pGPIOx == GPIOE)
			GPIOE_PCLK_DI();
		else if(pGPIOx == GPIOF)
			GPIOF_PCLK_DI();
		else if(pGPIOx == GPIOG)
			GPIOG_PCLK_DI();
		else if(pGPIOx == GPIOH)
			GPIOH_PCLK_DI();
	}
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	// 0. Enable the Peripheral Clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	// 1. MODER
	if(pGPIOHandle->GPIO_PinConfig.PinMode <= GPIO_MODE_ANALOG){
		// CLEAR the MODE
		pGPIOHandle->pGPIOx->MODER &= ~(3U << (2U * pGPIOHandle->GPIO_PinConfig.PinNumber));
		// SET the Desired MODE
		pGPIOHandle->pGPIOx->MODER |= (pGPIOHandle->GPIO_PinConfig.PinMode << (2U * pGPIOHandle->GPIO_PinConfig.PinNumber));
	}else{
		// Configure GPIO pin as input
		pGPIOHandle->pGPIOx->MODER &= ~(3U << (2U * pGPIOHandle->GPIO_PinConfig.PinNumber));

		// Enable SYSCFG clock
		SYSCFG_PCLK_EN();

		// Configure GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.PinNumber / 4U;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.PinNumber % 4U;

		uint8_t port_code = 0;

		if(pGPIOHandle->pGPIOx == GPIOA)
			port_code = 0;
		else if(pGPIOHandle->pGPIOx == GPIOB)
			port_code = 1;
		else if(pGPIOHandle->pGPIOx == GPIOC)
			port_code = 2;
		else if(pGPIOHandle->pGPIOx == GPIOD)
			port_code = 3;
		else if(pGPIOHandle->pGPIOx == GPIOE)
			port_code = 4;
		else if(pGPIOHandle->pGPIOx == GPIOF)
			port_code = 5;
		else if(pGPIOHandle->pGPIOx == GPIOG)
			port_code = 6;
		else if(pGPIOHandle->pGPIOx == GPIOH)
			port_code = 7;

		SYSCFG->EXTICR[temp1] &= ~(0xFU << (4U * temp2));
		SYSCFG->EXTICR[temp1] |= (port_code << (4U * temp2));

		// Configure trigger
		if(pGPIOHandle->GPIO_PinConfig.PinMode == GPIO_MODE_IT_FT){
			EXTI->FTSR |= (1U << pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI->RTSR &= ~(1U << pGPIOHandle->GPIO_PinConfig.PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.PinMode == GPIO_MODE_IT_RT){
			EXTI->RTSR |= (1U << pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI->FTSR &= ~(1U << pGPIOHandle->GPIO_PinConfig.PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.PinMode == GPIO_MODE_IT_RFT){
			EXTI->RTSR |= (1U << pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI->FTSR |= (1U << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}

		// Enable interrupt mask
		EXTI->IMR |= (1U << pGPIOHandle->GPIO_PinConfig.PinNumber);
	}

	// 2. OTYPER
	// CLEAR
	pGPIOHandle->pGPIOx->OTYPER &= ~(1U << pGPIOHandle->GPIO_PinConfig.PinNumber);
	// SET
	pGPIOHandle->pGPIOx->OTYPER |= (pGPIOHandle->GPIO_PinConfig.PinOPType << pGPIOHandle->GPIO_PinConfig.PinNumber);

	// 3. OSPEEDR
	// CLEAR
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(3U << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));
	// SET
	pGPIOHandle->pGPIOx->OSPEEDR |= (pGPIOHandle->GPIO_PinConfig.PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));

	// 4. PUPDR
	// CLEAR
	pGPIOHandle->pGPIOx->PUPDR &= ~(3U << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));
	// SET
	pGPIOHandle->pGPIOx->PUPDR |= (pGPIOHandle->GPIO_PinConfig.PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.PinNumber));

	// 5. ALTFUN
	if(pGPIOHandle->GPIO_PinConfig.PinMode == GPIO_MODE_ALTFN){
		uint8_t temp1, temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.PinNumber % 8;

		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xFU << (4U * temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.PinAltFunMode << (4U * temp2));
	}
}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
    if      (pGPIOx == GPIOA)
        GPIOA_REG_RESET();
    else if (pGPIOx == GPIOB)
    	GPIOB_REG_RESET();
    else if (pGPIOx == GPIOC)
    	GPIOC_REG_RESET();
    else if (pGPIOx == GPIOD)
    	GPIOD_REG_RESET();
    else if (pGPIOx == GPIOE)
    	GPIOE_REG_RESET();
    else if (pGPIOx == GPIOF)
    	GPIOF_REG_RESET();
    else if (pGPIOx == GPIOG)
    	GPIOG_REG_RESET();
    else if (pGPIOx == GPIOH)
    	GPIOH_REG_RESET();
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 1U);
	return value;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;
	return value;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
	if(Value == GPIO_PIN_SET)
		pGPIOx->ODR |= (1U << PinNumber);
	else
		pGPIOx->ODR &= ~(1U << PinNumber);
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){
	pGPIOx->ODR = Value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^= (1U << PinNumber);
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
{
    uint8_t temp1 = IRQNumber / 32U;
    uint8_t temp2 = IRQNumber % 32U;

    // Configure NVIC IRQ Enable / Disable
    if(EnorDi == ENABLE){
        if(temp1 == 0)
            *NVIC_ISER0 |= (1U << temp2);
        else if(temp1 == 1)
            *NVIC_ISER1 |= (1U << temp2);
        else if(temp1 == 2)
            *NVIC_ISER2 |= (1U << temp2);
        else if(temp1 == 3)
            *NVIC_ISER3 |= (1U << temp2);
    }else{
        if(temp1 == 0)
            *NVIC_ICER0 |= (1U << temp2);
        else if(temp1 == 1)
            *NVIC_ICER1 |= (1U << temp2);
        else if(temp1 == 2)
            *NVIC_ICER2 |= (1U << temp2);
        else if(temp1 == 3)
            *NVIC_ICER3 |= (1U << temp2);
    }

    // Configure IRQ Priority
    *((volatile uint8_t *)NVIC_PR_BASEADDR + IRQNumber) = (IRQPriority << 4U);
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
    // Check whether interrupt is pending
    if(EXTI->PR & (1U << PinNumber)){
        // Clear pending bit
        EXTI->PR |= (1U << PinNumber);
    }
}
