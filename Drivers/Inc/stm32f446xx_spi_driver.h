/*
 * stm32f446xx_spi_driver.h
 *
 *  Created on: Oct 4, 2026
 *      Author: Arun Kumar M P
 *      Description: SPI driver interface for STM32F446RE
 */

#ifndef INC_STM32F446XX_SPI_DRIVER_H_
#define INC_STM32F446XX_SPI_DRIVER_H_

#include "stm32f446xx.h"

/******************************************************************************
 * SPI Configuration Structure
 ******************************************************************************/
typedef struct{
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_CPHA;
    uint8_t SPI_CPOL;
    uint8_t SPI_SSM;

} SPI_Config_t;


/******************************************************************************
 * SPI Handle Structure
 ******************************************************************************/
typedef struct{
    SPI_RegDef_t *pSPIx;
    SPI_Config_t SPIConfig;

} SPI_Handle_t;

/************************************************
 * SPI Definitions
 ************************************************/
#define SPI_CR1_CPHA        			 0U
#define SPI_CR1_CPOL        			 1U
#define SPI_CR1_MSTR        			 2U
#define SPI_CR1_BR          			 3U
#define SPI_CR1_SPE         			 6U
#define SPI_CR1_LSBFIRST    			 7U
#define SPI_CR1_SSI         			 8U
#define SPI_CR1_SSM         			 9U
#define SPI_CR1_RXONLY     				10U
#define SPI_CR1_DFF        				11U
#define SPI_CR1_CRCNEXT    				12U
#define SPI_CR1_CRCEN      				13U
#define SPI_CR1_BIDIOE     				14U
#define SPI_CR1_BIDIMODE   				15U

#define SPI_DEVICE_MODE_MASTER 			1U
#define SPI_DEVICE_MODE_SLAVE  			0U

#define SPI_BUS_CONFIG_FD      			1U
#define SPI_BUS_CONFIG_HD      			2U
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY	3U

#define SPI_SCLK_SPEED_DIV2     		0U
#define SPI_SCLK_SPEED_DIV4     		1U
#define SPI_SCLK_SPEED_DIV8     		2U
#define SPI_SCLK_SPEED_DIV16    		3U
#define SPI_SCLK_SPEED_DIV32    		4U
#define SPI_SCLK_SPEED_DIV64    		5U
#define SPI_SCLK_SPEED_DIV128   		6U
#define SPI_SCLK_SPEED_DIV256   		7U

#define SPI_CPHA_LOW   					0U
#define SPI_CPHA_HIGH  					1U

#define SPI_CPOL_LOW   					0U
#define SPI_CPOL_HIGH  					1U

#define SPI_SSM_DI  					0U
#define SPI_SSM_EN  					1U

#define SPI_SR_RXNE   					0U
#define SPI_SR_TXE    					1U
#define SPI_SR_BSY    					7U

#define SPI_FLAG_RXNE       (1U << SPI_SR_RXNE)
#define SPI_FLAG_TXE        (1U << SPI_SR_TXE)
#define SPI_FLAG_BSY        (1U << SPI_SR_BSY)

/********************************************************************************
 * SPI Peripheral APIs
 ********************************************************************************/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint8_t FlagName);
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReadWriteByte(SPI_RegDef_t *pSPIx, uint8_t txByte);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);
void SPI_TransferData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t Len);

#endif /* INC_STM32F446XX_SPI_DRIVER_H_ */
