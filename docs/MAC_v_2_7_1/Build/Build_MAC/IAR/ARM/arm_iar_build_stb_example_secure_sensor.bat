REM $Id: arm_iar_build_stb_example_secure_sensor.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $


REM **********************************************
REM Build STB Example Application Secure_Sensor
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF212_AT91SAM3S4B_SAM3_RFEK02
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT91SAM3S4B_SAM3_RFEK02

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT91SAM3S4B_SAM3_RFEK02

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_SAM3_RFEK01
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor


    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\
