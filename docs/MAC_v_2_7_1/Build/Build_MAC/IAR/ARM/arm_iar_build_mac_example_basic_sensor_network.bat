REM $Id: arm_iar_build_mac_example_basic_sensor_network.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Application Basic Sensor Network
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor_Network.ewp -clean Release
    %arm_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF230B_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor_Network.ewp -clean Release
    %arm_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor_Network.ewp -clean Release
    %arm_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\


