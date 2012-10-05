REM $Id: arm_iar_build_stb_example_secure_star_network.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $


REM **********************************************
REM Build STB Example Application Secure_Star_Network
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Coordinator.ewp -clean Coordinator
    %arm_iar_build% Coordinator.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Device.ewp -clean Device
    %arm_iar_build% Device.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Coordinator.ewp -clean Coordinator
    %arm_iar_build% Coordinator.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Device.ewp -clean Device
    %arm_iar_build% Device.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF212_AT91SAM3S4B_SAM3_RFEK02
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF212_AT91SAM3S4B_SAM3_RFEK02

    %arm_iar_build% Coordinator.ewp -clean Coordinator
    %arm_iar_build% Coordinator.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF212_AT91SAM3S4B_SAM3_RFEK02

    %arm_iar_build% Device.ewp -clean Device
    %arm_iar_build% Device.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_SAM3_RFEK01
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    %arm_iar_build% Coordinator.ewp -clean Coordinator
    %arm_iar_build% Coordinator.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    %arm_iar_build% Device.ewp -clean Device
    %arm_iar_build% Device.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3
    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Coordinator\AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3

    %arm_iar_build% Coordinator.ewp -clean Coordinator
    %arm_iar_build% Coordinator.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Star_Network\Device\AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3

    %arm_iar_build% Device.ewp -clean Device
    %arm_iar_build% Device.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\
