/*
 * rfid.h
 *
 *  Created on: Oct 4, 2026
 *      Author: Arun Kumar M P
 * 		Description: MFRC522 Bare-Metal RFID Driver Header for STM32F446RE
 */

#ifndef RC522_H_
#define RC522_H_

#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_spi_driver.h"


// MFRC522 Commands
#define PCD_IDLE              0x00
#define PCD_TRANSCEIVE        0x0C
#define PCD_RESETPHASE        0x0F


// PICC Commands
#define PICC_REQIDL           0x26
#define PICC_ANTICOLL         0x93
#define PICC_HALT             0x50


// Registers
#define CommandReg            0x01
#define ComIEnReg             0x02
#define ComIrqReg             0x04
#define ErrorReg              0x06
#define Status2Reg            0x08
#define FIFODataReg           0x09
#define FIFOLevelReg          0x0A
#define ControlReg            0x0C
#define BitFramingReg         0x0D
#define ModeReg               0x11
#define RxModeReg             0x13
#define TxControlReg          0x14
#define TxASKReg              0x15
#define RFCfgReg              0x26
#define TModeReg              0x2A
#define TPrescalerReg         0x2B
#define TReloadRegH           0x2C
#define TReloadRegL           0x2D
#define VersionReg            0x37


// Return Status
#define MI_OK                 0
#define MI_ERR                1


// RFID Driver APIs
void MFRC522_WriteReg(uint8_t addr, uint8_t val);
uint8_t MFRC522_ReadReg(uint8_t addr);

void MFRC522_Reset(void);
void MFRC522_AntennaOn(void);

void MFRC522_Init(SPI_RegDef_t *pSPIx, GPIO_RegDef_t *csPort, uint8_t csPin);

uint8_t MFRC522_CheckCard(uint8_t *id);

void MFRC522_Halt(void);


#endif /* RC522_H_ */
