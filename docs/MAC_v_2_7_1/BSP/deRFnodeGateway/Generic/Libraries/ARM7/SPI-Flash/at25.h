/**
 * @file at25.h
 *
 * @brief Header file for at25.c
 *
 * Defines Macros and data types, declares function prototypes required
 * for the AT25 Flash driver implementation.
 *
 * $Id: at25.h,v 1.1.4.2 2011/07/14 18:21:56 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */

/** Prevent double inclusion */
#ifndef AT25_H
#define AT25_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <spid.h>
#include <at25family.h>

//------------------------------------------------------------------------------
//         Macros
//------------------------------------------------------------------------------

#define AT25_Name(pAt25)            ((pAt25)->pDesc->name)
#define AT25_Size(pAt25)            ((pAt25)->pDesc->size)
#define AT25_PageSize(pAt25)        ((pAt25)->pDesc->pageSize)
#define AT25_PageNumber(pAt25)      (AT25_Size(pAt25) / AT25_PageSize(pAt25))
#define AT25_SectorNumber           ((pAT25->pDesc->sectorCount))


//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

/// Device is protected, operation cannot be carried out.
#define AT25_ERROR_PROTECTED        1
/// Device is busy executing a command.
#define AT25_ERROR_BUSY             2
/// There was a problem while trying to program page data.
#define AT25_ERROR_PROGRAM          3
/// There was an SPI communication error.
#define AT25_ERROR_SPI              4


//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Describes a serial firmware flash device parameters.
//------------------------------------------------------------------------------
typedef struct _At25Desc {

   /// Device string name.
   const char *name;
   /// JEDEC ID of device.
   unsigned int jedecId;
   /// Size of device in bytes.
   const unsigned int size;
   /// Size of one page in bytes.
   const unsigned int pageSize;
   /// sector count instead of blocksize (since differing over memory space)
   const unsigned int sectorCount;
   /// array with offsets for sector start addresses
   const unsigned int* sectorStartingAddresses;

} At25Desc;

//------------------------------------------------------------------------------
/// Serial flash driver structure. Holds the current state of the driver, 
/// including the current command and the descriptor for the underlying device.
//------------------------------------------------------------------------------
typedef struct _At25 {

    /// Pointer to the underlying SPI driver.
    Spid *pSpid;
    /// Current SPI command sent to the SPI driver.
    SpidCmd command;
    /// Pointer to a descriptor for the serial firmware flash device.
    const At25Desc *pDesc;
    /// Command buffer.
    unsigned int pCmdBuffer[2];

} At25;

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

extern void AT25_Configure(At25 *pAt25, Spid *pSpid, unsigned char cs);

extern unsigned char AT25_SendCommand(
    At25 *pAt25,
    unsigned char cmd,
    unsigned char cmdSize,
    unsigned char *pData,
    unsigned int dataSize,
    unsigned int address,
    SpidCallback callback,
    void *pArgument);

extern unsigned char AT25_IsBusy(At25 *pAt25);

extern const At25Desc* AT25_FindDevice(
    At25 *pAt25,
    unsigned int jedecId);

#endif /* AT25_H */

