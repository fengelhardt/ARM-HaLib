REM $Id: iar_build_stb_example_secure_star_network.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build STB Example Secure Star Network Apps
REM **********************************************
REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD Coordinator
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Secure_Coordinator.ewp -clean Release
    iarbuild Secure_Coordinator.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD Device
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Secure_Device.ewp -clean Release
    iarbuild Secure_Device.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Secure_Coordinator.ewp -clean Release
    iarbuild Secure_Coordinator.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Device
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Secure_Device.ewp -clean Release
    iarbuild Secure_Device.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Secure_Coordinator.ewp -clean Release
    iarbuild Secure_Coordinator.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Device
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Secure_Device.ewp -clean Release
    iarbuild Secure_Device.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\


