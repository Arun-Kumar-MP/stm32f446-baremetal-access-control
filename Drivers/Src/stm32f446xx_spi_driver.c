/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: Oct 4, 2026
 *      Author: Arun Kumar M P
 *      Description: SPI driver implementation for STM32F446RE
 */

#include "stm32f446xx_spi_driver.h"

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pSPIx == SPI1)
			SPI1_PCLK_EN();
		else if(pSPIx == SPI2)
			SPI2_PCLK_EN();
		else if(pSPIx == SPI3)
			SPI3_PCLK_EN();
		else if(pSPIx == SPI4)
			SPI4_PCLK_EN();
	}else{
		if(pSPIx == SPI1)
			SPI1_PCLK_DI();
		else if(pSPIx == SPI2)
			SPI2_PCLK_DI();
		else if(pSPIx == SPI3)
			SPI3_PCLK_DI();
		else if(pSPIx == SPI4)
			SPI4_PCLK_DI();
	}
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
    if(EnorDi == ENABLE){
        pSPIx->CR1 |= (1U << SPI_CR1_SPE);
    }else{
        pSPIx->CR1 &= ~(1U << SPI_CR1_SPE);
    }
}

void SPI_Init(SPI_Handle_t *pSPIHandle){
	uint32_t tempReg = 0;

	// 0. Enable the Peripheral Clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	// 1. Device Mode Configure
	tempReg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

	// 2. Bus Configuration
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
		// Full Duplex
		tempReg &= ~(1U << SPI_CR1_BIDIMODE);

	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
		// Half Duplex
		tempReg |= (1U << SPI_CR1_BIDIMODE);

	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
		// Simplex Receive Only
		tempReg &= ~(1U << SPI_CR1_BIDIMODE);
        tempReg |= (1U << SPI_CR1_RXONLY);
	}

	// 3. Configure SPI clock speed
	tempReg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);

	// 4. Configure CPOL
	tempReg |= (pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);

	// 5. Configure CPHA
	tempReg |= (pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);

	// 6. Configure software slave management
	if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN){
	    tempReg |= (1U << SPI_CR1_SSM);
	    tempReg |= (1U << SPI_CR1_SSI);
	}

	// 7. Configure MSB first
	tempReg &= ~(1U << SPI_CR1_LSBFIRST);

	// 8. Configure 8-bit data frame
	tempReg &= ~(1U << SPI_CR1_DFF);

	// 9. Write configuration
	pSPIHandle->pSPIx->CR1 = tempReg;
}

void SPI_DeInit(SPI_RegDef_t *pSPIx){

}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint8_t FlagName){
    if(pSPIx->SR & FlagName)
        return FLAG_SET;
    return FLAG_RESET;
}

/* Helper function for full-duplex single-byte exchange */
uint8_t SPI_ReadWriteByte(SPI_RegDef_t *pSPIx, uint8_t txByte) {
    // 0. Flush any stale data left in RX buffer
    while (pSPIx->SR & SPI_FLAG_RXNE) {
        (void)*(volatile uint8_t *)&pSPIx->DR;
    }

    // 1. Wait until TX buffer is empty
    while (SPI_GetFlagStatus(pSPIx, SPI_FLAG_TXE) == FLAG_RESET);

    // 2. Force 8-bit memory write to DR
    *(volatile uint8_t *)&pSPIx->DR = txByte;

    // 3. Wait until RX buffer has received the response byte
    while (SPI_GetFlagStatus(pSPIx, SPI_FLAG_RXNE) == FLAG_RESET);

    // 4. Read and return the newly received byte
    return *(volatile uint8_t *)&pSPIx->DR;
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len) {
    while(Len > 0) {
        // Send dummy byte 0xFF to generate SPI clock, then store received byte
        *pRxBuffer = SPI_ReadWriteByte(pSPIx, 0xFF);
        pRxBuffer++;
        Len--;
    }
    while(SPI_GetFlagStatus(pSPIx, SPI_FLAG_BSY) == FLAG_SET);
}

void SPI_TransferData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t Len) {
    while(Len > 0) {
        *pRxBuffer = SPI_ReadWriteByte(pSPIx, *pTxBuffer);
        pTxBuffer++;
        pRxBuffer++;
        Len--;
    }
    while(SPI_GetFlagStatus(pSPIx, SPI_FLAG_BSY) == FLAG_SET);
}
