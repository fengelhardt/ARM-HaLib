This file provides informations concerning the Board Support Package (BSP)
for dresden elektronik wireless development boards provided in the current
directory. 

The BSP is an addon to but not part of the official Atmel MAC software package. 
Therefore please do not contact Atmel for any support requests but write to
dresden elektronik directly via support@dresden-elektronik.de.   


Supported platforms:
--------------------
Currently the following platforms are supported:
- dresden elektronik deRFnode-1TNP2-00N00 with any deRFarm7 module plugged
- dresden elektronik deRFnode-2TNP2-00N00 with any deRFarm7 module plugged
- dresden elektronik deRFgateway-1TNP2-00N00 together with any deRFmega128 
  radio module plugged


Provided functionality:
-----------------------
- access to the temperature, luminosity and acceleration sensors
- access to the serial data flash device
- battery voltage monitor


Usage:
------
The BSP may be used in two different ways:

For the recommended usage, before the BSP can be used, an initial call of 
bsp_init() is required which automatically configures all devices available 
on the current platform. This should be done not until the application has 
fully initialized the stack layers such as PAL, TAL and MAC. Afterwards the 
BSP functionality may be used by invoking the generic functions starting 
with prefix 'bsp_' such as bsp_get_temperature() or bsp_get_acceleration() 
which will redirect the calls to the respective low-level driver.    

A demo application available in the 'BSP_Examples' subdirectory of the 
'Applications' folder demonstrates how to do that.

An alternate way to use the BSP is to directly invoke low-level functions.
Basically this way may perform the same as the first option, but provides more 
flexibility such that only some devices may be initialized respective some 
of them differently. This variant is reserved for experienced users. It is  
demonstrated in the 'LowPower_Sensor' example of 'TINY_TAL_Examples'.   


Requirements:
-------------
Even if the BSP provides all files required for compilation, some things should 
be regarded if you like to integrate the BSP into your custom application: 

First, it is necessary to include the bsp_config.h into your application.
Second, the Makefile has to be adopted. Assumed all devices are to be used, 
the following files have to be compiled:
 - bsp.c  
 - flash.c
 - sensors.c
 - vbatmon.c
  
For the backend, these also applies to the file(s): 
- spid.c

Finally, family dependent  
 - at25.c, twi.c, twid.c for ARM7, 
 - twi_master.c for AVR respectivly
must be built. 
     
Even if unused on the first look, all of these files have to be compiled if 
the BSP is used in the recommended way. If only low-level accessing the devices
some objects may be spared out. Have a look at the named application examples 
for sample Makefiles. 


Implementation details:
-----------------------
For both MCU families (ARM7, AVR), the BSP already contains all files required 
for proper compilation. For the ARM7 family, the AT91Library is involved as 
backend. For AVRs, the device drivers were implemented from scratch.

Used data types are declared in the respective interface header like 
'sensors_interface.h'.

The bsp_config.h provides static inline functions such that the bsp.h doesn't 
have no to be touched if more platforms are to be added. Including it also 
ensuresv that automatically all required additional headers are included 
recursively. 


Path structure:
---------------
 
 |
 +- deRFnodeGateway    base directory to bundle related implementation for a 
 |   |                 board family
 |   |
 |   +- Boards         provides the bsp_config.h file for each supported board,
 |   |                 which defines the physical device interconnection as 
 |   |                 well as macros redirecting generic to specific device  
 |   |                 access functions 
 |   |                   
 |   +- Generic        provides the board-family specific implementation
 |       |
 |       +- Driver     device driver implementation
 |       |   +- Inc    lowlevel device driver interface 
 |       |   +- Src    platform specific implementation, sub-directories for  
 |       |             each MCU family
 |       |
 |       +- Libraries  libraries such as TWI or SPI driver, also split into 
 |       |             sub-directories for each MCU family
 |       |             
 |       +- Src        high-level interface
 |       
 +- Inc                contains header files describing existing BSPs and 
                       the high-level interface API     
                      
For more details please refer to the doxygen inline comments in the respective 
files. 


EOF.
