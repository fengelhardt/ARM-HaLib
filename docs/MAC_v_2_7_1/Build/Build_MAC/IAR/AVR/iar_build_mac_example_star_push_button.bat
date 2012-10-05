REM $Id: iar_build_mac_example_star_push_button.bat 28753 2011-09-28 13:54:08Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Application Star Push Button
REM **********************************************

REM     ATMEGA128RFA1_RCB_6_3_PLAIN

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\ATMEGA128RFA1_RCB_6_3_PLAIN

    iarbuild Star_Push_Button.ewp -clean Release
    iarbuild Star_Push_Button.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Star_Push_Button.ewp -clean Release
    iarbuild Star_Push_Button.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    iarbuild Star_Push_Button.ewp -clean Release
    iarbuild Star_Push_Button.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Star_Push_Button.ewp -clean Release
    iarbuild Star_Push_Button.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    iarbuild Star_Push_Button.ewp -clean Release
    iarbuild Star_Push_Button.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\
