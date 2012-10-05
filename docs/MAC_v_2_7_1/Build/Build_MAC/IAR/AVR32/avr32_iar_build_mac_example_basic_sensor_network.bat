REM $Id: avr32_iar_build_mac_example_basic_sensor_network.bat 28753 2011-09-28 13:54:08Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Application Basic Sensor Network
REM **********************************************

REM     AT86RF212_AT32UC3B1128_REB_5_0_STK600
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF212_AT32UC3B1128_REB_5_0_STK600\IAR

    %avr32_iar_build% Sensor_Network.ewp -clean Release
    %avr32_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF230B_AT32UC3B1128_REB_2_3_STK600
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF230B_AT32UC3B1128_REB_2_3_STK600\IAR

    %avr32_iar_build% Sensor_Network.ewp -clean Release
    %avr32_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3B1128_REB_4_0_STK600
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF231_AT32UC3B1128_REB_4_0_STK600\IAR

    %avr32_iar_build% Sensor_Network.ewp -clean Release
    %avr32_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Sensor_Network.ewp -clean Release
    %avr32_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF230B_AT32UC3L064_RZ600_UC3LEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Sensor_Network.ewp -clean Release
    %avr32_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Sensor_Network.ewp -clean Release
    %avr32_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF232_AT32UC3B1128_REB_7_0_STK600
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF232_AT32UC3B1128_REB_7_0_STK600\IAR

    %avr32_iar_build% Sensor_Network.ewp -clean Release
    %avr32_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

