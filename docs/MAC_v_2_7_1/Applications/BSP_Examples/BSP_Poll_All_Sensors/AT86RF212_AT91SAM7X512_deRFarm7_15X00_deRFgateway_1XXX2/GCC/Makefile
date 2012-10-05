############################################################################################
# Makefile for the project BSP_Poll_All_Sensors using single source files
############################################################################################

# Build specific properties
_PAL_TYPE = AT91SAM7X512
_PAL_GENERIC_TYPE = ARM7
_BOARD_TYPE = deRFarm7_15X00_deRFgateway_1XXX2
_HIGHEST_STACK_LAYER = PAL

# available memories on target 
MEMORIES = flash

## Trace level used
# (can be overriden by adding TRACE_LEVEL=#number to the command-line)
# TRACE_LEVEL_DEBUG      5
# TRACE_LEVEL_INFO       4
# TRACE_LEVEL_WARNING    3
# TRACE_LEVEL_ERROR      2
# TRACE_LEVEL_FATAL      1
# TRACE_LEVEL_NO_TRACE   0
TRACE_LEVEL = 0
DYNTRACE = 0

# verbosity level of compilation, uncomment for verbose make console output
SILENT = true

# Path variables
PATH_ROOT = ../../../../..
PATH_APP = ../..
PATH_PAL = $(PATH_ROOT)/PAL
PATH_SIO_SUPPORT = $(PATH_ROOT)/Applications/Helper_Files/SIO_Support
PATH_AT91LIB = $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/generic/at91lib
PATH_STARTUP = $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Startup
PATH_LDSCRIPTS = $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Linker_scripts
PATH_BSP = $(PATH_ROOT)/BSP

# Project name and output files destinations
PROJECT = BSP_Poll_All_Sensors
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
MCU    = arm7tdmi
COMMON = -mcpu=$(MCU)

# comment out, if debugging / USB interface used
OPTFLAGS = -O0

## Compile options common for all C compilation units.
CFLAGS += $(COMMON) 
CFLAGS += $(INCLUDES)
CFLAGS += -Wall -g -Wundef -std=c99 $(OPTFLAGS)
CFLAGS += -Wno-parentheses
CFLAGS += -DDEBUG=0 -DNOASSERT
#UART0 accessible via IO-Header, UART1 not routed
#DBGU available, if SIO_HUB _not_ defined 
CFLAGS += -DSIO_HUB -DUSB0
CFLAGS += -DPAL_GENERIC_TYPE=$(_PAL_GENERIC_TYPE)
CFLAGS += -DPAL_TYPE=$(_PAL_TYPE)
CFLAGS += -DVENDOR_BOARDTYPES=1
CFLAGS += -DBOARD_TYPE=$(_BOARD_TYPE)
CFLAGS += -DHIGHEST_STACK_LAYER=$(_HIGHEST_STACK_LAYER)
CFLAGS += -MD -MP -MT $(*F).o -MF $(OBJ_DIR)/$(@F).d
# AT91-specific
CFLAGS += -mthumb-interwork  
CFLAGS += -fomit-frame-pointer
CFLAGS += -Wstrict-prototypes
CFLAGS += -mlong-calls
CFLAGS += -ffunction-sections
CFLAGS += -DTRACE_LEVEL=$(TRACE_LEVEL)
CFLAGS += -DDYNTRACE=$(DYNTRACE)

## Assembly specific flags
ASMFLAGS += $(COMMON) 
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-g
ASMFLAGS += -g -gdwarf-2
ASMFLAGS += -D__ASSEMBLY__

## Linker flags
LDFLAGS += $(COMMON) -g $(OPTFLAGS) -nostartfiles -Wl,--gc-sections
LDFLAGS += -Wl,--cref


#------------------------------------------------------------------------------
# Include paths, files to build
#------------------------------------------------------------------------------

# Include directories for application
INCLUDES = -I $(PATH_APP)/Inc
# Include directories for SIO support
INCLUDES += -I $(PATH_SIO_SUPPORT)/Inc
# Include directories for general includes
INCLUDES += -I $(PATH_ROOT)/Include/
# Include directories for PAL
INCLUDES += -I$(PATH_ROOT)/PAL/Inc/
INCLUDES += -I$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/Generic/Inc
INCLUDES += -I$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/
# Include directories for specific boards types
INCLUDES += -I$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)
INCLUDES += -I$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Inc
# AT91Lib 
INCLUDES += -I$(PATH_AT91LIB)/peripherals 
INCLUDES += -I$(PATH_AT91LIB)/components
INCLUDES += -I$(PATH_AT91LIB)/usb/device
INCLUDES += -I$(PATH_AT91LIB)/usb/device/cdcdserial
INCLUDES += -I$(PATH_AT91LIB)/utility
INCLUDES += -I$(PATH_AT91LIB)
# USB descriptors 
INCLUDES += -I ./../descriptors
# Include directories for BSP
INCLUDES += -I $(PATH_BSP)/Inc
INCLUDES += -I $(PATH_BSP)/deRFnodeGateway/Boards/$(_BOARD_TYPE)
INCLUDES += -I $(PATH_BSP)/deRFnodeGateway/Generic/Driver/Inc
INCLUDES += -I $(PATH_BSP)/deRFnodeGateway/Generic/Libraries/$(_PAL_GENERIC_TYPE)/TWI
INCLUDES += -I $(PATH_BSP)/deRFnodeGateway/Generic/Libraries/$(_PAL_GENERIC_TYPE)/SPI-Flash

