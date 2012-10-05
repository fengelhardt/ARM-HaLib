REM $Id: arm_iar_build_mac_example_beacon_app.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Applications - Beacon
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Coordinator\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Coordinator_Beacon_App.ewp -clean Coordinator
    %arm_iar_build% Coordinator_Beacon_App.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK Device
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Device\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Device_Beacon_App.ewp -clean Device
    %arm_iar_build% Device_Beacon_App.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Coordinator\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Coordinator_Beacon_App.ewp -clean Coordinator
    %arm_iar_build% Coordinator_Beacon_App.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK Device
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Device\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Device_Beacon_App.ewp -clean Device
    %arm_iar_build% Device_Beacon_App.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\
