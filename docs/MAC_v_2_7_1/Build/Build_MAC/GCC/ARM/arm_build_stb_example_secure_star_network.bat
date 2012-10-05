REM $Id: arm_build_stb_example_secure_star_network.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM ***********************************************
REM ARM Build STB Example Secure Star Network
REM ***********************************************

REM     AT86RF212_AT91SAM3S4C_SAM3SEK Coordinator

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF212_AT91SAM3S4C_SAM3SEK Device

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM     AT86RF231_AT91SAM3S4C_SAM3SEK Coordinator

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF231_AT91SAM3S4C_SAM3SEK Device

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF212_AT91SAM3S4B_SAM3_RFEK02 Coordinator

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF212_AT91SAM3S4B_SAM3_RFEK02\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF212_AT91SAM3S4B_SAM3_RFEK02 Device

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF212_AT91SAM3S4B_SAM3_RFEK02\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM     AT86RF231_AT91SAM3S4B_SAM3_RFEK01 Coordinator

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF231_AT91SAM3S4B_SAM3_RFEK01\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF231_AT91SAM3S4B_SAM3_RFEK01 Device

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF231_AT91SAM3S4B_SAM3_RFEK01\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\



