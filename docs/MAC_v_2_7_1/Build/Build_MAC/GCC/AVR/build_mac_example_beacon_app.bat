REM $Id: build_mac_example_beacon_app.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Applications - Beacon
REM **********************************************


REM AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM AT86RF212_ATXMEGA256A3_REB_5_0_CBB Device
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\


REM AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM AT86RF231_ATXMEGA256A3_REB_4_1_CBB Device
    cd ..\..\..\..\Applications\MAC_Examples\Beacon_Application\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
