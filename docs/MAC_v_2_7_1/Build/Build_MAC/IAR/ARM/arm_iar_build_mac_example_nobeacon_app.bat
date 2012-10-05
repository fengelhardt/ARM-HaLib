REM $Id: arm_iar_build_mac_example_nobeacon_app.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Applications - Nobeacon
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Coordinator\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Coordinator_Nobeacon_App.ewp -clean Release
    %arm_iar_build% Coordinator_Nobeacon_App.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK Device
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Device\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Device_Nobeacon_App.ewp -clean Release
    %arm_iar_build% Device_Nobeacon_App.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Coordinator\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Coordinator_Nobeacon_App.ewp -clean Release
    %arm_iar_build% Coordinator_Nobeacon_App.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK Device
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Device\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Device_Nobeacon_App.ewp -clean Release
    %arm_iar_build% Device_Nobeacon_App.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\
