REM $Id: iar_build_mac_example_serial_at_interface.bat 28785 2011-09-30 10:22:59Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Serial AT Interface
REM **********************************************

    cd ..\..\..\..\Applications\MAC_Examples\Serial_AT_Interface\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Serial_AT_Interface.ewp -clean Release
    iarbuild Serial_AT_Interface.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\
