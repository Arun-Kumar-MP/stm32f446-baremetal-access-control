/*
 * rfid.c
 *
 *  Created on: Oct 4, 2026
 *      Author: Arun Kumar M P
 * 		Description: MFRC522 Bare-Metal RFID Driver Program for STM32F446RE
 */

#include <stdio.h>
#include <stdbool.h>

#include "rfid.h"
#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_spi_driver.h"

/******************************************************************************
 * Hardware Initialization
 ******************************************************************************/
#define MFRC522_CS_PORT    GPIOB
#define MFRC522_CS_PIN     GPIO_PIN_NO_12

#define MFRC522_RST_PORT   GPIOB
#define MFRC522_RST_PIN    GPIO_PIN_NO_1


/******************************************************************************
 * Handles & Internal Helpers
 ******************************************************************************/
static SPI_RegDef_t *g_pSPIx;
static GPIO_RegDef_t *g_csPort;
static uint8_t g_csPin;

static void CS_Low(void)  { GPIO_WriteToOutputPin(g_csPort, g_csPin, GPIO_PIN_RESET); }
static void CS_High(void) { GPIO_WriteToOutputPin(g_csPort, g_csPin, GPIO_PIN_SET); }

static uint8_t SPI_ReadWriteByte_Internal(SPI_RegDef_t *pSPIx, uint8_t txByte) {
    while (pSPIx->SR & SPI_FLAG_RXNE) {
        (void)*(volatile uint8_t *)&pSPIx->DR;
    }
    while (SPI_GetFlagStatus(pSPIx, SPI_FLAG_TXE) == FLAG_RESET);
    *(volatile uint8_t *)&pSPIx->DR = txByte;
    while (SPI_GetFlagStatus(pSPIx, SPI_FLAG_RXNE) == FLAG_RESET);
    return *(volatile uint8_t *)&pSPIx->DR;
}

/******************************************************************************
 * Register & Control APIs
 ******************************************************************************/
void MFRC522_WriteReg(uint8_t addr, uint8_t val) {
    CS_Low();
    SPI_ReadWriteByte_Internal(g_pSPIx, (addr << 1) & 0x7E);
    SPI_ReadWriteByte_Internal(g_pSPIx, val);
    CS_High();
}

uint8_t MFRC522_ReadReg(uint8_t addr) {
    uint8_t val;
    CS_Low();
    SPI_ReadWriteByte_Internal(g_pSPIx, ((addr << 1) & 0x7E) | 0x80);
    val = SPI_ReadWriteByte_Internal(g_pSPIx, 0x00);
    CS_High();
    return val;
}

void MFRC522_Reset(void) {
    MFRC522_WriteReg(CommandReg, PCD_RESETPHASE);
    uint32_t timeout = 10000;
    while ((MFRC522_ReadReg(CommandReg) & 0x0F) && (timeout > 0)) {
        timeout--;
    }
}

void MFRC522_AntennaOn(void) {
    uint8_t temp = MFRC522_ReadReg(TxControlReg);
    if (!(temp & 0x03)) {
        MFRC522_WriteReg(TxControlReg, temp | 0x03);
    }
}

/******************************************************************************
 * Card Communication Engine
 ******************************************************************************/
static uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint32_t *backLen) {
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00, waitIRq = 0x00, lastBits, n;
    uint32_t i;

    if (command == PCD_TRANSCEIVE) {
        irqEn = 0x77;
        waitIRq = 0x30;
    }

    MFRC522_WriteReg(ComIEnReg, irqEn | 0x80);
    MFRC522_WriteReg(ComIrqReg, 0x7F);
    MFRC522_WriteReg(CommandReg, PCD_IDLE);
    MFRC522_WriteReg(FIFOLevelReg, 0x80);

    for (i = 0; i < sendLen; i++) {
        MFRC522_WriteReg(FIFODataReg, sendData[i]);
    }

    MFRC522_WriteReg(CommandReg, command);
    if (command == PCD_TRANSCEIVE) {
        MFRC522_WriteReg(BitFramingReg, MFRC522_ReadReg(BitFramingReg) | 0x80);
    }

    i = 200;
    do {
        n = MFRC522_ReadReg(ComIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

    MFRC522_WriteReg(BitFramingReg, MFRC522_ReadReg(BitFramingReg) & (~0x80));

    if (i != 0) {
        if (!(MFRC522_ReadReg(ErrorReg) & 0x1B)) {
            status = MI_OK;
            if (n & irqEn & 0x01) status = MI_ERR;

            if (command == PCD_TRANSCEIVE) {
                n = MFRC522_ReadReg(FIFOLevelReg);
                lastBits = MFRC522_ReadReg(ControlReg) & 0x07;
                if (lastBits) *backLen = (n - 1) * 8 + lastBits;
                else *backLen = n * 8;

                if (n == 0) n = 1;
                if (n > 16) n = 16;

                for (i = 0; i < n; i++) {
                    backData[i] = MFRC522_ReadReg(FIFODataReg);
                }
            }
        }
    }
    return status;
}

static uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType) {
    uint8_t status;
    uint32_t backBits;

    MFRC522_WriteReg(BitFramingReg, 0x07);
    TagType[0] = reqMode;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

    if ((status != MI_OK) || (backBits != 0x10)) status = MI_ERR;
    return status;
}

static uint8_t MFRC522_Anticoll(uint8_t *serNum) {
    uint8_t status, i, serNumCheck = 0;
    uint32_t unLen;

    MFRC522_WriteReg(BitFramingReg, 0x00);
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_OK) {
        for (i = 0; i < 4; i++) {
            serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i]) status = MI_ERR;
    }
    return status;
}

uint8_t MFRC522_CheckCard(uint8_t *id) {
    uint8_t status = MFRC522_Request(PICC_REQIDL, id);
    if (status == MI_OK) {
        status = MFRC522_Anticoll(id);
    }
    MFRC522_WriteReg(CommandReg, PCD_IDLE);
    return status;
}

void MFRC522_Halt(void) {
    uint32_t unLen;
    uint8_t buff[4];
    buff[0] = PICC_HALT;
    buff[1] = 0x00;
    MFRC522_WriteReg(BitFramingReg, 0x00);
    MFRC522_ToCard(PCD_TRANSCEIVE, buff, 2, buff, &unLen);
    MFRC522_WriteReg(Status2Reg, MFRC522_ReadReg(Status2Reg) & ~(1U << 3));
}


static void Delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) {
        __asm("nop");
    }
}

static void MFRC522_GPIO_Init(void) {
    GPIO_Handle_t gpio;
    GPIOB_PCLK_EN();

    // PB13 (SCK), PB14 (MISO), PB15 (MOSI)
    gpio.pGPIOx = GPIOB;
    gpio.GPIO_PinConfig.PinMode = GPIO_MODE_ALTFN;
    gpio.GPIO_PinConfig.PinAltFunMode = 5;
    gpio.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
    gpio.GPIO_PinConfig.PinSpeed = GPIO_SPEED_FAST;
    gpio.GPIO_PinConfig.PinPuPdControl = GPIO_NO_PUPD;

    gpio.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_13; GPIO_Init(&gpio);
    gpio.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_14; GPIO_Init(&gpio);
    gpio.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_15; GPIO_Init(&gpio);

    // CS Pin (PB12)
    gpio.GPIO_PinConfig.PinMode = GPIO_MODE_OUT;
    gpio.GPIO_PinConfig.PinNumber = MFRC522_CS_PIN;
    GPIO_Init(&gpio);
    CS_High();

    // Reset Pin (PB1)
    gpio.pGPIOx = MFRC522_RST_PORT;
    gpio.GPIO_PinConfig.PinNumber = MFRC522_RST_PIN;
    gpio.GPIO_PinConfig.PinMode = GPIO_MODE_OUT;
    gpio.GPIO_PinConfig.PinSpeed = GPIO_SPEED_SLOW;
    GPIO_Init(&gpio);

    GPIO_WriteToOutputPin(MFRC522_RST_PORT, MFRC522_RST_PIN, GPIO_PIN_RESET);
    Delay_ms(20);
    GPIO_WriteToOutputPin(MFRC522_RST_PORT, MFRC522_RST_PIN, GPIO_PIN_SET);
    Delay_ms(20);
}

