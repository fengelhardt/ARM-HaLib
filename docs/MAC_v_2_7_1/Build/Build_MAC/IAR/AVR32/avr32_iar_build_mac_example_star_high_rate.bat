REM $Id: avr32_iar_build_mac_example_star_high_rate.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $
REM **********************************************
REM Build MAC Example Application Star High Rate
REM **********************************************

REM    AT86RF212_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Star_High_Rate.ewp -clean Release
    %avr32_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT32UC3A3256S_RZ600\IAR

    %avr32_iar_build% Star_High_Rate.ewp -clean Release
    %avr32_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Star_High_Rate.ewp -clean Release
    %avr32_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iar_build% Star_High_Rate.ewp -clean Release
    %avr32_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
