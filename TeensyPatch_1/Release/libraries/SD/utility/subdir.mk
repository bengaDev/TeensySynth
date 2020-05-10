################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility\Sd2Card.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility\SdFile.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility\SdVolume.cpp 

C_SRCS += \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility\KinetisSDHC.c 

C_DEPS += \
.\libraries\SD\utility\KinetisSDHC.c.d 

LINK_OBJ += \
.\libraries\SD\utility\KinetisSDHC.c.o \
.\libraries\SD\utility\Sd2Card.cpp.o \
.\libraries\SD\utility\SdFile.cpp.o \
.\libraries\SD\utility\SdVolume.cpp.o 

CPP_DEPS += \
.\libraries\SD\utility\Sd2Card.cpp.d \
.\libraries\SD\utility\SdFile.cpp.d \
.\libraries\SD\utility\SdVolume.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\SD\utility\KinetisSDHC.c.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility\KinetisSDHC.c
	@echo 'Building file: $<'
	@echo 'Avvio compilazione C'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-gcc" -c -O1 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD  -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -D__MK20DX256__ -DTEENSYDUINO=145 -DARDUINO=10802 -DF_CPU=72000000 -DUSB_MIDI_SERIAL -DLAYOUT_US_ENGLISH   -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\MIDI\src" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SerialFlash" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SPI" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\SD\utility\Sd2Card.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility\Sd2Card.cpp
	@echo 'Building file: $<'
	@echo 'Avvio compilazione C++'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-g++" -c -O1 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -fno-exceptions -felide-constructors -std=gnu++14 -Wno-error=narrowing -fno-rtti -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -D__MK20DX256__ -DTEENSYDUINO=145 -DARDUINO=10802 -DF_CPU=72000000 -DUSB_MIDI_SERIAL -DLAYOUT_US_ENGLISH "-IC:/Teensy_Patches/Workspace_try/TeensyPatch_1/Release/pch"   -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\MIDI\src" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SerialFlash" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SPI" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\SD\utility\SdFile.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility\SdFile.cpp
	@echo 'Building file: $<'
	@echo 'Avvio compilazione C++'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-g++" -c -O1 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -fno-exceptions -felide-constructors -std=gnu++14 -Wno-error=narrowing -fno-rtti -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -D__MK20DX256__ -DTEENSYDUINO=145 -DARDUINO=10802 -DF_CPU=72000000 -DUSB_MIDI_SERIAL -DLAYOUT_US_ENGLISH "-IC:/Teensy_Patches/Workspace_try/TeensyPatch_1/Release/pch"   -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\MIDI\src" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SerialFlash" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SPI" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\SD\utility\SdVolume.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility\SdVolume.cpp
	@echo 'Building file: $<'
	@echo 'Avvio compilazione C++'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-g++" -c -O1 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -fno-exceptions -felide-constructors -std=gnu++14 -Wno-error=narrowing -fno-rtti -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -D__MK20DX256__ -DTEENSYDUINO=145 -DARDUINO=10802 -DF_CPU=72000000 -DUSB_MIDI_SERIAL -DLAYOUT_US_ENGLISH "-IC:/Teensy_Patches/Workspace_try/TeensyPatch_1/Release/pch"   -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\MIDI\src" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SD" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SerialFlash" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SPI" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire\utility" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


