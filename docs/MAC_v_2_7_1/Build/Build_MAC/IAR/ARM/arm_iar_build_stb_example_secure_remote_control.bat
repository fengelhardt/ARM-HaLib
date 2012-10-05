REM $Id: arm_iar_build_stb_example_secure_remote_control.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build STB Example Application Secure_Remote_Control
REM **********************************************


REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Secure_Remote_Control.ewp -clean Secure_Remote_Control
    %arm_iar_build% Secure_Remote_Control.ewp -build Secure_Remote_Control

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Secure_Remote_Control.ewp -clean Secure_Remote_Control
    %arm_iar_build% Secure_Remote_Control.ewp -build Secure_Remote_Control

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
