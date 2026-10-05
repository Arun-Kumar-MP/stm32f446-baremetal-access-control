/*
 * stm32f446xx.h
 *
 *  Created on: Oct 3, 2026
 *      Author: Arun Kumar M P
 * 		Materials:
 *   		- STM32F446 Reference Manual - RM0390
 *   		- STM32F446xC/E Datasheet
 *   		- NUCLEO-F446RE User Manual / Schematic
 *      Description: MCU-specific definitions for STM32F446RE
 */

#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#include <stdint.h>


/************************************************
 * Processor specific details
 ************************************************/
// ARM Cortex-Mx Processor NVIC ISERx Register Addresses
#define NVIC_ISER0		((volatile uint32_t *)0xE000E100UL)
#define NVIC_ISER1		((volatile uint32_t *)0xE000E104UL)
#define NVIC_ISER2		((volatile uint32_t *)0xE000E108UL)
#define NVIC_ISER3		((volatile uint32_t *)0xE000E10CUL)

// ARM Cortex-Mx Processor NVIC ICERx Register Addresses
#define NVIC_ICER0		((volatile uint32_t *)0xE000E180UL)
#define NVIC_ICER1		((volatile uint32_t *)0xE000E184UL)
#define NVIC_ICER2		((volatile uint32_t *)0xE000E188UL)
#define NVIC_ICER3		((volatile uint32_t *)0xE000E18CUL)

// ARM Cortex-Mx Processor NVIC IPRx Register Addresses
#define NVIC_PR_BASEADDR 		((volatile uint32_t *)0xE000E400UL)
#define NO_PR_BITS_IMPELMENTED		4U

/************************************************
 * Peripheral Register Definition Structures
 ************************************************/
/* GPIO */
typedef struct{
	volatile uint32_t MODER;		// 0x00			GPIO port mode register (GPIOx_MODER) (x = A..H)
	volatile uint32_t OTYPER;		// 0x04			GPIO port output type register (GPIOx_OTYPER) (x = A..H)
	volatile uint32_t OSPEEDR;		// 0x08			GPIO port output speed register (GPIOx_OSPEEDR) (x = A..H)
	volatile uint32_t PUPDR;		// 0x0C			GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A..H)
	volatile uint32_t IDR;			// 0x10			GPIO port input data register (GPIOx_IDR) (x = A..H)
	volatile uint32_t ODR;			// 0x14			GPIO port output data register (GPIOx_ODR) (x = A..H)
	volatile uint32_t BSRR;			// 0x18			GPIO port bit set/reset register (GPIOx_BSRR) (x = A..H)
	volatile uint32_t LCKR;			// 0x1C			GPIO port configuration lock register (GPIOx_LCKR) (x = A..H)
	volatile uint32_t AFR[2];		// 0x20-0x24	AFR[0]: GPIO alternate function Low Register & AFR[1]: GPIO alternate function High Register

} GPIO_RegDef_t;

/* RCC */
typedef struct{
    volatile uint32_t CR;          // 0x00  		Clock control register
    volatile uint32_t PLLCFGR;     // 0x04  		PLL configuration register
    volatile uint32_t CFGR;        // 0x08  		Clock configuration register
    volatile uint32_t CIR;         // 0x0C  		Clock interrupt register
    volatile uint32_t AHB1RSTR;    // 0x10  		AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;    // 0x14  		AHB2 peripheral reset register
    volatile uint32_t AHB3RSTR;    // 0x18  		AHB3 peripheral reset register
    volatile uint32_t RESERVED0;   // 0x1C  		Reserved
    volatile uint32_t APB1RSTR;    // 0x20  		APB1 peripheral reset register
    volatile uint32_t APB2RSTR;    // 0x24  		APB2 peripheral reset register
    volatile uint32_t RESERVED1[2];// 0x28-0x2C 	Reserved
    volatile uint32_t AHB1ENR;     // 0x30  		AHB1 peripheral clock enable
    volatile uint32_t AHB2ENR;     // 0x34  		AHB2 peripheral clock enable
    volatile uint32_t AHB3ENR;     // 0x38  		AHB3 peripheral clock enable
    volatile uint32_t RESERVED2;   // 0x3C  		Reserved
    volatile uint32_t APB1ENR;     // 0x40  		APB1 peripheral clock enable
    volatile uint32_t APB2ENR;     // 0x44  		APB2 peripheral clock enable
    volatile uint32_t RESERVED3[2];// 0x48-0x4C 	Reserved
    volatile uint32_t AHB1LPENR;   // 0x50  		AHB1 low-power enable
    volatile uint32_t AHB2LPENR;   // 0x54  		AHB2 low-power enable
    volatile uint32_t AHB3LPENR;   // 0x58  		AHB3 low-power enable
    volatile uint32_t RESERVED4;   // 0x5C  		Reserved
    volatile uint32_t APB1LPENR;   // 0x60  		APB1 low-power enable
    volatile uint32_t APB2LPENR;   // 0x64  		APB2 low-power enable
    volatile uint32_t RESERVED5[2];// 0x68-0x6C 	Reserved
    volatile uint32_t BDCR;        // 0x70  		Backup domain control register
    volatile uint32_t CSR;         // 0x74  		Control/status register
    volatile uint32_t RESERVED6[2];// 0x78-0x7C 	Reserved
    volatile uint32_t SSCGR;       // 0x80  		Spread spectrum clock generation
    volatile uint32_t PLLI2SCFGR;  // 0x84  		PLLI2S configuration register
    volatile uint32_t PLLSAICFGR;  // 0x88  		PLLSAI configuration register
    volatile uint32_t DCKCFGR;     // 0x8C  		Dedicated clocks configuration
    volatile uint32_t CKGATENR;    // 0x90  		Clocks gated enable register
    volatile uint32_t DCKCFGR2;    // 0x94  		Dedicated clocks configuration 2

} RCC_RegDef_t;

