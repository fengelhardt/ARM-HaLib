#include <stdint.h>
#include <arm-halib/arm/uart.h>
#include <logging/logging.h>

using namespace ::logging;
using namespace ::arm_halib;
using driver::Uart0;

struct LogUart : public Uart0
{
    LogUart& operator<<(char c)
    {
        while(!ready());
        put(c);
    }
};

typedef OutputLevelSwitchDisabled <
            OutputStream<
                LogUart
            > 
        > LogType;

LOGGING_DEFINE_OUTPUT(LogType);

#include <arm-halib/external/led.h>
#include <arm-halib/common/delay.h>
#include <arm-halib/arm/usbCore.h>


using external::Led;
using driver::USBCore;
using common::delay_ms;

Led<17> yellow;
Led<18> green;

USBCore usb;

int main() 
{
    static const uint32_t xtalStable = 1<<0;
    static Register* const clkStatusReg = reinterpret_cast<Register*>(0x400E0468);
    if(*clkStatusReg & xtalStable)
        yellow.toggle();



//    unsigned int i=0;
    while(true)
    {
/*        log::emit() << "Toggle Led: " << i++ << log::endl;
        green.toggle();
        delay_ms(1000);*/
    }

    return 0;
}

