#pragma once

#include <stdint.h>

namespace arm_halib{
namespace driver{
namespace usb
{
    static const uint32_t peripheralId           = 1<< (34-32);
    static const uint32_t clockId                = 1<<7;
    static const uint32_t pullUpConnect          = 1<<9;
    static const uint32_t txDisable              = 1<<8;
    static const uint32_t busReset               = 1<<12;
    static const uint32_t clearInterrupts        = 0x3f << 8;
    static const uint32_t functionEndpointEnable = 1<<8;
    static const uint32_t functionAddressEnable  = 1<<0;
    static const uint32_t configured             = 1<<1;
    static const uint32_t endpoint0Interrupt     = 1<<0;
    static const uint32_t endpointEnable         = 1<<15;
    static const uint32_t endpointRxSetup        = 1<<2;
    static const uint32_t endpointDir            = 1<<7;
    static const uint32_t endpointTxComplete     = 1<<0;
    static const uint32_t endpointFlags          = 0x4f;
    static const uint32_t endpointPacketReady    = 1<<4;
    static const uint32_t endpointForceStall     = 1<<5;
    static const uint32_t endpointStallSend      = 1<<3;

    union RegMap{
        struct{
            uint8_t __base[0x40034000];
            union{
                struct{
                    uint8_t __pad0[0x00];
                    uint32_t frameNumber;
                    uint32_t globalState;
                    uint32_t functionAddress;
                };
                struct{
                    uint8_t __pad1[0x10];
                    uint32_t interruptEnable;
                    uint32_t interruptDisable;
                    uint32_t interruptMask;
                    uint32_t interruptStatus;
                    uint32_t interruptClear;
                };
                struct{
                    uint8_t __pad2[0x30];
                    uint32_t endpoint0Control;
                    uint32_t endpoint1Control;
                };
                struct{
                    uint8_t __pad3[0x50];
                    uint32_t endpoint0Fifo;
                    uint32_t endpoint1Fifo;
                };
                struct{
                    uint8_t __pad4[0x74];
                    uint32_t control;
                };
            };
        };
        struct{
            uint8_t __base[0x400E0000];
            union{
                struct{
                    uint8_t __pad0 [0x400];
                    uint32_t systemClockEnable;
                };
                struct{
                    uint8_t __pad1 [0x500];
                    uint32_t peripheralClockEnable;
                };
            };
        } clockController;
        struct
        {
            uint8_t __base[0xE000E000];
            union{
                struct{
                    uint8_t __pad[0x104];
                    uint32_t interruptEnable;
                    uint32_t interruptDisable;
                };
           };
            
        } interruptController;
    };
}
}
}
