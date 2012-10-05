REM $Id: arm_iar_build_mac_example_star_network.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $


REM **********************************************
REM Build MAC Example Application Star_Nobeacon
REM **********************************************

REM  AT86RF230B_AT91SAM7X256_REB_2_3_REX_ARM_REV_2
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_AT91SAM7X256_REB_2_3_REX_ARM_REV_2

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\


REM  AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\


REM  AT86RF212_AT91SAM7X256_REB_5_0_REX_ARM_REV_3
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT91SAM7X256_REB_5_0_REX_ARM_REV_3

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4B_SAM3_RFEK02
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT91SAM3S4B_SAM3_RFEK02

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_SAM3_RFEK01
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