static void MFRC522_SPI2_Init(void) {
    SPI_Handle_t hspi2;
    hspi2.pSPIx = SPI2;
    hspi2.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    hspi2.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    hspi2.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
    hspi2.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    hspi2.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    hspi2.SPIConfig.SPI_SSM = SPI_SSM_EN;

    SPI_Init(&hspi2);
    SPI_PeripheralControl(SPI2, ENABLE);
}

void MFRC522_Init(SPI_RegDef_t *pSPIx, GPIO_RegDef_t *csPort, uint8_t csPin) {
    g_pSPIx = pSPIx;
    g_csPort = csPort;
    g_csPin = csPin;

    MFRC522_GPIO_Init();
    MFRC522_SPI2_Init();

    CS_High();

    // 1. Soft Reset
    MFRC522_Reset();

    // 2. Configure Internal Timer (~25ms Timeout for PCD_TRANSCEIVE)
    MFRC522_WriteReg(TModeReg, 0x80);       // TAuto = 1
    MFRC522_WriteReg(TPrescalerReg, 0xA9);  // Prescaler setting
    MFRC522_WriteReg(TReloadRegH, 0x03);    // Timer reload value High
    MFRC522_WriteReg(TReloadRegL, 0xE8);    // Timer reload value Low (1000)

    // 3. Force 100% ASK Modulation (CRITICAL FOR ISO14443A / MIFARE CARDS)
    MFRC522_WriteReg(TxASKReg, 0x40);

    // 4. Configure ISO14443A CRC seed (0x6363)
    MFRC522_WriteReg(ModeReg, 0x3D);
    MFRC522_WriteReg(RxModeReg, 0x00);

    // 5. Boost Receiver Gain to 48 dB (Maximum Sensitivity)
    MFRC522_WriteReg(RFCfgReg, 0x70);

    // 6. Turn Antenna ON
    MFRC522_AntennaOn();
}

/*
 * Test
static bool compareUID(uint8_t *readUID, uint8_t *validUID) {
    for (uint8_t i = 0; i < 4; i++) {
        if (readUID[i] != validUID[i]) return false;
    }
    return true;
}

int main(void) {
    uint8_t card_uid[5];
    uint8_t validUID1[4] = {}; // Keychain
    uint8_t validUID2[4] = {}; // White Card

    printf("\r\n==============================\r\n");
    printf("       RC522 RFID TEST        \r\n");
    printf("==============================\r\n");

    MFRC522_GPIO_Init();
    MFRC522_SPI2_Init();
    MFRC522_Init(SPI2, MFRC522_CS_PORT, MFRC522_CS_PIN);

    uint8_t ver = MFRC522_ReadReg(VersionReg);
    printf("RC522 Version: 0x%02X\r\n", ver);

    if (ver == 0x00 || ver == 0xFF) {
        printf("ERROR: RC522 NOT DETECTED!\r\n");
        while (1);
    }

    printf("RC522 detected successfully!\r\n\r\n");
    printf("Waiting for RFID tag...\r\n\r\n");

    while (1) {
        if (MFRC522_CheckCard(card_uid) == MI_OK) {
            printf("RFID TAG DETECTED: %02X:%02X:%02X:%02X\r\n",
                   card_uid[0], card_uid[1], card_uid[2], card_uid[3]);

            if (compareUID(card_uid, validUID1)) {
                printf("ACCESS GRANTED\r\n");
                printf("TAG: KEYCHAIN\r\n");
            } else if (compareUID(card_uid, validUID2)) {
                printf("ACCESS GRANTED\r\n");
                printf("TAG: WHITE CARD\r\n");
            } else {
                printf("ACCESS DENIED\r\n");
                printf("UNKNOWN TAG\r\n");
            }

            printf("------------------------------\r\n");
            MFRC522_Halt();
            Delay_ms(500);
        }

        Delay_ms(50);
    }

    return 0;
}
 */
