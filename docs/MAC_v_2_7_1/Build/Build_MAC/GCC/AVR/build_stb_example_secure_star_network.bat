REM $Id: build_stb_example_secure_star_network.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM ***********************************************
REM Build STB Example App Secure_Star_Network
REM ***********************************************

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Device

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Device

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