## paths to search for source files
VPATH += $(PATH_APP)/Src
VPATH += $(PATH_SIO_SUPPORT)/Src
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)
VPATH += $(PATH_AT91LIB)/utility
VPATH += $(PATH_AT91LIB)/peripherals
VPATH += $(PATH_AT91LIB)/peripherals/aic
VPATH += $(PATH_AT91LIB)/peripherals/dbgu
VPATH += $(PATH_AT91LIB)/peripherals/usart
VPATH += $(PATH_AT91LIB)/peripherals/pio
VPATH += $(PATH_AT91LIB)/peripherals/pmc
VPATH += $(PATH_AT91LIB)/usb/device/cdc-serial
VPATH += $(PATH_AT91LIB)/usb/device/core/
VPATH += $(PATH_AT91LIB)/usb/common/core
VPATH += $(PATH_AT91LIB)/usb/common/cdc
VPATH += ./../descriptors
VPATH += $(PATH_BSP)/deRFnodeGateway/Generic/Src/
VPATH += $(PATH_BSP)/deRFnodeGateway/Generic/Driver/Src/$(_PAL_GENERIC_TYPE)/
VPATH += $(PATH_BSP)/deRFnodeGateway/Generic/Libraries/$(_PAL_GENERIC_TYPE)/TWI/
VPATH += $(PATH_BSP)/deRFnodeGateway/Generic/Libraries/$(_PAL_GENERIC_TYPE)/SPI-Flash/
VPATH += $(PATH_AT91LIB)/peripherals/adc
VPATH += $(PATH_STARTUP)

## Objects built from C source files
# main application
C_OBJECTS += main.o
# stack sources
C_OBJECTS += sio_handler.o
C_OBJECTS += pal_uart.o
C_OBJECTS += pal_usb.o
C_OBJECTS += pal_sio_hub.o
C_OBJECTS += pal_usbd_callbacks.o
C_OBJECTS += pal.o
C_OBJECTS += pal_timer.o
C_OBJECTS += pal_utils.o
C_OBJECTS += pal_trx_access.o
C_OBJECTS += pal_irq.o
C_OBJECTS += pal_board.o
# AT91Lib sources
C_OBJECTS += led.o
C_OBJECTS += string.o
C_OBJECTS += stdio.o
C_OBJECTS += aic.o
C_OBJECTS += dbgu.o
C_OBJECTS += usart.o
C_OBJECTS += pio.o
C_OBJECTS += pio_it.o
C_OBJECTS += board_memories.o
# AT91Lib USB support
C_OBJECTS += CDCDSerialDriver.o
C_OBJECTS += USBD_OTGHS.o
C_OBJECTS += USBD_UDP.o
C_OBJECTS += USBD_UDPHS.o
C_OBJECTS += USBDDriver.o
C_OBJECTS += USBDDriverCb_IfSettingChanged.o
C_OBJECTS += USBDDriverCb_CfgChanged.o
C_OBJECTS += USBDCallbacks_Reset.o
C_OBJECTS += USBDCallbacks_Initialized.o
C_OBJECTS += USBEndpointDescriptor.o
C_OBJECTS += USBConfigurationDescriptor.o
C_OBJECTS += USBGenericRequest.o
C_OBJECTS += USBGetDescriptorRequest.o
C_OBJECTS += USBSetConfigurationRequest.o
C_OBJECTS += USBFeatureRequest.o
C_OBJECTS += USBSetAddressRequest.o
C_OBJECTS += USBGenericDescriptor.o
C_OBJECTS += USBInterfaceRequest.o
C_OBJECTS += CDCSetControlLineStateRequest.o
C_OBJECTS += CDCLineCoding.o
C_OBJECTS += CDCDSerialDriverDescriptors.o
# BSP sources
C_OBJECTS += bsp.o
C_OBJECTS += twi.o twid.o math.o
C_OBJECTS += sensors.o pmc.o
C_OBJECTS += vbatmon.o adc.o
C_OBJECTS += flash.o
C_OBJECTS += spid.o at25.o


## Objects built from Assembly source files
ASM_OBJECTS = board_cstartup.o


# -----------------------------------------------------------------------------
# make targets and -rules
# -----------------------------------------------------------------------------

# include file with flash rules
include $(PATH_ROOT)/Flash_ARM7.in

all: $(BIN_DIR) $(OBJ_DIR) $(MEMORIES)

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
	

# Include the dependency files, should be the last of the makefile
-include $(shell mkdir $(OBJ_DIR)/.dep 2>/dev/null) $(wildcard  $(OBJ_DIR)/.dep/*)

# *** EOF ***
