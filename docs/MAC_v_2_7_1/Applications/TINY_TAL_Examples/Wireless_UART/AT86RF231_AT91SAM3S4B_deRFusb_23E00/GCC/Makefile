############################################################################################
# Makefile for the project Wireless_UART Release Using single source files
############################################################################################

# Build specific properties
_TAL_TYPE = AT86RF231
_PAL_TYPE = AT91SAM3S4B
_PAL_GENERIC_TYPE = SAM3
_BOARD_TYPE = deRFusb_23E00
_HIGHEST_STACK_LAYER = TINY_TAL

# available memories on target
MEMORIES = flash sram

# Trace level used for compilation
# (can be overriden by adding TRACE_LEVEL=#number to the command-line)
# TRACE_LEVEL_DEBUG      5
# TRACE_LEVEL_INFO       4
# TRACE_LEVEL_WARNING    3
# TRACE_LEVEL_ERROR      2
# TRACE_LEVEL_FATAL      1
# TRACE_LEVEL_NO_TRACE   0
TRACE_LEVEL = 0

# verbosity level of compilation, uncomment for verbose make console output
SILENT = true

# Path variables
PATH_ROOT = ../../../../..
PATH_APP = ../..
PATH_TINY_TAL = $(PATH_ROOT)/TINY_TAL
PATH_PAL = $(PATH_ROOT)/PAL
PATH_AT91LIB = $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/libraries
PATH_STARTUP = $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Startup
PATH_LDSCRIPTS = $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Linker_scripts

# Project name and output files destinations
PROJECT = Wireless_UART
BIN_DIR = bin
OBJ_DIR = obj
OUTPUT := $(BIN_DIR)/$(PROJECT)


#------------------------------------------------------------------------------
# Toolchain
#------------------------------------------------------------------------------

# Tool suffix when cross-compiling, either 'arm-elf-' or 'arm-none-eabi-'
TOOLCHAIN = arm-none-eabi-
# Compilation tools
CC     = $(TOOLCHAIN)gcc
CP     = $(TOOLCHAIN)objcopy
SIZE   = $(TOOLCHAIN)size
STRIP  = $(TOOLCHAIN)strip

## Options common to compile, link and assembly rules
# General Flags
MCU    = cortex-m3
COMMON = -mcpu=$(MCU) -mthumb

# comment out, if debugging
OPTFLAGS = -O0 -ffunction-sections -mlong-calls

## Compile options common for all C compilation units.
CFLAGS += $(COMMON) 
CFLAGS += $(INCLUDES)
CFLAGS += -Wall -Werror -g -Wundef -std=c99 $(OPTFLAGS)
CFLAGS += -Wno-parentheses
CFLAGS += -DDEBUG=0 -DNOASSERT
CFLAGS += -DTRACE_LEVEL=$(TRACE_LEVEL)
## Stack related flags
CFLAGS += -DUSB0 -DSIO_HUB
CFLAGS += -DTAL_TYPE=$(_TAL_TYPE)
CFLAGS += -DPAL_GENERIC_TYPE=$(_PAL_GENERIC_TYPE)
CFLAGS += -DPAL_TYPE=$(_PAL_TYPE)
CFLAGS += -DVENDOR_BOARDTYPES=1
CFLAGS += -DBOARD_TYPE=$(_BOARD_TYPE)
CFLAGS += -DHIGHEST_STACK_LAYER=$(_HIGHEST_STACK_LAYER)
CFLAGS += -MD -MP -MT $(*F).o -MF $(OBJ_DIR)/$(@F).d
# uncomment for older revision
#CGLAGS += -DREV00

## Assembly specific flags
ASFLAGS =  $(COMMON) -Wall -g $(OPTFLAGS) $(INCLUDES) -D__ASSEMBLY__

## Linker flags
LDFLAGS =  $(COMMON) -g $(OPTFLAGS) -nostartfiles -Wl,--gc-sections

#------------------------------------------------------------------------------
# Include paths, files to build
#------------------------------------------------------------------------------

# Include directories for application
INCLUDES = -I $(PATH_APP)/Inc
# Include directories for general includes
INCLUDES += -I$(PATH_ROOT)/Include/
# Include directories for TAL
INCLUDES += -I$(PATH_ROOT)/TAL/$(_TAL_TYPE)/Inc/
# Include directories for TINY_TAL
INCLUDES += -I$(PATH_ROOT)/TINY_TAL/Inc/
INCLUDES += -I$(PATH_ROOT)/TINY_TAL/$(_TAL_TYPE)/Inc/
# Include directories for PAL
INCLUDES += -I$(PATH_ROOT)/PAL/Inc/
INCLUDES += -I$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/Generic/Inc
INCLUDES += -I$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/
# Include directories for specific boards types
INCLUDES += -I$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)
INCLUDES += -I$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Inc
# AT91Lib 
INCLUDES += -I$(PATH_AT91LIB)/cmsis
INCLUDES += -I$(PATH_AT91LIB)/drivers
INCLUDES += -I$(PATH_AT91LIB)
# USB descriptors 
INCLUDES += -I ./../descriptors

