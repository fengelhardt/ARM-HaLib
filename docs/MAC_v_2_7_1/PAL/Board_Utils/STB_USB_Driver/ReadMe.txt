Atmel Corporation Inc.

This file contains important information for the deRFdevelopmentKit device
drivers.

Support contact: avr@atmel.com

----------------------------------------------------------------------------

Generally, driver are only needed, if your device has to communicate with a
connected PC. If you intend your device only be talking over the radio channel,
no driver needs to be installed. For the first case, dresden elektronik
provides drivers for MS Windows only where the required device driver(s) differ
depending on your device:

- For
  * deRFusb-based platforms or
  * deRFarm-radio modules combined with deRFnode/-gateway boards
  you solely need a virtual COM-port driver.

- For
  * any device combined with the Sensor-Terminal-board as well as
  * FTDI-based deRFnode/gateway-boards
  you need to install two different drivers. These are modified drivers from
  www.ftdichip.com.

For simplicity, both drivers have been merged together and reside in the
current directory. Please disregard any warnings of the windows device manager
that the driver is not digitally signed and therefore its provider may not be
trusted. For driver deinstallation, windows standard procedures apply.
