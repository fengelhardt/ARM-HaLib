REM $Id: build_mac_example_nobeacon_app.bat 28117 2011-08-17 16:44:39Z yogesh.bellan $

REM **********************************************
REM Build MAC Example Applications - Nobeacon
REM **********************************************


REM AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM AT86RF212_ATXMEGA256A3_REB_5_0_CBB Device
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\


REM AT86RF230B_ATXMEGA256A3_REB_2_3_CBB Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Coordinator\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM AT86RF230B_ATXMEGA256A3_REB_2_3_CBB Device
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Device\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\


REM AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM AT86RF231_ATXMEGA256A3_REB_4_1_CBB Device
    cd ..\..\..\..\Applications\MAC_Examples\Nobeacon_Application\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
