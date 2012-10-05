REM $Id: arm_build_stb_example_secure_sensor.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM ***********************************************
REM ARM Build STB Example App 1
REM ***********************************************

REM     AT86RF212_AT91SAM3S4C_SAM3SEK Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF212_AT91SAM3S4C_SAM3SEK Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM     AT86RF231_AT91SAM3S4C_SAM3SEK Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF231_AT91SAM3S4C_SAM3SEK Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM     AT86RF231_AT91SAM3S4B_SAM3_RFEK01 Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM3S4B_SAM3_RFEK01\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF231_AT91SAM3S4B_SAM3_RFEK01 Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM3S4B_SAM3_RFEK01\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\



