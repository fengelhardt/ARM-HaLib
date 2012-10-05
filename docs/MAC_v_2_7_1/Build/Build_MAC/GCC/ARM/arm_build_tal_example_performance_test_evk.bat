REM $Id: arm_build_tal_example_performance_test_evk.bat 28954 2011-10-18 15:28:53Z ravikumar.vanteddu $

REM ***********************************************
REM Build TAL Example Performance test evk
REM ***********************************************

REM     AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF231_AT91SAM3S4B_SAM3_RFEK01\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\
