#pragma once

#include <stdint.h>
#include <arm-halib/common/delay.h>

namespace arm_halib{
namespace driver
{
    struct USBCore
    {
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
                    };
                    struct{
                        uint8_t __pad3[0x50];
                        uint32_t endpoint0Fifo;
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

        struct DeviceDescriptor
        {
            uint8_t  bLength;
            uint8_t  bDescriptorType;
            uint16_t bcdUSB;
            uint8_t  bDeviceClass;
            uint8_t  bDeviceSubClass;
            uint8_t  bDeviceProtocol;
            uint8_t  bMaxPacketSize;
            uint16_t idVendor;
            uint16_t idProduct;
            uint16_t bcdDevice;
            uint8_t  iManufactor;
            uint8_t  iProduct;
            uint8_t  iSerialNumber;
            uint8_t  bNumConfigurations;
        } __attribute__((packed));

        struct ConfigurationDescriptor
        {
            uint8_t  bLength;
            uint8_t  bDescriptorType;
            uint16_t wTotalLength;
            uint8_t  bNumInterfaces;
            uint8_t  bConfigurationValue;
            uint8_t  iConfiguration;
            uint8_t  bmAttributes;
            uint8_t  bMaxPower;

        } __attribute__((packed));

        struct InterfaceDescriptor
        {
            uint8_t  bLength;
            uint8_t  bDescriptorType;
            uint8_t  bInterfaceNumber;
            uint8_t  bAlternateSetting;
            uint8_t  bInterfaceClass;
            uint8_t  bInterfaceSubClass;
            uint8_t  bInterfaceProtocol;
            uint8_t  iInterface;

        } __attribute__((packed));

        struct EndpointDescriptor
        {
            uint8_t  bLength;
            uint8_t  bDescriptorType;
            uint8_t  bEndpointAddress;
            uint8_t  bmAttributes;
            uint16_t wMaxPacketSize;
            uint8_t  bInterval;
            
        } __attribute__((packed));

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

        static DeviceDescriptor        devDesc;
        static ConfigurationDescriptor confDesc;
        static InterfaceDescriptor     ifDesc;
        static EndpointDescriptor      epDesc;

        USBCore()
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            log::emit() << "Starting USB bus" << log::endl;
            
            rm.clockController.systemClockEnable     = clockId;
            rm.clockController.peripheralClockEnable = peripheralId;
            rm.interruptController.interruptEnable   = peripheralId;
           
            rm.interruptClear  = clearInterrupts;
            rm.control         = pullUpConnect | txDisable;           
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

        static void sendDescriptor(SetupData& data)
        {
            log::emit() << "Request: GET_DESCRIPTOR" << log::endl;
            log::emit() << "Descriptor Type : " << (data.wValue>>8) << log::endl;;
            log::emit() << "Descriptor Index: " << (data.wValue&0xFF) << log::endl;
            log::emit() << "Lang-Id         : " << data.wIndex << log::endl;
            log::emit() << "Length          : " << data.wLength << log::endl;

           setEndpointBit(endpointDir);
           clearEndpointBit(endpointRxSetup);

           volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
           clearEndpointBit(endpointPacketReady);

           switch(data.wValue>>8)
           {
                case(0x01): for(uint8_t i=0;i<sizeof(DeviceDescriptor);i++)
                                rm.endpoint0Fifo=reinterpret_cast<uint8_t*>(&devDesc)[i];
                            break;
                case(0x02): for(uint8_t i=0;i<sizeof(ConfigurationDescriptor);i++)
                                rm.endpoint0Fifo=reinterpret_cast<uint8_t*>(&confDesc)[i];
                            for(uint8_t i=0;i<sizeof(InterfaceDescriptor);i++)
                                rm.endpoint0Fifo=reinterpret_cast<uint8_t*>(&ifDesc)[i];
                            break;
           };

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

            rm.globalState |= configured;
        }

        static void handleIRQ()
        {
            volatile RegMap& rm = *reinterpret_cast<volatile RegMap*>(0x0);
            if(rm.interruptStatus & busReset)
            {
                log::emit() << "USB Bus reset " << log::endl;
                rm.interruptClear   = busReset;
                rm.interruptEnable  = endpoint0Interrupt;
                setEndpointBit(endpointEnable);
                rm.control          = pullUpConnect;
                return;
            }

            if(rm.endpoint0Control & endpointRxSetup)
            {
                log::emit() << "Got Setup" << log::endl;

                SetupData data;
                for(uint8_t i=0;i<sizeof(data);i++)
                    reinterpret_cast<uint8_t*>(&data)[i]=rm.endpoint0Fifo;

                switch(data.bRequest)
                {
                    case( 0): log::emit() << "Request: GET_STATUS" << log::endl;
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

USBCore::DeviceDescriptor USBCore::devDesc ={
                sizeof(DeviceDescriptor),
                0x01,
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
USBCore::ConfigurationDescriptor USBCore::confDesc ={
                sizeof(ConfigurationDescriptor),
                0x02,
                sizeof(ConfigurationDescriptor)+sizeof(InterfaceDescriptor),//+sizeof(EndpointDescriptor),
                1,
                1,
                0,
                0x80,
                50
            };
USBCore::InterfaceDescriptor USBCore::ifDesc ={
                sizeof(InterfaceDescriptor),
                0x04,
                0,
                0,
                0xFF,
                0x0,
                0x0,
                0
            };
USBCore::EndpointDescriptor USBCore::epDesc ={
                sizeof(EndpointDescriptor),
                0x05,
            };
}
}

extern "C" void UDP_Handler()
{
    arm_halib::driver::USBCore::handleIRQ();
}
