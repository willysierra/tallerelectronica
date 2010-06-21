###############################################################################
# Makefile for the project ControlDiplayExterno
###############################################################################

## General Flags
PROJECT = ControlDiplayExterno
MCU = atmega168
TARGET = ControlDiplayExterno.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99   -O3 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=ControlDiplayExterno.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Users\Manuel Vieda\Documents\tallerelectronica\AVR\ControlDiplayExterno\." 

## Objects that must be built in order to link
OBJECTS = ControlDiplayExterno.o USART.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) ControlDiplayExterno.hex ControlDiplayExterno.eep ControlDiplayExterno.lss size

## Compile
ControlDiplayExterno.o: ../ControlDiplayExterno.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

USART.o: ../includes/USART.c
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
	-rm -rf $(OBJECTS) ControlDiplayExterno.elf dep/* ControlDiplayExterno.hex ControlDiplayExterno.eep ControlDiplayExterno.lss ControlDiplayExterno.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

