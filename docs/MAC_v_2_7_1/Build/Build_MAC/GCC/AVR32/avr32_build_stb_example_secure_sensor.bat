REM $Id: avr32_build_stb_example_secure_sensor.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM ***********************************************
REM Build STB Example Secure Sensor
REM ***********************************************

REM     AT86RF212_AT32UC3A3256S_RZ600 Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT32UC3A3256S_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF212_AT32UC3A3256S_RZ600 Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT32UC3A3256S_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\


REM     AT86RF231_AT32UC3A3256S_RZ600 Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT32UC3A3256S_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF231_AT32UC3A3256S_RZ600 Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT32UC3A3256S_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

