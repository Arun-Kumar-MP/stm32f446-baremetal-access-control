/*
 * smart_lock.c
 *
 *  Created on: Oct 5, 2026
 *      Author: Arun Kumar M P
 * 		Description: Smart Door Lock System (RFID + Keypad + Servo + Buzzer)
 */

#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"
#include "rfid.h"
#include "keypad.h"
#include "servo.h"
#include "buzzer.h"

/* ---------------- Configuration ---------------- */

#define LOCK_ANGLE             0U
#define UNLOCK_ANGLE           90U
#define PIN_LENGTH             4U

static const uint8_t VALID_UID_1[4] = {0xF1, 0x91, 0xCC, 0x5C}; // Keychain
static const uint8_t VALID_UID_2[4] = {0x8A, 0xED, 0x92, 0x02}; // White Card
static const char VALID_PIN[PIN_LENGTH + 1] = "5555";

/* ---------------- Delays ---------------- */

static void Delay_Short(void) {
    for (volatile uint32_t i = 0; i < 50000; i++); // ~15ms fast debounce
}

static void Delay_Long(void) {
    for (volatile uint32_t i = 0; i < 500000; i++); // ~150ms state delay
}

/* ---------------- Utility Helpers ---------------- */

static uint8_t CompareUID(uint8_t *uid, const uint8_t *valid) {
    for (uint8_t i = 0; i < 4; i++) {
        if (uid[i] != valid[i]) return 0;
    }
    return 1;
}

static uint8_t IsValidUID(uint8_t *uid) {
    return (CompareUID(uid, VALID_UID_1) || CompareUID(uid, VALID_UID_2));
}

/* ---------------- Access Control ---------------- */

static void AccessGranted(void) {
    printf("\r\n[+] ACCESS GRANTED | Unlocking...\r\n");

    Buzzer_On();
    Delay_Short();
    Buzzer_Off();

    Servo_SetAngle(UNLOCK_ANGLE);

    // Hold unlocked state
    Delay_Long();
    Delay_Long();
    Delay_Long();

    Servo_SetAngle(LOCK_ANGLE);
    printf("[*] DOOR LOCKED\r\n\r\n");
}

static void AccessDenied(void) {
    printf("\r\n[-] ACCESS DENIED | Invalid Credential\r\n");

    // Double warning beep
    Buzzer_On(); Delay_Short(); Buzzer_Off();
    Delay_Short();
    Buzzer_On(); Delay_Short(); Buzzer_Off();
    printf("\r\n");
}

/* ---------------- RFID Task ---------------- */

static void RFID_Authentication(void) {
    uint8_t card_uid[5];

    if (MFRC522_CheckCard(card_uid) == MI_OK) {
        printf("\r\n[RFID] Tag Read: %02X:%02X:%02X:%02X\r\n",
               card_uid[0], card_uid[1], card_uid[2], card_uid[3]);

        if (IsValidUID(card_uid)) {
            AccessGranted();
        } else {
            AccessDenied();
        }

        MFRC522_Halt();
        Delay_Long(); // Prevent duplicate rapid scans
    }
}

/* ---------------- Keypad Task ---------------- */

static void Keypad_Authentication(void) {
    static char enteredPIN[PIN_LENGTH + 1];
    static uint8_t pinIndex = 0;
    char key = Keypad_GetKey();

    if (key == '\0') return;

    /* Clear PIN (* key) */
    if (key == '*') {
        pinIndex = 0;
        enteredPIN[0] = '\0';
        printf("\r\n[KEYPAD] PIN Cleared\r\n");
        Delay_Short();
        return;
    }

    /* Submit PIN (# key) */
    if (key == '#') {
        if (pinIndex == PIN_LENGTH) {
            enteredPIN[PIN_LENGTH] = '\0';
            printf("\r\n[KEYPAD] Verifying PIN...\r\n");

            uint8_t valid = 1;
            for (uint8_t i = 0; i < PIN_LENGTH; i++) {
                if (enteredPIN[i] != VALID_PIN[i]) {
                    valid = 0;
                    break;
                }
            }

            if (valid) AccessGranted();
            else AccessDenied();
        } else {
            printf("\r\n[KEYPAD] Error: 4 digits required\r\n");
            AccessDenied();
        }

        pinIndex = 0;
        enteredPIN[0] = '\0';
        return;
    }

    /* Store Numeric Key ('0' - '9') */
    if (key >= '0' && key <= '9') {
        if (pinIndex < PIN_LENGTH) {
            enteredPIN[pinIndex++] = key;
            printf("*");
            fflush(stdout);
            Delay_Short(); // Fast, responsive debounce
        }
    }
}

/* ---------------- Main Execution ---------------- */

int main(void) {
    // Peripheral Initializations
    Keypad_Init();
    Buzzer_Init();
    Servo_Init();
    MFRC522_Init(SPI2, GPIOB, GPIO_PIN_NO_12);

    // Initial State
    Servo_SetAngle(LOCK_ANGLE);
    Buzzer_Off();

    printf("\r\n========================================\r\n");
    printf("        SMART DOOR LOCK SYSTEM          \r\n");
    printf("========================================\r\n");
    printf("Status: LOCKED | Ready (RFID / Keypad)\r\n\r\n");

    // Super-Loop
    while (1) {
        RFID_Authentication();
        Keypad_Authentication();
    }

    return 0;
}
