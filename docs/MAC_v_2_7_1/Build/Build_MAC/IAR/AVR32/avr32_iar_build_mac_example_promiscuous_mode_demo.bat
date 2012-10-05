REM $Id: avr32_iar_build_mac_example_promiscuous_mode_demo.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Application Promiscuous Mode Demo
REM **********************************************

REM  AT86RF212_AT32UC3L064_RZ600_UC3LEK
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM  AT86RF230B_AT32UC3L064_RZ600_UC3LEK
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM  AT86RF231_AT32UC3L064_RZ600_UC3LEK
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM  AT86RF212_AT32UC3A3256S_RZ600
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF212_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM  AT86RF230B_AT32UC3A3256S_RZ600
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF230B_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM  AT86RF231_AT32UC3A3256S_RZ600
    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF231_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iar_build% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
