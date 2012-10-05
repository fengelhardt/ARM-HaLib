REM $Id: build_stb_example_secure_remote_control.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM ***********************************************
REM Build STB Example App Secure Remote Control
REM ***********************************************

REM     AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\


REM     AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     ATMEGA128RFA1_RCB_6_3_PLAIN

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\ATMEGA128RFA1_RCB_6_3_PLAIN\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\


REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

