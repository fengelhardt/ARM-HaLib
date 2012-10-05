/**
 * @file at25.c
 *
 * @brief Implementation of an AT25 Family flash device driver.
 *
 * Inspired from the existing implementations for AT26/AT46 device drivers.
 *
 * $Id: at25.c,v 1.1.4.2 2011/07/14 18:21:56 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "at25.h"
#include <board.h>
#include <utility/assert.h>

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------

/// SPI clock frequency used in Hz.
#define SPCK                    1000000

/// SPI chip select configuration value.
#define CSR                     (AT91C_SPI_NCPHA | \
                                SPID_CSR_DLYBCT(BOARD_MCK, 100) | \
                                SPID_CSR_DLYBS(BOARD_MCK, 5) | \
                                SPID_CSR_SCBR(BOARD_MCK, SPCK))

/// Number of recognized dataflash.
#define NUMDATAFLASH            (sizeof(at25Devices) / sizeof(At25Desc))

#define DECLARE_SECTORS(x)      static const unsigned int sectors_##x[] = x
#define USE_SECTORS(x)          &sectors_##x[0]


//------------------------------------------------------------------------------
//         Internal variables
//------------------------------------------------------------------------------

/// generate a variable per each known flash device
DECLARE_SECTORS(AT25DF021_SECTORS);
DECLARE_SECTORS(AT25DF041_SECTORS);

/// Array of recognized serial dataflash chips.
static const At25Desc at25Devices[] = {
    // name,        Jedec ID,   size,     page size  , sectors, sector Offset
    {"AT25DF021"  , 0x0000431F, 256*1000, 256        , 3      , USE_SECTORS(AT25DF021_SECTORS) },
    {"AT25DF041"  , 0x0001441F, 512*1000, 256        , 11     , USE_SECTORS(AT25DF041_SECTORS) }
};


//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Initializes an AT25 driver instance with the given SPI driver and chip
/// select value.
/// \param pAt25  Pointer to an AT25 driver instance.
/// \param pSpid  Pointer to an SPI driver instance.
/// \param cs  Chip select value to communicate with the serial flash.
//------------------------------------------------------------------------------
void AT25_Configure(At25 *pAt25, Spid *pSpid, unsigned char cs)
{
    SpidCmd *pCommand;

    SANITY_CHECK(pAt25);
    SANITY_CHECK(pSpid);
    SANITY_CHECK(cs < 4);

    // Configure the SPI chip select for the serial flash
    SPID_ConfigureCS(pSpid, cs, CSR);

    // Initialize the AT25 fields
    pAt25->pSpid = pSpid;
    pAt25->pDesc = 0;

    // Initialize the command structure
    pCommand = &(pAt25->command);
    pCommand->pCmd = (unsigned char *) pAt25->pCmdBuffer;
    pCommand->callback = 0;
    pCommand->pArgument = 0;
    pCommand->spiCs = cs;    
}

//------------------------------------------------------------------------------
/// Returns 1 if the serial flash driver is currently busy executing a command;
/// otherwise returns 0.
/// \param pAt25  Pointer to an AT25 driver instance.
//------------------------------------------------------------------------------
unsigned char AT25_IsBusy(At25 *pAt25)
{
    return SPID_IsBusy(pAt25->pSpid);
}
    
//------------------------------------------------------------------------------
/// Sends a command to the serial flash through the SPI. The command is made up
/// of two parts: the first is used to transmit the command byte and optionally,
/// address and dummy bytes. The second part is the data to send or receive.
/// This function does not block: it returns as soon as the transfer has been
/// started. An optional callback can be invoked to notify the end of transfer.
/// Return 0 if successful; otherwise, returns AT25_ERROR_BUSY if the AT25
/// driver is currently executing a command, or AT25_ERROR_SPI if the command
/// cannot be sent because of a SPI error.
/// \param pAt25  Pointer to an AT25 driver instance.
/// \param cmd  Command byte.
/// \param cmdSize  Size of command (command byte + address bytes + dummy bytes).
/// \param pData Data buffer.
/// \param dataSize  Number of bytes to send/receive.
/// \param address  Address to transmit.
/// \param callback  Optional user-provided callback to invoke at end of transfer.
/// \param pArgument  Optional argument to the callback function.
//------------------------------------------------------------------------------
unsigned char AT25_SendCommand(
    At25 *pAt25,
    unsigned char cmd,
    unsigned char cmdSize,
    unsigned char *pData,
    unsigned int dataSize,
    unsigned int address,
    SpidCallback callback,
    void *pArgument)

{
    SpidCmd *pCommand;
    
    SANITY_CHECK(pAt25);

    // Check if the SPI driver is available
    if (AT25_IsBusy(pAt25)) {
    
        return AT25_ERROR_BUSY;
    }
    
    // Store command and address in command buffer
    pAt25->pCmdBuffer[0] = (cmd & 0x000000FF)
                           | ((address & 0x0000FF) << 24)
                           | ((address & 0x00FF00) << 8)
                           | ((address & 0xFF0000) >> 8);

    // Update the SPI transfer descriptor
    pCommand = &(pAt25->command);
     pCommand->cmdSize = cmdSize;
     pCommand->pData = pData;
     pCommand->dataSize = dataSize;
     pCommand->callback = callback;
     pCommand->pArgument = pArgument;
    
     // Start the SPI transfer
     if (SPID_SendCommand(pAt25->pSpid, pCommand)) {

         return AT25_ERROR_SPI;
     }
 
     return 0;
}

//------------------------------------------------------------------------------
/// Tries to detect a serial firmware flash device given its JEDEC identifier.
/// The JEDEC id can be retrieved by sending the correct command to the device.
/// Returns the corresponding AT25 descriptor if found; otherwise returns 0.
/// \param pAt25  Pointer to an AT25 driver instance.
/// \param jedecId  JEDEC identifier of device.
//------------------------------------------------------------------------------
const At25Desc * AT25_FindDevice(At25 *pAt25, unsigned int jedecId)
{
    unsigned int i = 0;

    SANITY_CHECK(pAt25);

    // Search if device is recognized
    pAt25->pDesc = 0;
    while ((i < NUMDATAFLASH) && !(pAt25->pDesc)) {
    
        if (jedecId == at25Devices[i].jedecId) {

            pAt25->pDesc = &(at25Devices[i]);
        }

        i++;
    }

    return pAt25->pDesc;
}

