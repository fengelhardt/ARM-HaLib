REM $Id: arm_iar_build_mac_example_star_high_rate.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $


REM **********************************************
REM Build MAC Example Application Star_High_Rate
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star_High_Rate.ewp -clean Release
    %arm_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star_High_Rate.ewp -clean Release
    %arm_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4B_SAM3_RFEK02
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT91SAM3S4B_SAM3_RFEK02

    %arm_iar_build% Star_High_Rate.ewp -clean Release
    %arm_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_SAM3_RFEK01
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    %arm_iar_build% Star_High_Rate.ewp -clean Release
    %arm_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
