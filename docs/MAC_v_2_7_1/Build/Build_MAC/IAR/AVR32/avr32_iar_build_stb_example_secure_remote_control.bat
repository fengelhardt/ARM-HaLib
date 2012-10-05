REM $Id: avr32_iar_build_stb_example_secure_remote_control.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $
REM **********************************************
REM Build STB Example Secure_Remote_Control
REM **********************************************

REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Secure_Remote_Control.ewp -clean Release
    %avr32_iar_build% Secure_Remote_Control.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Secure_Remote_Control.ewp -clean Release
    %avr32_iar_build% Secure_Remote_Control.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