## paths to search for source files
VPATH += $(PATH_APP)/Src
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src
VPATH += $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src
VPATH += $(PATH_TINY_TAL)/Src
VPATH += $(PATH_AT91LIB)/
VPATH += $(PATH_AT91LIB)/cmsis
VPATH += $(PATH_AT91LIB)/drivers/pio
VPATH += $(PATH_AT91LIB)/drivers/pmc
VPATH += $(PATH_AT91LIB)/drivers/spi
VPATH += $(PATH_AT91LIB)/drivers/tc
VPATH += $(PATH_AT91LIB)/drivers/usart
VPATH += $(PATH_AT91LIB)/drivers/usbd
VPATH += $(PATH_AT91LIB)/drivers/utility
VPATH += $(PATH_AT91LIB)/drivers/wdt
VPATH += $(PATH_AT91LIB)/usb/common/core
VPATH += $(PATH_AT91LIB)/usb/device/core
VPATH += $(PATH_AT91LIB)/usb/common/cdc
VPATH += $(PATH_AT91LIB)/usb/device/cdc-serial
VPATH += ./../descriptors
VPATH += $(PATH_STARTUP)

# Objects built from C source files
# main application
C_OBJECTS += main.o
# stack sources
C_OBJECTS += pal_usb.o
C_OBJECTS += pal_sio_hub.o
C_OBJECTS += pal.o
C_OBJECTS += pal_timer.o
C_OBJECTS += pal_trx_access.o
C_OBJECTS += tiny_tal.o
C_OBJECTS += tiny_tal_rx.o
C_OBJECTS += tiny_tal_tx.o
C_OBJECTS += tiny_tal_pib.o
C_OBJECTS += tiny_tal_init.o
C_OBJECTS += tiny_tal_irq_handler.o
C_OBJECTS += tiny_tal_pwr_mgmt.o
C_OBJECTS += tiny_tal_rx_enable.o
# platform dependent
C_OBJECTS += board_cstartup_gnu.o
C_OBJECTS += exceptions.o
C_OBJECTS += pal_board.o
C_OBJECTS += pal_irq.o
# AT91Lib sources
C_OBJECTS += core_cm3.o
C_OBJECTS += pio.o
C_OBJECTS += pmc.o
C_OBJECTS += uart_console.o
C_OBJECTS += stdio.o
C_OBJECTS += led.o
# SAM3S USB LIB objects
C_OBJECTS += USBD.o USBD_HAL.o
C_OBJECTS += CDCDSerialDriver.o
C_OBJECTS += CDCSetControlLineStateRequest.o
C_OBJECTS += CDCLineCoding.o
C_OBJECTS += USBDDriver.o
C_OBJECTS += USBSetAddressRequest.o
C_OBJECTS += USBGenericDescriptor.o
C_OBJECTS += USBInterfaceRequest.o
C_OBJECTS += USBGenericRequest.o
C_OBJECTS += USBGetDescriptorRequest.o
C_OBJECTS += USBSetConfigurationRequest.o
C_OBJECTS += USBFeatureRequest.o
C_OBJECTS += USBEndpointDescriptor.o
C_OBJECTS += USBConfigurationDescriptor.o
C_OBJECTS += CDCDSerialDriverDescriptors.o
# custom USB callbacks
C_OBJECTS += pal_usbd_callbacks.o

# -----------------------------------------------------------------------------
# make targets and -rules
# -----------------------------------------------------------------------------

# include file with flash rules
include $(PATH_ROOT)/Flash_SAM3.in

all: $(BIN_DIR) $(OBJ_DIR) 
#$(MEMORIES)

$(BIN_DIR) $(OBJ_DIR):
	mkdir $@

define RULES

C_OBJECTS_$(1) = $(addprefix $(OBJ_DIR)/$(1)_, $(C_OBJECTS))
ASM_OBJECTS_$(1) = $(addprefix $(OBJ_DIR)/$(1)_, $(ASM_OBJECTS))

$(1): $$(ASM_OBJECTS_$(1)) $$(C_OBJECTS_$(1))
	@echo --------------------------------------------------------------------
	@echo linking
	@echo --------------------------------------------------------------------
	$(CC) $(LDFLAGS) -T$(PATH_LDSCRIPTS)/$$@.lds -Wl,-Map=$(OUTPUT)-$$@.map \
					-o $(OUTPUT)-$$@.elf $$^
	@echo --------------------------------------------------------------------
	@echo creating BIN file
	@echo --------------------------------------------------------------------
	$(CP) -O binary $(OUTPUT)-$$@.elf $(OUTPUT)-$$@.bin
	@echo --------------------------------------------------------------------
	@echo determine file size
	@echo --------------------------------------------------------------------
	@-$(SIZE) $$^ $(OUTPUT)-$$@.elf

$$(C_OBJECTS_$(1)): $(OBJ_DIR)/$(1)_%.o: %.c Makefile $(OBJ_DIR) $(BIN_DIR)
ifeq ($(SILENT),true)
	@echo compiling $$@
	@-$(CC) $(CFLAGS) -D$(1) -c -o $$@ $$<
else
	$(CC) $(CFLAGS) -D$(1) -c -o $$@ $$<	
endif

$$(ASM_OBJECTS_$(1)): $(OBJ_DIR)/$(1)_%.o: %.S Makefile $(OBJ_DIR) $(BIN_DIR)
ifeq ($(SILENT),true)
	@echo compiling $$@
	@-$(CC) $(ASMFLAGS) -D$(1) -c -o $$@ $$<
else
	$(CC) $(ASMFLAGS) -D$(1) -c -o $$@ $$<
endif	

endef

$(foreach MEMORY, $(MEMORIES), $(eval $(call RULES,$(MEMORY))))

clean:
	-rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.lst $(OBJ_DIR)/.d -r $(OBJ_DIR)/.dep
	-rm -f $(BIN_DIR)/*.bin $(BIN_DIR)/*.elf $(BIN_DIR)/*.map
	
