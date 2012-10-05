REM $Id: avr32_iar_build_tal_example_performance_test_evk.bat 28753 2011-09-28 13:54:08Z ravikumar.vanteddu $

REM **********************************************
REM Build TAL Example Application Performance_Test_EVK 
REM **********************************************

REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR\

    %avr32_iar_build% Performance_test_evk.ewp -clean Release
    %avr32_iar_build% Performance_test_evk.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF231_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF231_AT32UC3A3256S_RZ600\IAR\

    %avr32_iar_build% Performance_test_evk.ewp -clean Release
    %avr32_iar_build% Performance_test_evk.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
