#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

libusb_device_handle* handle=NULL;

void terminate();

void init()
{
    libusb_init(NULL);
    libusb_set_debug(NULL, 3);
    int res;
    do
    {
        do
            handle = libusb_open_device_with_vid_pid(NULL, 0, 0);
        while(!handle);
    
        printf("Found device %04x:%04x\n", 0, 0);

        res = libusb_claim_interface(handle, 0);
        if(res)
        {
            fprintf(stderr, "Error: could not claim interface 0\n");
        }
        else
            printf("claimed interface 0\n");
    }
    while(res);
}

void terminate()
{
    if(handle)
        libusb_close(handle);

    init();

//    libusb_exit(NULL);
//    exit(-1);
}

void reportError(int error)
{
    fprintf(stderr, "Error: ");
    switch(error)
    {
        case(LIBUSB_ERROR_TIMEOUT)  : fprintf(stderr, "timed out");
                                      break;
        case(LIBUSB_ERROR_PIPE)     : fprintf(stderr, "endpoint halted");
                                      break;
        case(LIBUSB_ERROR_OVERFLOW) : fprintf(stderr, "too much data");
                                      break;
        case(LIBUSB_ERROR_NO_DEVICE): fprintf(stderr, "device disconnected");
                                      break;    
    }
    fprintf(stderr, "\n");
}

int main()
{
    uint8_t buffer[65];
    buffer[64]='\0';

        init();

    while(1)
    {
        int transferred = 0;
        int res = libusb_interrupt_transfer( handle, 0x83, buffer, 64, &transferred, 0);
        if(res)
        {
            reportError(res);
            terminate();
        }
        buffer[transferred]='\0';
        printf("[%02u]: %s", transferred, buffer);
    }
    return 0;
}
