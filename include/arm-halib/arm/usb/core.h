#pragma once

#include <stdint.h>
#include <arm-halib/common/delay.h>
#include "descriptors.h"
#include "regmap.h"

namespace arm_halib{
namespace driver{
namespace usb
{
    DeviceDescriptor devDesc ={
                sizeof(DeviceDescriptor),
                Descriptors::device,
                0x200,
                0,
                0,
                0,
                64,
                0,
                0,
                0,
                0,
                0,
                0,
                1
    };

    DeviceDescriptor devQualDesc ={
                sizeof(DeviceDescriptor),
                Descriptors::device_qualifier,
                0x200,
                0,
                0,
                0,
                64,
                1,
                0
    };
    ConfigurationDescriptor confDesc ={
                sizeof(ConfigurationDescriptor),
                Descriptors::config,
                sizeof(ConfigurationDescriptor)+sizeof(InterfaceDescriptor)+sizeof(EndpointDescriptor),
                1,
                1,
                0,
                0x80,
                50
    };
    InterfaceDescriptor ifDesc ={
                sizeof(InterfaceDescriptor),
                Descriptors::interface,
                0,
                0,
                1,
                0xFF,
                0x0,
                0x0,
                0
    };
    EndpointDescriptor epDesc ={
                sizeof(EndpointDescriptor),
                Descriptors::endpoint,
                0x83,
                0x3,
                8,
                255
    };

    struct Core
    {
        struct SetupData
        {
            struct
            {
                uint8_t recipient : 5;
                uint8_t type      : 2;
                uint8_t direction : 1;
            } bmRequestType;
            uint8_t bRequest;
            uint16_t wValue;
            uint16_t wIndex;
            uint16_t wLength;
        } __attribute__((packed));

        Core()
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            log::emit() << "Starting USB bus" << log::endl;
            
            rm.clockController.systemClockEnable     = clockId;
            rm.clockController.peripheralClockEnable = peripheralId;
            rm.interruptController.interruptEnable   = peripheralId;
           
            rm.interruptClear  = clearInterrupts;
            rm.control         = pullUpConnect | txDisable;
        }

        void test()
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            while(!(rm.globalState & configured));
            while(true)
            {
                log::emit() << "Transmit test data" << log::endl;
                for(uint8_t i=0;i<8;i++)
                    rm.endpoint3Fifo='A';

                setEndpoint3Bit(endpointPacketReady);
                while(!(rm.endpoint3Control & endpointTxComplete));
                clearEndpoint3Bit(endpointTxComplete);
            }
        }

        static void setEndpointBit(uint32_t bit)
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            uint32_t temp = rm.endpoint0Control;
            temp |= endpointFlags;
            temp |= bit;
            rm.endpoint0Control = temp;
            SysTickTimer::wait(15, false);
        }

