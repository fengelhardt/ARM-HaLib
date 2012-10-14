#pragma once

#include <stdint.h>

namespace arm_halib{
namespace driver{
namespace usb
{
    struct Descriptors
    {
        enum DescriptorType
        {
            device           = 0x1,
            config           = 0x2,
            interface        = 0x4,
            endpoint         = 0x5,
            device_qualifier = 0x6
        };
    };

    typedef Descriptors::DescriptorType DescriptorType;

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

    struct DeviceQualifierDescriptor
    {
        uint8_t  bLength;
        uint8_t  bDescriptorType;
        uint16_t bcdUSB;
        uint8_t  bDeviceClass;
        uint8_t  bDeviceSubClass;
        uint8_t  bDeviceProtocol;
        uint8_t  bMaxPacketSize;
        uint8_t  bNumConfigurations;
        uint8_t  bReserved;
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
        uint8_t  bNumEndpoints;
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
}
}
}
