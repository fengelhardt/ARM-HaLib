REM $Id: iar_build_mac_example_promiscuous_mode_demo.bat 28753 2011-09-28 13:54:08Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Application Promiscuous Mode Demo
REM **********************************************

REM   AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD

    iarbuild Promiscuous_Mode_Demo.ewp -clean Release
    iarbuild Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM   AT86RF230B_ATMEGA1281_RCB_3_2_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF230B_ATMEGA1281_RCB_3_2_SENS_TERM_BOARD

    iarbuild Promiscuous_Mode_Demo.ewp -clean Release
    iarbuild Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM   AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD

    iarbuild Promiscuous_Mode_Demo.ewp -clean Release
    iarbuild Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM   ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Promiscuous_Mode_Demo.ewp -clean Release
    iarbuild Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Promiscuous_Mode_Demo.ewp -clean Release
    iarbuild Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    iarbuild Promiscuous_Mode_Demo.ewp -clean Release
    iarbuild Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Promiscuous_Mode_Demo.ewp -clean Release
    iarbuild Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    iarbuild Promiscuous_Mode_Demo.ewp -clean Release
    iarbuild Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\
