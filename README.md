# STM32F446RE Bare-Metal Access Control System

A bare-metal embedded C access-control system developed on the **STM32F446RE (ARM Cortex-M4)** without using STM32 HAL libraries or CubeMX-generated peripheral drivers.

The system combines **MFRC522 RFID authentication**, **4×3 keypad PIN authentication**, **SG90 servo-based lock actuation**, and **buzzer feedback**.

---

## Overview

This project was developed from the peripheral level upward, using custom register-level drivers for the STM32F446RE.

The firmware includes:

- Custom GPIO driver
- Custom SPI driver
- Custom Timer/PWM driver
- MFRC522 RFID driver
- 4×3 matrix keypad driver
- SG90 servo control
- Buzzer control
- RFID UID authentication
- Keypad PIN authentication
- Access granted / denied handling

The final application runs as a simple polling-based embedded super-loop.

---

## Hardware

| Component | Purpose |
| --- | --- |
| NUCLEO-F446RE | Main development board |
| STM32F446RE | ARM Cortex-M4 MCU |
| MFRC522 v1.3 | RFID authentication |
| 4×3 Matrix Keypad | PIN authentication |
| SG90 Servo | Lock / unlock mechanism |
| Active Buzzer | Access feedback |

---

## Hardware Pinout

### MFRC522

| MFRC522 | STM32F446RE | Configuration |
| --- | --- | --- |
| SDA / SS | `PB12` | GPIO Output |
| SCK | `PB13` | AF5 – SPI2_SCK |
| MISO | `PB14` | AF5 – SPI2_MISO |
| MOSI | `PB15` | AF5 – SPI2_MOSI |
| RST | `PB1` | GPIO Output |
| 3.3V | `3V3` | Power |
| GND | `GND` | Ground |

### Keypad

| Keypad | STM32F446RE | Configuration |
| --- | --- | --- |
| Rows 1–4 | `PC0–PC3` | GPIO Output |
| Columns 1–3 | `PC4–PC6` | GPIO Input + Pull-Up |

### Servo

| Signal | STM32F446RE | Configuration |
| --- | --- | --- |
| PWM | `PB4` | AF2 – TIM3_CH1 |

### Buzzer

| Signal | STM32F446RE | Configuration |
| --- | --- | --- |
| Control | `PC8` | GPIO Output |

---

## System Architecture

```text
                    STM32F446RE
                   ARM Cortex-M4
                        |
        +---------------+---------------+
        |               |               |
       SPI2            GPIO            TIM3
        |               |               |
     MFRC522          Keypad           Servo
        |               |
     RFID UID         Buzzer
```

---

## Firmware Architecture

The firmware is organized into three logical layers.

### Application Layer

```text
smart_lock.c
```

Responsible for:

- Authentication flow
- Credential validation
- Access granted / denied handling
- Main application super-loop

### Device Driver Layer

```text
rfid.c
keypad.c
servo.c
buzzer.c
```

Responsible for interfacing with the connected external hardware.

### MCU Driver Layer

```text
stm32f446xx_gpio_driver.c
stm32f446xx_spi_driver.c
stm32f446xx_timer_driver.c
```

Responsible for register-level configuration and control of STM32 peripherals.

---

## Firmware Structure

```text
001SmartDoorLock/
│
├── Drivers/
│   ├── Inc/
│   │   ├── stm32f446xx.h
│   │   ├── stm32f446xx_gpio_driver.h
│   │   ├── stm32f446xx_spi_driver.h
│   │   └── stm32f446xx_timer_driver.h
│   │
│   └── Src/
│       ├── stm32f446xx_gpio_driver.c
│       ├── stm32f446xx_spi_driver.c
│       └── stm32f446xx_timer_driver.c
│
├── Inc/
│   ├── buzzer.h
│   ├── keypad.h
│   ├── rfid.h
│   └── servo.h
│
├── Src/
│   ├── buzzer.c
│   ├── keypad.c
│   ├── rfid.c
│   ├── servo.c
│   ├── smart_lock.c
│   ├── syscalls.c
│   └── sysmem.c
│
├── Startup/
│
├── STM32F446RETX_FLASH.ld
├── STM32F446RETX_RAM.ld
├── .cproject
├── .project
├── .gitignore
└── README.md
```

---

## Authentication

The system supports two authentication methods.

### RFID

Authorized RFID UIDs currently configured in the application:

```text
F1:91:CC:5C
8A:ED:92:02
```

A detected UID is compared against the configured authorized UIDs.

### Keypad

The system uses a 4-digit PIN:

```text
5555
```

Controls:

```text
*  → Clear PIN
#  → Submit PIN
```

Both authentication methods use the same access-control response.

---

## Access Control

### Access Granted

```text
Authentication Successful
        ↓
Buzzer confirmation
        ↓
Servo → 90°
        ↓
Door remains unlocked
        ↓
Servo → 0°
        ↓
Door locked
```

### Access Denied

```text
Authentication Failed
        ↓
Warning beep
        ↓
Second warning beep
        ↓
System remains locked
```

---

## Key Technical Highlights

### Bare-Metal STM32 Development

- Direct register-level peripheral configuration
- Custom STM32F446RE register definitions
- No STM32 HAL peripheral drivers
- No CubeMX-generated peripheral initialization

### Custom Peripheral Drivers

- GPIO driver
- SPI driver
- Timer/PWM driver
- GPIO interrupt / EXTI support

### Device Drivers

- MFRC522 RFID
- 4×3 matrix keypad
- SG90 servo
- Active buzzer

### Embedded Concepts

- Memory-mapped peripheral registers
- GPIO alternate functions
- SPI master communication
- SPI status flags
- Timer/PWM generation
- GPIO polling
- EXTI/NVIC configuration
- Modular embedded firmware architecture

---

## Build & Flash

### Requirements

- STM32CubeIDE
- NUCLEO-F446RE
- ST-LINK
- Required project hardware

### Build

Import the project into STM32CubeIDE as an existing project and build it using the configured GCC toolchain.

### Flash

Connect the NUCLEO-F446RE through USB and program the MCU using the onboard ST-LINK debugger/programmer.

---

## Example Output

```text
========================================
        SMART DOOR LOCK SYSTEM
========================================
Status: LOCKED | Ready (RFID / Keypad)


[RFID] Tag Read: 8A:ED:92:02

[+] ACCESS GRANTED | Unlocking...

[*] DOOR LOCKED


[KEYPAD] Verifying PIN...

[-] ACCESS DENIED | Invalid Credential
```

---

## Development Approach

The firmware was developed incrementally, validating each peripheral before integrating the complete system:

```text
STM32F446RE Register Definitions
            ↓
Custom GPIO Driver
            ↓
Keypad / Buzzer
            ↓
Timer / PWM Driver
            ↓
Servo Control
            ↓
Custom SPI Driver
            ↓
MFRC522 RFID Driver
            ↓
Authentication Logic
            ↓
Complete Access-Control System
```

---

## Future Improvements

- Replace blocking delays with a non-blocking state-machine architecture
- Add FreeRTOS-based task architecture
- Store credentials in non-volatile memory
- Add configurable RFID users and PINs
- Add failed-attempt lockout
- Add UART-based configuration and diagnostics