/* SYSCFG */
typedef struct
{
    volatile uint32_t MEMRMP;	 	// 0x00  		SYSCFG memory remap register
    volatile uint32_t PMC;      	// 0x04  		SYSCFG peripheral mode configuration register
    volatile uint32_t EXTICR[4]; 	// 0x08-0x14  	SYSCFG external interrupt configuration register
    volatile uint32_t RESERVED1[2]; // 0x18-0x1C 	Reserved
    volatile uint32_t CMPCR;   	 	// 0x20  		SYSCFG Compensation cell control register
    volatile uint32_t RESERVED2[2]; // 0x24-0x28 	Reserved
    volatile uint32_t CFGR;   	 	// 0x2C  		SYSCFG configuration register

} SYSCFG_RegDef_t;

/* EXTI */
typedef struct{
	volatile uint32_t IMR;			// 0x00			Interrupt mask register (EXTI_IMR)
	volatile uint32_t EMR;			// 0x04			Event mask register (EXTI_EMR)
	volatile uint32_t RTSR;			// 0x08			Rising trigger selection register (EXTI_RTSR)
	volatile uint32_t FTSR;			// 0x0C			Falling trigger selection register (EXTI_FTSR)
	volatile uint32_t SWIER;		// 0x10			Software interrupt event register (EXTI_SWIER)
	volatile uint32_t PR;			// 0x14			Pending register (EXTI_PR)

} EXTI_RegDef_t;


/* SPI */
typedef struct{
	volatile uint32_t CR1;			// 0x00			SPI control register 1 (SPI_CR1) (not used in I2S mode)
	volatile uint32_t CR2;			// 0x04			SPI control register 2 (SPI_CR2)
	volatile uint32_t SR;			// 0x08			SPI status register (SPI_SR)
	volatile uint32_t DR;			// 0x0C			SPI data register (SPI_DR)
	volatile uint32_t CRCPR;		// 0x10			SPI CRC polynomial register (SPI_CRCPR) (not used in I2S mode)
	volatile uint32_t RXCRCR;		// 0x14			SPI RX CRC register (SPI_RXCRCR) (not used in I2S mode)
	volatile uint32_t TXCRCR;		// 0x18			SPI TX CRC register (SPI_TXCRCR) (not used in I2S mode)
	volatile uint32_t I2SCFGR;		// 0x1C			SPI_I2S configuration register (SPI_I2SCFGR)
	volatile uint32_t I2SPR;		// 0x20			SPI_I2S prescaler register (SPI_I2SPR)

} SPI_RegDef_t;

