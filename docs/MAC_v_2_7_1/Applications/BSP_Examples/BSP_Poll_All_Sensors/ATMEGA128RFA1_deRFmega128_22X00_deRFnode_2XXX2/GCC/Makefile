############################################################################################
# Makefile for the project BSP_Poll_All_Sensors using single source files
############################################################################################

# Build specific properties
_PAL_TYPE = ATMEGA128RFA1
_PAL_GENERIC_TYPE = MEGA_RF
_BOARD_TYPE = deRFmega128_22X00_deRFnode_2XXX2
_HIGHEST_STACK_LAYER = PAL

# Path variables
## Path to main project directory
PATH_ROOT = ../../../../..
PATH_APP = ../..
PATH_PAL = $(PATH_ROOT)/PAL
PATH_SIO_SUPPORT = $(PATH_ROOT)/Applications/Helper_Files/SIO_Support
PATH_BSP = $(PATH_ROOT)/BSP

## General Flags
PROJECT = BSP_Poll_All_Sensors
MCU = atmega128rfa1
TARGET_DIR = .
TARGET = $(TARGET_DIR)/$(PROJECT).elf
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -g -Wundef -std=c99 -Os
CFLAGS += -DSIO_HUB -DUSB0
CFLAGS += -DDEBUG=0
CFLAGS += -DPAL_GENERIC_TYPE=$(_PAL_GENERIC_TYPE)
CFLAGS += -DPAL_TYPE=$(_PAL_TYPE)
CFLAGS += -DVENDOR_BOARDTYPES=1
CFLAGS += -DBOARD_TYPE=$(_BOARD_TYPE)
CFLAGS += -DHIGHEST_STACK_LAYER=$(_HIGHEST_STACK_LAYER)
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d
CFLAGS += --function-sections

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-g

## Linker flags
LDFLAGS = $(COMMON) -Wl,-Map=$(PROJECT).map -Wl,--section-start=.data=0x800200

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Include directories for application
INCLUDES = -I $(PATH_APP)/Inc
# Include directories for SIO support
INCLUDES += -I $(PATH_SIO_SUPPORT)/Inc
## Include directories for general includes
INCLUDES += -I $(PATH_ROOT)/Include
## Include directories for PAL
INCLUDES += -I $(PATH_ROOT)/PAL/Inc/
INCLUDES += -I $(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/Generic/Inc
INCLUDES += -I $(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Inc/
## Include directories for specific boards type
INCLUDES += -I $(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/
INCLUDES += -I $(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)
## Include directories for BSP
INCLUDES += -I $(PATH_BSP)/Inc
INCLUDES += -I $(PATH_BSP)/deRFnodeGateway/Boards/$(_BOARD_TYPE)
INCLUDES += -I $(PATH_BSP)/deRFnodeGateway/Generic/Driver/Inc
INCLUDES += -I $(PATH_BSP)/deRFnodeGateway/Generic/Libraries/$(_PAL_GENERIC_TYPE)/TWI
INCLUDES += -I $(PATH_BSP)/deRFnodeGateway/Generic/Libraries/$(_PAL_GENERIC_TYPE)/SPI-Flash

## Objects that must be built in order to link

OBJECTS += $(TARGET_DIR)/main.o\
	$(TARGET_DIR)/sio_handler.o\
	$(TARGET_DIR)/pal_sio_hub.o\
	$(TARGET_DIR)/pal_uart.o\
	$(TARGET_DIR)/pal_usb_ftdi.o\
	$(TARGET_DIR)/pal_irq.o\
	$(TARGET_DIR)/pal.o\
	$(TARGET_DIR)/pal_mcu_generic.o\
	$(TARGET_DIR)/pal_timer.o\
	$(TARGET_DIR)/pal_board.o\
	$(TARGET_DIR)/pal_utils.o\
	$(TARGET_DIR)/bsp.o\
	$(TARGET_DIR)/twi_master.o\
	$(TARGET_DIR)/sensors.o\
	$(TARGET_DIR)/vbatmon.o\
	$(TARGET_DIR)/flash.o\
	$(TARGET_DIR)/spid.o
	

## Objects explicitly added by the user
LINKONLYOBJECTS =

## Build
all: $(TARGET) $(TARGET_DIR)/$(PROJECT).hex $(TARGET_DIR)/$(PROJECT).eep $(TARGET_DIR)/$(PROJECT).lss size

## Compile
$(TARGET_DIR)/main.o: $(PATH_APP)/Src/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  -o $@ $<
$(TARGET_DIR)/sio_handler.o: $(PATH_SIO_SUPPORT)/Src/sio_handler.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  -o $@ $<
$(TARGET_DIR)/pal_sio_hub.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src/pal_sio_hub.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_uart.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src/pal_uart.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_usb_ftdi.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)/pal_usb_ftdi.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_irq.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src/pal_irq.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src/pal.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_mcu_generic.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src/pal_mcu_generic.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_timer.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src/pal_timer.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_board.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)/pal_board.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_utils.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src/pal_utils.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

$(TARGET_DIR)/bsp.o: $(PATH_BSP)/deRFnodeGateway/Generic/Src/bsp.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/twi_master.o: $(PATH_BSP)/deRFnodeGateway/Generic/Libraries/$(_PAL_GENERIC_TYPE)/TWI/twi_master.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/sensors.o: $(PATH_BSP)/deRFnodeGateway/Generic/Driver/Src/$(_PAL_GENERIC_TYPE)/sensors.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<	
$(TARGET_DIR)/vbatmon.o: $(PATH_BSP)/deRFnodeGateway/Generic/Driver/Src/$(_PAL_GENERIC_TYPE)/vbatmon.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<	
$(TARGET_DIR)/spid.o: $(PATH_BSP)/deRFnodeGateway/Generic/Libraries/$(_PAL_GENERIC_TYPE)/SPI-Flash/spid.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/flash.o: $(PATH_BSP)/deRFnodeGateway/Generic/Driver/Src/$(_PAL_GENERIC_TYPE)/flash.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<	

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## avr-size options
IS_WIN32 := $(shell uname -s | sed -n -e 's/^MINGW.*/-C/p' -e 's/^CYGWIN.*/-C/p')
ifdef IS_WIN32
SIZEFLAGS = -C --mcu=${MCU}
else
SIZEFLAGS = -B
endif

size: ${TARGET}
	@echo
	@avr-size $(SIZEFLAGS) ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(TARGET_DIR)/*.o $(TARGET_DIR)/$(PROJECT).elf dep/* $(TARGET_DIR)/$(PROJECT).hex $(TARGET_DIR)/$(PROJECT).eep $(TARGET_DIR)/$(PROJECT).lss $(TARGET_DIR)/$(PROJECT).map

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


