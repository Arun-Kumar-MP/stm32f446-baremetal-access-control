################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/buzzer.c \
../Src/keypad.c \
../Src/rfid.c \
../Src/servo.c \
../Src/smart_lock.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/buzzer.o \
./Src/keypad.o \
./Src/rfid.o \
./Src/servo.o \
./Src/smart_lock.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/buzzer.d \
./Src/keypad.d \
./Src/rfid.d \
./Src/servo.d \
./Src/smart_lock.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"D:/Embedded C/Projects/Drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/buzzer.cyclo ./Src/buzzer.d ./Src/buzzer.o ./Src/buzzer.su ./Src/keypad.cyclo ./Src/keypad.d ./Src/keypad.o ./Src/keypad.su ./Src/rfid.cyclo ./Src/rfid.d ./Src/rfid.o ./Src/rfid.su ./Src/servo.cyclo ./Src/servo.d ./Src/servo.o ./Src/servo.su ./Src/smart_lock.cyclo ./Src/smart_lock.d ./Src/smart_lock.o ./Src/smart_lock.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

