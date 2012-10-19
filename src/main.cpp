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
        > LogUartType;

LOGGING_DEFINE_OUTPUT(LogUartType);

#include <arm-halib/arm/usb.h>

struct LogUsb : public driver::UsbFrameLink
{
    char buffer[64];
    uint8_t size;

    LogUsb() : size(0){}

    LogUsb& operator<<(char c)
    {
        buffer[size++]=c;
        if(size==64 || c=='\n')
        {
            send((uint8_t*)buffer, size);
            size=0;
        }
    }
};

typedef OutputLevelSwitchDisabled <
            OutputStream<
                LogUsb
            > 
        > LogUsbType;

LogUsbType logUsb;

//LOGGING_DEFINE_OUTPUT(LogUsbType);


#include <arm-halib/external/led.h>
#include <arm-halib/common/delay.h>

using external::Led;
using common::delay_ms;

Led<17> yellow;
Led<18> green;

int main() 
{
    static const uint32_t xtalStable = 1<<0;
    static Register* const clkStatusReg = reinterpret_cast<Register*>(0x400E0468);
    if(*clkStatusReg & xtalStable)
        yellow.toggle();

    uint16_t i=0;

    while(true)
    {
        logUsb << "USB Logging test: " << i++ << log::endl;
        green.toggle();
        delay_ms(1000);
    }

    return 0;
}

