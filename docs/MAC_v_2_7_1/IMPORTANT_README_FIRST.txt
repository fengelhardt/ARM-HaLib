ATMEL Corporation Inc.

This file contains important information for the MAC used for
802.15.4 transceivers and microcontrollers of ATMEL Corp.

For license information see file:
EULA.txt.

For
- Release and version information of the MAC
- System requirements - Supported Compiler/Tools
see file:
Doc\MAC_Release_Notes.txt.

For the AVR2025 IEEE 802.15.4 MAC Software Package User Guide see file:
Doc\User_Guide\AVR2025_User_Guide.pdf.

For the AVR2025 IEEE 802.15.4 MAC Reference Manual including the API see file:
MAC_readme.html.



Installation
- Unzip release file to a top level folder, like "c:\Atmel".
  In case the package is installed in another subdirectory, issues may occur regarding the length of
  Windows path names during compilation. Therefore make sure the path to the package is as short as
  possible.

- The Windows USB Driver (.inf) and/or corresponding installation files for various boards
  can be found at "PAL\Board_Utils".

Note: Before running the batch files, please make sure system enviroment variable avr32_iar_build is present/create with value "C:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\IarBuild.exe" or the path wherever (if different from mentioned path) you installed your IAR-AVR32 workbench on your Windows system.
Similarly the environment variable arm_iar_build has to be set in your windows system, once you installed your IAR-ARM workbench.


Applications
The included example applications can be built by using the corresponding
makefiles for GCC or the corresponding IAR project files
(e.g. Coordinator.eww).

For further information about the build process please refer to the
AVR2025 IEEE 802.15.4 MAC Software Package User Guide.

$Id: IMPORTANT_README_FIRST.txt 28235 2011-08-24 13:47:47Z ravikumar.vanteddu $