/* TIMx */
// TIM2 to TIM5 registers
typedef struct{
	volatile uint32_t CR1;			// 0x00			TIMx control register 1 (TIMx_CR1)
	volatile uint32_t CR2;			// 0x04			TIMx control register 2 (TIMx_CR2)
	volatile uint32_t SMCR;			// 0x08			TIMx slave mode control register (TIMx_SMCR)
	volatile uint32_t DIER;			// 0x0C			TIMx DMA/Interrupt enable register (TIMx_DIER)
	volatile uint32_t SR;			// 0x10			TIMx status register (TIMx_SR)
	volatile uint32_t EGR;			// 0x14			TIMx event generation register (TIMx_EGR)
	volatile uint32_t CCMR1;		// 0x18			TIMx capture/compare mode register 1 (TIMx_CCMR1)
	volatile uint32_t CCMR2;		// 0x1C			TIMx capture/compare mode register 2 (TIMx_CCMR2)
	volatile uint32_t CCER;			// 0x20			TIMx capture/compare enable register (TIMx_CCER)
	volatile uint32_t CNT;			// 0x24			TIMx counter (TIMx_CNT)
	volatile uint32_t PSC;			// 0x28			TIMx pre-scaler (TIMx_PSC)
	volatile uint32_t ARR;			// 0x2C			TIMx auto-reload register (TIMx_ARR)
	volatile uint32_t RESERVED0;	// 0x30
	volatile uint32_t CCR1;			// 0x34			TIMx capture/compare register 1 (TIMx_CCR1)
	volatile uint32_t CCR2;			// 0x38			TIMx capture/compare register 2 (TIMx_CCR2)
	volatile uint32_t CCR3;			// 0x3C			TIMx capture/compare register 3 (TIMx_CCR3)
	volatile uint32_t CCR4;			// 0x40			TIMx capture/compare register 4 (TIMx_CCR4)
	volatile uint32_t RESERVED1;	// 0x44
	volatile uint32_t DCR;			// 0x48			TIMx DMA control register (TIMx_DCR)
	volatile uint32_t DMAR;			// 0x4C			TIMx DMA address for full transfer (TIMx_DMAR)
	volatile uint32_t OR;			// 0x50			TIM2/5 option register (TIM2/5_OR)

} TIM_RegDef_t;

/************************************************
 * Peripheral Base Addresses
 ************************************************/
/* Memory */
#define FLASH_BASEADDR		0x08000000UL	// Non-Volatile Memory
#define SRAM1_BASEADDR		0x20000000UL	// Volatile Memory
#define SRAM2_BASEADDR		0x2001C000UL	// Volatile Memory
#define ROM_BASEADDR		0x1FFF0000UL	// System Memory
#define SRAM				SRAM1_BASEADDR

/* Bus */
#define PERIPH_BASEADDR				0x40000000UL
// APBx: Advanced Peripheral Bus (x = 1, 2)
#define APB1_PERIPH_BASEADDR		PERIPH_BASEADDR
#define APB2_PERIPH_BASEADDR		0x40010000UL
// AHBx: Advanced High-performance Bus (x = 1, 2, 3)
#define AHB1_PERIPH_BASEADDR		0x40020000UL
#define AHB2_PERIPH_BASEADDR		0x50000000UL
#define AHB3_PERIPH_BASEADDR		0xA0001000UL

// GPIOx: General Purpose Input/Outputs (x = A..H)
#define GPIOA_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x0000U)
#define GPIOB_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x0400U)
#define GPIOC_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x1000U)
#define GPIOF_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x1400U)
#define GPIOG_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x1800U)
#define GPIOH_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x1C00U)

/* RCC */
#define RCC_BASEADDR		(AHB1_PERIPH_BASEADDR + 0x3800U)

/* SPI */
#define SPI1_BASEADDR		(APB2_PERIPH_BASEADDR + 0x3000U)
#define SPI2_BASEADDR		(APB1_PERIPH_BASEADDR + 0x3800U)
#define SPI3_BASEADDR		(APB1_PERIPH_BASEADDR + 0x3C00U)
#define SPI4_BASEADDR		(APB2_PERIPH_BASEADDR + 0x3400U)

#define SYSCFG_BASEADDR		(APB2_PERIPH_BASEADDR + 0x3800U)

#define EXTI_BASEADDR		(APB2_PERIPH_BASEADDR + 0x3C00U)

/* TIM2-5 */
#define TIM2_BASEADDR		(APB1_PERIPH_BASEADDR + 0x0000U)
#define TIM3_BASEADDR		(APB1_PERIPH_BASEADDR + 0x0400U)
#define TIM4_BASEADDR		(APB1_PERIPH_BASEADDR + 0x0800U)
#define TIM5_BASEADDR		(APB1_PERIPH_BASEADDR + 0x0C00U)

/************************************************
 * Peripheral Definitions
 ************************************************/
/* GPIO */
#define GPIOA		((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB		((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC		((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD		((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE		((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF		((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG		((GPIO_RegDef_t *)GPIOG_BASEADDR)
#define GPIOH		((GPIO_RegDef_t *)GPIOH_BASEADDR)

