#pragma once

#include <stdint.h>
#include <arm-halib/regmaps/local.h>
#include "gpio.h"

namespace arm_halib
{
namespace driver {

struct Buttons {
	enum ButtonSetup { plain, pullup, pulldown };
};
typedef Buttons::ButtonSetup ButtonSetup;

template<typename Config>
class Button : public helper::InputGpioPin<Config> {
private:
	typedef typename Config::RegMap RegMap;
public:
	Button() {
		RegMap& rm = *reinterpret_cast<RegMap*>(0);

		switch(Config::setup) {
			case Buttons::plain:
				break;
			case Buttons::pullup:
				rm.pullup |= 0x1 << Config::pin;
				break;
			case Buttons::pulldown:
				rm.pulldown |= 0x1 << Config::pin;
				break;
		}
	}
};

}
}
