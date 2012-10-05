REM $Id: arm_iar_build_mac_example_serial_at_interface.bat 28785 2011-09-30 10:22:59Z ravikumar.vanteddu $


REM **********************************************
REM Build MAC Example Application Serial AT Interface
REM **********************************************

REM  AT86RF231_AT91SAM3S4B_SAM3_RFEK01
    cd ..\..\..\..\Applications\MAC_Examples\Serial_AT_Interface\AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    %arm_iar_build% Serial_AT_Interface.ewp -clean Serial_AT_Interface
    %arm_iar_build% Serial_AT_Interface.ewp -build Serial_AT_Interface

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
