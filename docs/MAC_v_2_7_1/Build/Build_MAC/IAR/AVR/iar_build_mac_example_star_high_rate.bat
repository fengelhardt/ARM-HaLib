REM $Id: iar_build_mac_example_star_high_rate.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Application Star High Rate
REM **********************************************

REM  ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Star_High_Rate.ewp -clean Release
    iarbuild Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Star_High_Rate.ewp -clean Release
    iarbuild Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Star_High_Rate.ewp -clean Release
    iarbuild Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\
