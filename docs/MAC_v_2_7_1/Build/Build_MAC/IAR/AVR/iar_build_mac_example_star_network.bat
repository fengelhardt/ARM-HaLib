REM $Id: iar_build_mac_example_star_network.bat 28753 2011-09-28 13:54:08Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Application Star Network
REM **********************************************

REM     AT86RF212_ATMEGA1281_RCB_5_3_PLAIN
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_ATMEGA1281_RCB_5_3_PLAIN

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF230B_ATMEGA1281_RCB_3_2_PLAIN
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_ATMEGA1281_RCB_3_2_PLAIN

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF230B_ATMEGA1281_RCB_3_2_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_ATMEGA1281_RCB_3_2_SENS_TERM_BOARD

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM   AT86RF231_ATMEGA1281_RCB_4_1_PLAIN
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_ATMEGA1281_RCB_4_1_PLAIN

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM   AT86RF231_ATMEGA1281_RCB_4_0_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_ATMEGA1281_RCB_4_0_SENS_TERM_BOARD

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM   ATMEGA128RFA1_EK1
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\ATMEGA128RFA1_EK1

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM   ATMEGA128RFA1_RCB_6_3_PLAIN
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\ATMEGA128RFA1_RCB_6_3_PLAIN

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM   ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    iarbuild Star.ewp -clean Release
    iarbuild Star.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\
