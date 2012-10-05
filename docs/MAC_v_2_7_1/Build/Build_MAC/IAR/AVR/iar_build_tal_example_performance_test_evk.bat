REM $Id: iar_build_tal_example_performance_test_evk.bat 28954 2011-10-18 15:28:53Z ravikumar.vanteddu $

REM ***********************************************
REM Build TAL Examples Performance test evk
REM ***********************************************

REM     ATMEGA128RFA1_RCB_6_3_PLAIN_OR_STB

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\ATMEGA128RFA1_RCB_6_3_PLAIN_OR_STB\IAR\

    iarbuild Performance_test_evk.ewp -clean Release
    iarbuild Performance_test_evk.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
	
REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\IAR\

    iarbuild Performance_test_evk.ewp -clean Release
    iarbuild Performance_test_evk.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\IAR\

    iarbuild Performance_test_evk.ewp -clean Release
    iarbuild Performance_test_evk.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

REM     AT86RF232_ATXMEGA256A3_REB_7_0_CBB

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF232_ATXMEGA256A3_REB_7_0_CBB\IAR\

    iarbuild Performance_test_evk.ewp -clean Release
    iarbuild Performance_test_evk.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