        static void clearEndpointBit(uint32_t bit)
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            uint32_t temp = rm.endpoint0Control;
            temp |= endpointFlags;
            temp &= ~bit;
            rm.endpoint0Control = temp;
            SysTickTimer::wait(15, false);
        }

        static void setEndpoint3Bit(uint32_t bit)
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            uint32_t temp = rm.endpoint3Control;
            temp |= endpointFlags;
            temp |= bit;
            rm.endpoint3Control = temp;
            SysTickTimer::wait(15, false);
        }

        static void clearEndpoint3Bit(uint32_t bit)
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            uint32_t temp = rm.endpoint3Control;
            temp |= endpointFlags;
            temp &= ~bit;
            rm.endpoint3Control = temp;
            SysTickTimer::wait(15, false);
        }

        static void sendDescriptor(SetupData& data)
        {
            log::emit() << "Request: GET_DESCRIPTOR" << log::endl;
            log::emit() << "Descriptor Type : "      << (data.wValue>>8) << log::endl;;
            log::emit() << "Descriptor Index: "      << (data.wValue&0xFF) << log::endl;
            log::emit() << "Lang-Id         : "      << data.wIndex << log::endl;
            log::emit() << "Length          : "      << data.wLength << log::endl;

           setEndpointBit(endpointDir);
           clearEndpointBit(endpointRxSetup);

           volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
           clearEndpointBit(endpointPacketReady);

           uint8_t txBytes = 0;

           switch(data.wValue>>8)
           {
               case(Descriptors::device): 
                   for(uint8_t i=0;i<sizeof(DeviceDescriptor);i++)
                   {
                       rm.endpoint0Fifo=reinterpret_cast<uint8_t*>(&devDesc)[i];
                       txBytes++;
                   }
                   break;

               case(Descriptors::device_qualifier): 
                   for(uint8_t i=0;i<sizeof(DeviceQualifierDescriptor) && txBytes<data.wLength ;i++)
                   {
                       rm.endpoint0Fifo=reinterpret_cast<uint8_t*>(&devQualDesc)[i];
                       txBytes++;
                   }
                   break;

               case(Descriptors::config): 
                   for(uint8_t i=0;i<sizeof(ConfigurationDescriptor) && txBytes<data.wLength;i++)
                   {
                       rm.endpoint0Fifo=reinterpret_cast<uint8_t*>(&confDesc)[i];
                       txBytes++;
                   }
                   for(uint8_t i=0;i<sizeof(InterfaceDescriptor) && txBytes<data.wLength;i++)
                   {
                       rm.endpoint0Fifo=reinterpret_cast<uint8_t*>(&ifDesc)[i];
                       txBytes++;
                   }
                   for(uint8_t i=0;i<sizeof(EndpointDescriptor) && txBytes<data.wLength;i++)
                   {
                       rm.endpoint0Fifo=reinterpret_cast<uint8_t*>(&epDesc)[i];
                       txBytes++;
                   }
                   break;
               default: 
                   setEndpointBit(endpointForceStall);
                   return;
           };

           log::emit() << "Transmitting " << (uint16_t)txBytes << " Bytes to Host" << log::endl;

            setEndpointBit(endpointPacketReady);
            while(!(rm.endpoint0Control & endpointTxComplete));
            clearEndpointBit(endpointTxComplete);
        }

        static void setAddress(SetupData& data)
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);

            log::emit() << "Request: SET_ADDRESS" << log::endl;
            log::emit() << "Address: " << data.wValue << log::endl;;

            setEndpointBit(endpointDir);
            clearEndpointBit(endpointRxSetup);

            clearEndpointBit(endpointPacketReady);
            setEndpointBit(endpointPacketReady);
            while(!(rm.endpoint0Control & endpointTxComplete));
            clearEndpointBit(endpointTxComplete);

            rm.functionAddress = data.wValue | functionEndpointEnable;
            rm.globalState |= functionAddressEnable;
        }

        static void setConfiguration(SetupData& data)
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            log::emit() << "Request: SET_CONFIGURATION" << log::endl;
            log::emit() << "Configuration index: " << data.wValue << log::endl;
            
            setEndpointBit(endpointDir);
            clearEndpointBit(endpointRxSetup);

            setEndpointBit(endpointPacketReady);
            while(!(rm.endpoint0Control & endpointTxComplete));
            clearEndpointBit(endpointTxComplete);

            setEndpoint3Bit(0x7 << 8);
            setEndpoint3Bit(endpointEnable);

            rm.globalState |= configured;
        }

        static void sendStatus(SetupData& data)
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            log::emit() << "Request: GET_STATUS" << log::endl;

            setEndpointBit(endpointDir);
            clearEndpointBit(endpointRxSetup);

            rm.endpoint0Fifo = 0x0;
            rm.endpoint0Fifo = 0x0;

            setEndpointBit(endpointPacketReady);
            while(!(rm.endpoint0Control & endpointTxComplete));
            clearEndpointBit(endpointTxComplete);
        }

        static void handleIRQ()
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            log::emit() << "Got interrupt: " << log::hex << rm.interruptStatus << log::dec << log::endl;
            if(rm.interruptStatus & busReset)
            {
                log::emit() << "USB Bus reset " << log::endl;
                rm.interruptClear   = 0xffffffff;
                rm.interruptDisable = 0xffffffff;
                rm.interruptEnable  = endpoint0Interrupt;
                setEndpointBit(endpointEnable);
                rm.control          = pullUpConnect;
                return;
            }

            if(rm.endpoint0Control & endpointStallSend)
            {
                log::emit() << "Request error" << log::endl;
                clearEndpointBit(endpointStallSend);
            }

            if(rm.endpoint0Control & endpointRxSetup)
            {
                log::emit() << "Got Setup" << log::endl;

                SetupData data;
                for(uint8_t i=0;i<sizeof(data);i++)
                    reinterpret_cast<uint8_t*>(&data)[i]=rm.endpoint0Fifo;

                switch(data.bRequest)
                {
                    case( 0): sendStatus(data);
                             break;
                    case( 1): log::emit() << "Request: CLEAR_FEATURE" << log::endl;
                             break;
                    case( 2): log::emit() << "Request: Reserved" << log::endl;
                             break;
                    case( 3): log::emit() << "Request: SET_FEATURE" << log::endl;
                             break;
                    case( 4): log::emit() << "Request: Reserved" << log::endl;
                             break;
                    case( 5): setAddress(data);
                             break;
                    case( 6): sendDescriptor(data);
                             break;
                    case( 7): log::emit() << "Request: SET_DESCRIPTOR" << log::endl;
                             break;
                    case( 8): log::emit() << "Request: GET_CONFIGURATION" << log::endl;
                             break;
                    case( 9): setConfiguration(data);
                             break;
                    case(10): log::emit() << "Request: GET_INTERFACE" << log::endl;
                             break;
                    case(11): log::emit() << "Request: SET_INTERFACE" << log::endl;
                             break;
                    case(12): log::emit() << "Request: SYNC_FRAME" << log::endl;
                             break;
                    default : log::emit() << "Request: Unknown" << log::endl;
                }
                return;
           }

//           log::emit() << "UDP: Something else happend" << log::endl;
        }
    };
}
}
}

extern "C" void UDP_Handler()
{
    arm_halib::driver::usb::Core::handleIRQ();
}
