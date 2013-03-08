#pragma once

#include <uart0.h>
#include <pwmGenerator.h>
#include <arm-halib/regmaps/local.h>
#include <logging/logging.h>

namespace platform{
namespace stellaris
{

  using namespace ::logging;
  using namespace arm_halib;
  using arm_halib::regmaps::local::Timer;
  using arm_halib::driver::PwmGenerator;
  using arm_halib::driver::PwmChannelType;
  using arm_halib::driver::PwmChannels;
  using arm_halib::regmaps::local::GpioBankType;
  using arm_halib::regmaps::local::GpioBanks;
  using arm_halib::driver::GpioPin;
  using arm_halib::regmaps::local::GpioBank;

  struct LogUart : public arm_halib::driver::Uart0
  {
      LogUart& operator<<(char c)
      {
          while(!ready());
          put(c);
          return *this;
      }
  };

  typedef OutputLevelSwitchDisabled <
              OutputStream<
                  LogUart
              > 
          > LogType;

  struct RedConfig
  {
    typedef Timer<0> RegMap;
    static const PwmChannelType channel = PwmChannels::b;
    static const GpioBankType bank = GpioBanks::f;
    static const uint8_t pin = 1;
    static const bool invert = true;
  };

  struct BlueConfig
  {
    typedef Timer<1> RegMap;
    static const PwmChannelType channel = PwmChannels::a;
    static const GpioBankType bank = GpioBanks::f;
    static const uint8_t pin = 2;
    static const bool invert = true;
  };

  struct GreenConfig
  {
    typedef Timer<1> RegMap;
    static const PwmChannelType channel = PwmChannels::b;
    static const GpioBankType bank = GpioBanks::f;
    static const uint8_t pin = 3;
    static const bool invert = true;
  };

  typedef PwmGenerator<RedConfig> RedLed;
  typedef PwmGenerator<BlueConfig> BlueLed;
  typedef PwmGenerator<GreenConfig> GreenLed;

  struct SW1Config
  {
    typedef GpioBank<GpioBanks::f> RegMap;
    static const uint8_t pin = 4;
    static const bool special = false;
    static const bool output = false;
  };

  struct SW2Config
  {
    typedef GpioBank<GpioBanks::f> RegMap;
    static const uint8_t pin = 0;
    static const bool special = false;
    static const bool output = false;
  };

  typedef GpioPin<SW1Config> SW1;
  typedef GpioPin<SW2Config> SW2;
}
}
