###############################################################################
# Makefile for the project ControlParqueadero644
###############################################################################

## General Flags
PROJECT = ControlParqueadero644
MCU = atmega644
TARGET = ControlParqueadero644.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=ControlParqueadero644.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = lcd_HD44780_4.o USART.o SPI.o SDCard.o ControlParqueadero644.o I2C.o DS1307.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) ControlParqueadero644.hex ControlParqueadero644.eep ControlParqueadero644.lss size

## Compile
lcd_HD44780_4.o: ../includes/lcd_HD44780_4.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

USART.o: ../includes/USART.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SPI.o: ../includes/SPI.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SDCard.o: ../includes/SDCard.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ControlParqueadero644.o: ../ControlParqueadero644.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

I2C.o: ../includes/I2C.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

DS1307.o: ../includes/DS1307.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) ControlParqueadero644.elf dep/* ControlParqueadero644.hex ControlParqueadero644.eep ControlParqueadero644.lss ControlParqueadero644.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

