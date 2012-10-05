REM $Id: arm_iar_build_tal_example_performance_test_evk.bat 28954 2011-10-18 15:28:53Z ravikumar.vanteddu $

REM **********************************************
REM Build TAL Example Application Performance_Test_EVK 
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Performance_test_evk.ewp -clean Performance_test_evk
    %arm_iar_build% Performance_test_evk.ewp -build Performance_test_evk

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF231_AT91SAM3S4B_SAM3_RFEK01
    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF231_AT91SAM3S4B_SAM3_RFEK01

    %arm_iar_build% Performance_test_evk.ewp -clean Performance_test_evk
    %arm_iar_build% Performance_test_evk.ewp -build Performance_test_evk

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
