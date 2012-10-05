REM $Id: iar_build_all_example_apps.bat 28785 2011-09-30 10:22:59Z ravikumar.vanteddu $

REM **********************************************
REM Build all Example Applications
REM **********************************************


rm -f iar_build_all_example_apps.log
rm -f iar_build_all_example_apps.err


REM ***********************************************
REM MAC Examples
REM ***********************************************

CALL iar_build_mac_example_beacon_app.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_mac_example_nobeacon_app.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_mac_example_basic_sensor_network.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_mac_example_promiscuous_mode_demo.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_mac_example_star_network.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_mac_example_star_high_rate.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_mac_example_star_push_button.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_mac_example_serial_at_interface.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err

REM ***********************************************
REM STB Examples
REM ***********************************************

CALL iar_build_stb_example_secure_remote_control.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_stb_example_secure_sensor.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err
CALL iar_build_stb_example_secure_star_network.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err

REM ***********************************************
REM TAL Examples
REM ***********************************************
CALL iar_build_tal_example_performance_test_evk.bat >> iar_build_all_example_apps.log 2>> iar_build_all_example_apps.err

