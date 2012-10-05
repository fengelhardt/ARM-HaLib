REM $Id: avr32_iar_build_mac_example_star_network.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $
REM **********************************************
REM Build MAC Example Application Star High Rate
REM **********************************************

REM    AT86RF212_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF230_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM    AT86RF212_AT32UC3B1128_REB_5_0_STK600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT32UC3B1128_REB_5_0_STK600\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF230B_AT32UC3B1128_REB_2_3_STK600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_AT32UC3B1128_REB_2_3_STK600\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3B1128_REB_4_0_STK600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT32UC3B1128_REB_4_0_STK600\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF230B_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Star.ewp -clean Release
    %avr32_iar_build% Star.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
