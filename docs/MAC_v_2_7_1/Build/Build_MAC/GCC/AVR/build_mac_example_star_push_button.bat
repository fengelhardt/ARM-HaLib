REM $Id: build_mac_example_star_push_button.bat 28747 2011-09-28 12:09:13Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example Star Push Button
REM ***********************************************

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF232_ATXMEGA256A3_REB_7_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     ATMEGA128RFA1_RCB_6_3_PLAIN

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\ATMEGA128RFA1_RCB_6_3_PLAIN\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\
