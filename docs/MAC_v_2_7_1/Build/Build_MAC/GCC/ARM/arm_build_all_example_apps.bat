REM $Id: arm_build_all_example_apps.bat 28747 2011-09-28 12:09:13Z ravikumar.vanteddu $

REM **********************************************
REM Build all ARM GCC Example Applications
REM **********************************************


rm -f arm_build_all_example_apps.log
rm -f arm_build_all_example_apps.err


REM ***********************************************
REM MAC Examples
REM ***********************************************

CALL arm_build_mac_example_nobeacon_app.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_beacon_app.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_basic_sensor_network.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_promiscuous_mode_demo.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_star_network.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_star_high_rate.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err

REM ***********************************************
REM STB Examples
REM ***********************************************

CALL arm_build_stb_example_secure_remote_control.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_stb_example_secure_sensor.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_stb_example_secure_star_network.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err

REM STB Examples for Software AES are not included in this build

REM ***********************************************
REM TAL Examples
REM ***********************************************
CALL arm_build_tal_example_performance_test_evk.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err

