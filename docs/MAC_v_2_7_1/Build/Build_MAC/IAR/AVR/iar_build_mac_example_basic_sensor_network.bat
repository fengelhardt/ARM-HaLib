REM $Id: iar_build_mac_example_basic_sensor_network.bat 28753 2011-09-28 13:54:08Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Application Basic Sensor Network
REM **********************************************

REM  AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF230B_ATMEGA1281_RCB_3_2_PLAIN
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF230B_ATMEGA1281_RCB_3_2_PLAIN

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF230B_ATMEGA1281_RCB_3_2_SENS_TERM_BOARD

    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF230B_ATMEGA1281_RCB_3_2_SENS_TERM_BOARD

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATMEGA1281_RCB_4_1_PLAIN

    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF231_ATMEGA1281_RCB_4_1_PLAIN

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD

    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     ATMEGA128RFA1_RCB_6_3_PLAIN
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\ATMEGA128RFA1_RCB_6_3_PLAIN

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    iarbuild Sensor_Network.ewp -clean Release
    iarbuild Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\