/* RCC */
#define RCC			((RCC_RegDef_t *)RCC_BASEADDR)

/* EXTI */
#define EXTI		((EXTI_RegDef_t *)EXTI_BASEADDR)

/* SYSCFG */
#define SYSCFG		((SYSCFG_RegDef_t *)SYSCFG_BASEADDR)

/* SPI */
#define SPI1		((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2		((SPI_RegDef_t *)SPI2_BASEADDR)
#define SPI3		((SPI_RegDef_t *)SPI3_BASEADDR)
#define SPI4		((SPI_RegDef_t *)SPI4_BASEADDR)

/* TIM2-5 */
#define TIM2		((TIM_RegDef_t *)TIM2_BASEADDR)
#define TIM3		((TIM_RegDef_t *)TIM3_BASEADDR)
#define TIM4		((TIM_RegDef_t *)TIM4_BASEADDR)
#define TIM5		((TIM_RegDef_t *)TIM5_BASEADDR)

/************************************************
 * Clock Definitions
 ************************************************/
// Clock Enable Macro for GPIOx Peripherals
#define GPIOA_PCLK_EN()			(RCC -> AHB1ENR |= (1U << 0))
#define GPIOB_PCLK_EN()			(RCC -> AHB1ENR |= (1U << 1))
#define GPIOC_PCLK_EN()			(RCC -> AHB1ENR |= (1U << 2))
#define GPIOD_PCLK_EN()			(RCC -> AHB1ENR |= (1U << 3))
#define GPIOE_PCLK_EN()			(RCC -> AHB1ENR |= (1U << 4))
#define GPIOF_PCLK_EN()			(RCC -> AHB1ENR |= (1U << 5))
#define GPIOG_PCLK_EN()			(RCC -> AHB1ENR |= (1U << 6))
#define GPIOH_PCLK_EN()			(RCC -> AHB1ENR |= (1U << 7))

// Clock Enable Macro for SYSCFG Peripherals
#define SYSCFG_PCLK_EN()		(RCC -> APB2ENR |= (1U << 14))

// Clock Enable Macro for SPIx Peripherals
#define SPI1_PCLK_EN()			(RCC -> APB2ENR |= (1U << 12))
#define SPI2_PCLK_EN()			(RCC -> APB1ENR |= (1U << 14))
#define SPI3_PCLK_EN()			(RCC -> APB1ENR |= (1U << 15))
#define SPI4_PCLK_EN()			(RCC -> APB2ENR |= (1U << 13))

// Clock Enable Macro for TIM2-5 Peripherals
#define TIM2_PCLK_EN()			(RCC -> APB1ENR |= (1U << 0))
#define TIM3_PCLK_EN()			(RCC -> APB1ENR |= (1U << 1))
#define TIM4_PCLK_EN()			(RCC -> APB1ENR |= (1U << 2))
#define TIM5_PCLK_EN()			(RCC -> APB1ENR |= (1U << 3))


// Clock Disable Macro for GPIOx Peripherals
#define GPIOA_PCLK_DI()			(RCC -> AHB1ENR &= ~(1U << 0))
#define GPIOB_PCLK_DI()			(RCC -> AHB1ENR &= ~(1U << 1))
#define GPIOC_PCLK_DI()			(RCC -> AHB1ENR &= ~(1U << 2))
#define GPIOD_PCLK_DI()			(RCC -> AHB1ENR &= ~(1U << 3))
#define GPIOE_PCLK_DI()			(RCC -> AHB1ENR &= ~(1U << 4))
#define GPIOF_PCLK_DI()			(RCC -> AHB1ENR &= ~(1U << 5))
#define GPIOG_PCLK_DI()			(RCC -> AHB1ENR &= ~(1U << 6))
#define GPIOH_PCLK_DI()			(RCC -> AHB1ENR &= ~(1U << 7))

// Clock Disable Macro for SYSCFG Peripherals
#define SYSCFG_PCLK_DI()		(RCC -> APB2ENR &= ~(1U << 14))

// Clock Disable Macro for SPIx Peripherals
#define SPI1_PCLK_DI()			(RCC -> APB2ENR &= ~(1U << 12))
#define SPI2_PCLK_DI()			(RCC -> APB1ENR &= ~(1U << 14))
#define SPI3_PCLK_DI()			(RCC -> APB1ENR &= ~(1U << 15))
#define SPI4_PCLK_DI()			(RCC -> APB2ENR &= ~(1U << 13))

