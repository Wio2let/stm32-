################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/source/beep.c \
../lib/source/digitron.c \
../lib/source/key.c \
../lib/source/led.c \
../lib/source/mode.c \
../lib/source/snake.c \
../lib/source/zoord.c 

OBJS += \
./lib/source/beep.o \
./lib/source/digitron.o \
./lib/source/key.o \
./lib/source/led.o \
./lib/source/mode.o \
./lib/source/snake.o \
./lib/source/zoord.o 

C_DEPS += \
./lib/source/beep.d \
./lib/source/digitron.d \
./lib/source/key.d \
./lib/source/led.d \
./lib/source/mode.d \
./lib/source/snake.d \
./lib/source/zoord.d 


# Each subdirectory must supply rules for building sources it contributes
lib/source/%.o lib/source/%.su lib/source/%.cyclo: ../lib/source/%.c lib/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/university/workspace_1.16.0/MiniGame/lib/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib-2f-source

clean-lib-2f-source:
	-$(RM) ./lib/source/beep.cyclo ./lib/source/beep.d ./lib/source/beep.o ./lib/source/beep.su ./lib/source/digitron.cyclo ./lib/source/digitron.d ./lib/source/digitron.o ./lib/source/digitron.su ./lib/source/key.cyclo ./lib/source/key.d ./lib/source/key.o ./lib/source/key.su ./lib/source/led.cyclo ./lib/source/led.d ./lib/source/led.o ./lib/source/led.su ./lib/source/mode.cyclo ./lib/source/mode.d ./lib/source/mode.o ./lib/source/mode.su ./lib/source/snake.cyclo ./lib/source/snake.d ./lib/source/snake.o ./lib/source/snake.su ./lib/source/zoord.cyclo ./lib/source/zoord.d ./lib/source/zoord.o ./lib/source/zoord.su

.PHONY: clean-lib-2f-source

