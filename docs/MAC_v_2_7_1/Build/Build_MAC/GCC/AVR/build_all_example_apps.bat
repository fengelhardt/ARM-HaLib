REM $Id: build_all_example_apps.bat 28747 2011-09-28 12:09:13Z ravikumar.vanteddu $

REM **********************************************
REM Build all Example Applications
REM **********************************************


rm -f build_all_example_apps.log
rm -f build_all_example_apps.err


REM ***********************************************
REM MAC Examples
REM ***********************************************

CALL build_mac_example_beacon_app.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_mac_example_nobeacon_app.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_mac_example_basic_sensor_network.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_mac_example_promiscuous_mode_demo.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_mac_example_star_network.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_mac_example_star_push_button.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_mac_example_star_high_rate.bat >> build_all_example_apps.log 2>> build_all_example_apps.err

REM ***********************************************
REM STB Examples
REM ***********************************************

CALL build_stb_example_secure_remote_control.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_stb_example_secure_sensor.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_stb_example_secure_star_network.bat >> build_all_example_apps.log 2>> build_all_example_apps.err

REM ***********************************************
REM TAL Examples
REM ***********************************************

REM CALL build_tal_example_performance_test.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
CALL build_tal_example_performance_test_evk.bat >> build_all_example_apps.log 2>> build_all_example_apps.err
