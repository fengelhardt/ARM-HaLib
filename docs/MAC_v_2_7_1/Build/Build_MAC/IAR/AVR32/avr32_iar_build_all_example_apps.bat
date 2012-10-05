REM $Id: avr32_iar_build_all_example_apps.bat 28753 2011-09-28 13:54:08Z ravikumar.vanteddu $

REM **********************************************
REM Build all Example Applications
REM **********************************************


rm -f avr32_iar_build_all_example_apps.log
rm -f avr32_iar_build_all_example_apps.err


REM ***********************************************
REM MAC Examples
REM ***********************************************

CALL avr32_iar_build_mac_example_beacon_app.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err
CALL avr32_iar_build_mac_example_nobeacon_app.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err
CALL avr32_iar_build_mac_example_basic_sensor_network.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err
CALL avr32_iar_build_mac_example_promiscuous_mode_demo.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err
CALL avr32_iar_build_mac_example_star_network.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err
CALL avr32_iar_build_mac_example_star_high_rate.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err
CALL avr32_iar_build_mac_example_star_push_button.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err


REM ***********************************************
REM STB Examples
REM ***********************************************

CALL avr32_iar_build_stb_example_secure_remote_control.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err
CALL avr32_iar_build_stb_example_secure_sensor.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err

REM ***********************************************
REM TAL Examples
REM ***********************************************
CALL avr32_iar_build_tal_example_performance_test_evk.bat >> avr32_iar_build_all_example_apps.log 2>> avr32_iar_build_all_example_apps.err