// Clock Disable Macro for TIM2-5 Peripherals
#define TIM2_PCLK_DI()			(RCC -> APB1ENR &= ~(1U << 0))
#define TIM3_PCLK_DI()			(RCC -> APB1ENR &= ~(1U << 1))
#define TIM4_PCLK_DI()			(RCC -> APB1ENR &= ~(1U << 2))
#define TIM5_PCLK_DI()			(RCC -> APB1ENR &= ~(1U << 3))

/************************************************
 * IRQ Definitions
 ************************************************/
#define IRQ_NO_EXTI0		 6U
#define IRQ_NO_EXTI1		 7U
#define IRQ_NO_EXTI2		 8U
#define IRQ_NO_EXTI3		 9U
#define IRQ_NO_EXTI4		10U
#define IRQ_NO_EXTI9_5		23U
#define IRQ_NO_EXTI15_10	40U

#define IRQ_NO_SPI1			35U
#define IRQ_NO_SPI2			46U
#define IRQ_NO_SPI3			51U
#define IRQ_NO_SPI4			84U

// IRQ's Priority Levels
#define NVIC_IRQ_PRIORITY_0		0U
#define NVIC_IRQ_PRIORITY_1		1U
#define NVIC_IRQ_PRIORITY_2		2U
#define NVIC_IRQ_PRIORITY_3		3U
#define NVIC_IRQ_PRIORITY_4		4U
#define NVIC_IRQ_PRIORITY_5		5U
#define NVIC_IRQ_PRIORITY_6		6U
#define NVIC_IRQ_PRIORITY_7		7U
#define NVIC_IRQ_PRIORITY_8		8U
#define NVIC_IRQ_PRIORITY_9		9U
#define NVIC_IRQ_PRIORITY_10   10U
#define NVIC_IRQ_PRIORITY_11   11U
#define NVIC_IRQ_PRIORITY_12   12U
#define NVIC_IRQ_PRIORITY_13   13U
#define NVIC_IRQ_PRIORITY_14   14U
#define NVIC_IRQ_PRIORITY_15   15U

/************************************************
 * Miscellaneous
 ************************************************/
// GPIOx Port RESET
#define GPIOA_REG_RESET()		do{ (RCC -> AHB1RSTR |= (1U << 0)); (RCC -> AHB1RSTR &= ~(1U << 0)); } while(0)
#define GPIOB_REG_RESET()		do{ (RCC -> AHB1RSTR |= (1U << 1)); (RCC -> AHB1RSTR &= ~(1U << 1)); } while(0)
#define GPIOC_REG_RESET()		do{ (RCC -> AHB1RSTR |= (1U << 2)); (RCC -> AHB1RSTR &= ~(1U << 2)); } while(0)
#define GPIOD_REG_RESET()		do{ (RCC -> AHB1RSTR |= (1U << 3)); (RCC -> AHB1RSTR &= ~(1U << 3)); } while(0)
#define GPIOE_REG_RESET()		do{ (RCC -> AHB1RSTR |= (1U << 4)); (RCC -> AHB1RSTR &= ~(1U << 4)); } while(0)
#define GPIOF_REG_RESET()		do{ (RCC -> AHB1RSTR |= (1U << 5)); (RCC -> AHB1RSTR &= ~(1U << 5)); } while(0)
#define GPIOG_REG_RESET()		do{ (RCC -> AHB1RSTR |= (1U << 6)); (RCC -> AHB1RSTR &= ~(1U << 6)); } while(0)
#define GPIOH_REG_RESET()		do{ (RCC -> AHB1RSTR |= (1U << 7)); (RCC -> AHB1RSTR &= ~(1U << 7)); } while(0)

// SPIx Port RESET
#define SPI1_REG_RESET()		do{ (RCC -> APB2RSTR |= (1U << 12)); (RCC -> APB2RSTR &= ~(1U << 12)); } while(0)
#define SPI2_REG_RESET()		do{ (RCC -> APB1RSTR |= (1U << 14)); (RCC -> APB1RSTR &= ~(1U << 14)); } while(0)
#define SPI3_REG_RESET()		do{ (RCC -> APB1RSTR |= (1U << 15)); (RCC -> APB1RSTR &= ~(1U << 15)); } while(0)
#define SPI4_REG_RESET()		do{ (RCC -> APB2RSTR |= (1U << 13)); (RCC -> APB2RSTR &= ~(1U << 13)); } while(0)

#define RESET 			0U
#define SET 			1U
#define ENABLE			SET
#define DISABLE			RESET
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET
#define FLAG_SET		SET
#define FLAG_RESET		RESET



#endif /* INC_STM32F446XX_H_ */
