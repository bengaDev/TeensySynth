################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\Audio\utility\sqrt_integer.c 

C_DEPS += \
.\libraries\Audio\utility\sqrt_integer.c.d 

LINK_OBJ += \
.\libraries\Audio\utility\sqrt_integer.c.o 


# Each subdirectory must supply rules for building sources it contributes
libraries\Audio\utility\sqrt_integer.c.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\Audio\utility\sqrt_integer.c
	@echo 'Building file: $<'
	@echo 'Avvio compilazione C'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-gcc" -c -O1 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD  -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -D__MK20DX256__ -DTEENSYDUINO=145 -DARDUINO=10802 -DF_CPU=72000000 -DUSB_MIDI_SERIAL -DLAYOUT_US_ENGLISH   -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\MIDI\src" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SerialFlash" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SPI" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


