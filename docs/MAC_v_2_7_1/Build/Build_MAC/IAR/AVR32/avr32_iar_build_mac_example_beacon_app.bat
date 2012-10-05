REM $Id: avr32_iar_build_mac_example_beacon_app.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Application Beacon
REM **********************************************

REM AT86RF212_AT32UC3A3256S_RZ600 Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Coordinator\AT86RF212_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Coordinator_Beacon_App.ewp -clean Release
    %avr32_iar_build% Coordinator_Beacon_App.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM AT86RF212_AT32UC3A3256S_RZ600 Device
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Device\AT86RF212_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Device_Beacon_App.ewp -clean Release
    %avr32_iar_build% Device_Beacon_App.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM AT86RF231_AT32UC3A3256S_RZ600 Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Coordinator\AT86RF231_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Coordinator_Beacon_App.ewp -clean Release
    %avr32_iar_build% Coordinator_Beacon_App.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM AT86RF231_AT32UC3A3256S_RZ600 Device
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Device\AT86RF231_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Device_Beacon_App.ewp -clean Release
    %avr32_iar_build% Device_Beacon_App.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
