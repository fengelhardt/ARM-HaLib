REM $Id: iar_build_stb_example_secure_sensor.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build STB Example Secure_Sensor
REM **********************************************

REM     AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD Data_Sink
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD

    iarbuild Data_Sink.ewp -clean Release
    iarbuild Data_Sink.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD Sensor
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_ATMEGA1281_RCB_5_3_SENS_TERM_BOARD

    iarbuild Sensor.ewp -clean Release
    iarbuild Sensor.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD Data_Sink
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD

    iarbuild Data_Sink.ewp -clean Release
    iarbuild Data_Sink.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD Sensor
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_ATMEGA1281_RCB_4_1_SENS_TERM_BOARD

    iarbuild Sensor.ewp -clean Release
    iarbuild Sensor.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD Data_Sink
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Data_Sink.ewp -clean Release
    iarbuild Data_Sink.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD Sensor
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Sensor.ewp -clean Release
    iarbuild Sensor.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Data_Sink
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Data_Sink.ewp -clean Release
    iarbuild Data_Sink.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Sensor
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Sensor.ewp -clean Release
    iarbuild Sensor.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Data_Sink
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Data_Sink.ewp -clean Release
    iarbuild Data_Sink.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Sensor
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Sensor.ewp -clean Release
    iarbuild Sensor.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\



