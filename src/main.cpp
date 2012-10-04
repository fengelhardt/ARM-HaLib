#include <stdint.h>
#include <arm-halib/arm/uart.h>
#include <arm-halib/external/led.h>
#include <arm-halib/common/delay.h>

#include <logging/logging.h>

using namespace ::logging;
using namespace ::arm_halib;
using external::Led;
using driver::Uart0;
using common::delay_ms;

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

Led<17> yellow;
Led<18> green;

int main() 
{
    static const uint32_t xtalStable = 1<<0;
    static Register* const clkStatusReg = reinterpret_cast<Register*>(0x400E0468);
    if(*clkStatusReg & xtalStable)
        yellow.toggle();

    log::emit() << "Starting USB bus" << log::endl;

    static const uint32_t udp              = 1<< (34-32);
    static const uint32_t udpPullUpConnect = 1<<9;
    static const uint32_t udpTxDisable     = 1<<8;
    static const uint32_t udpBusReset      = 1<<12;
    static const uint32_t udpClearInts     = 0x3f << 8;
    static const uint32_t udpClock         = 1<<7;
    static const uint32_t udpEpEn          = 1<<15;
    static const uint32_t udpEpRxSetup     = 1<<2;

    static Register* const nvicSetIntReg1    = reinterpret_cast<Register*>(0xE000E104);
    static Register* const pmcPeriEnableReg1 = reinterpret_cast<Register*>(0x400E0500);
    static Register* const pmcSysClockEnReg  = reinterpret_cast<Register*>(0x400E0400);
    *pmcSysClockEnReg  = udpClock;
    *pmcPeriEnableReg1 = udp;
//    *nvicSetIntReg1    = udp;

    static Register* const udpControlReg   = reinterpret_cast<Register*>(0x40034074);
    static Register* const udpIntStatusReg = reinterpret_cast<Register*>(0x4003401C);
    static Register* const udpIntClearReg = reinterpret_cast<Register*>(0x4003401C);
    static Register* const udpEp0CSReg     = reinterpret_cast<Register*>(0x40034030);
    *udpIntClearReg = udpClearInts;
    *udpControlReg = udpPullUpConnect | udpTxDisable;

    while(!(*udpIntStatusReg & udpBusReset));
    log::emit() << "USB Bus reset " << log::endl;
    *udpIntClearReg = udpBusReset;

    *udpEp0CSReg = udpEpEn;
    *udpControlReg = udpPullUpConnect;

    while(!(*udpEp0CSReg & udpEpRxSetup));
    log::emit() << "Got Address" << log::endl;


    unsigned int i=0;
    while(true)
    {
        log::emit() << "Toggle Led: " << i++ << log::endl;
        green.toggle();
        delay_ms(1000);
    }

    return 0;
}

