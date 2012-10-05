REM $Id: arm_iar_build_all_example_apps.bat 28785 2011-09-30 10:22:59Z ravikumar.vanteddu $

REM **********************************************
REM Build all ARM IAR Example Applications
REM **********************************************


rm -f arm_iar_build_all_example_apps.log
rm -f arm_iar_build_all_example_apps.err


REM ***********************************************
REM MAC Examples
REM ***********************************************

CALL arm_iar_build_mac_example_nobeacon_app.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
CALL arm_iar_build_mac_example_beacon_app.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
CALL arm_iar_build_mac_example_basic_sensor_network.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
CALL arm_iar_build_mac_example_promiscuous_mode_demo.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
CALL arm_iar_build_mac_example_star_network.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
CALL arm_iar_build_mac_example_star_high_rate.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
CALL arm_iar_build_mac_example_serial_at_interface.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err

REM ***********************************************
REM STB Examples
REM ***********************************************

CALL arm_iar_build_stb_example_secure_remote_control.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
CALL arm_iar_build_stb_example_secure_sensor.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
CALL arm_iar_build_stb_example_secure_star_network.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err

REM ***********************************************
REM TAL Examples
REM ***********************************************
CALL arm_iar_build_tal_example_performance_test_evk.bat >> arm_iar_build_all_example_apps.log 2>> arm_iar_build_all_example_apps.err
