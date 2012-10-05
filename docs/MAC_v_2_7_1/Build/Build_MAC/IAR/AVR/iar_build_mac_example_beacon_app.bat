REM $Id: iar_build_mac_example_beacon_app.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Application- Beacon
REM **********************************************

REM AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Coordinator_Beacon_App.ewp -clean Release
    iarbuild Coordinator_Beacon_App.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Device_Beacon_App.ewp -clean Release
    iarbuild Device_Beacon_App.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\


REM AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Coordinator_Beacon_App.ewp -clean Release
    iarbuild Coordinator_Beacon_App.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Device_Beacon_App.ewp -clean Release
    iarbuild Device_Beacon_App.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
