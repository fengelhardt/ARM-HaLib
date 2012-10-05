/**
 * @file flash.c
 *
 * @brief  Implementation of an AT25 serial flash library
 *
 * Developed for At25DF041A Flash devices, it should be also working for
 * related devices from same family.
 *
 * $Id: flash.c,v 1.1.4.2 2011/07/14 18:21:56 dam Exp $
 *
 * @author    dresden elektronik: http://www.dresden-elektronik.de
 * @author    Support email: support@dresden-elektronik.de
 *
 * Copyright (c) 2011, Dresden Elektronik All rights reserved.
 *
 * Licensed under Dresden Elektronik Limited License Agreement --> deEULA.txt
 */


/* === INCLUDES ============================================================ */
#include <stdio.h>
#include <aic/aic.h>
#include <dbgu/dbgu.h>
#include <pio/pio.h>
#include <drivers/spi-flash/spid.h>
#include <utility/assert.h>
#include <utility/trace.h>

#include "bsp_config.h"             // physical device interconnection
#include "flash_interface.h"        // common library interface
#include "at25.h"                   // underlying driver

/* === TYPES =============================================================== */

/* === MACROS / DEFINES ==================================================== */

// Nearly each operation requires setting the 'Write Enable Latch'-Bit in the
// Status Register which mostly gets automatically reset after performing the
// respective command. For maximum safety, setting WEL should be done manually
// to prevent accidental data loss; wherever required this may be done
// automatically by uncommenting the following macro.
#define FORCE_WRITE_ENABLE

#ifdef FORCE_WRITE_ENABLE
#define AT25_FORCE_WEL()   AT25_ForceWEL()
#else
#define AT25_FORCE_WEL()   {}
#endif


/* === GLOBALS ============================================================= */
static Spid spid;           // SPI driver instance.
static At25 at25;           // Serial flash driver instance.
static uint8_t regval;      // commonly used for status register access
static uint8_t buf[256];    // commonly used buffer
static unsigned char error; // used for return codes in nearly all functions

/* === PROTOTYPES ========================================================== */
static void ISR_Spi(void);
#ifdef FORCE_WRITE_ENABLE
 static void AT25_ForceWEL(void);
#endif
static void AT25_WaitReady(void);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief  Interrupt service routine for the SPI peripheral.
 * Forwards the interrupt to the SPI driver.
 */
static void ISR_Spi(void)
{
    SPID_Handler(&spid);
}


// === interface implementation ===============================================

/**
 * @brief Initialize the SPI (interface/driver) and also the serial flash device
 */
flash_status_t AT25_Init(void)
{
    static Pin pins[] = { BOARD_AT25_SPI_PINS };

    PIO_Configure(pins, PIO_LISTSIZE(pins));
    AIC_ConfigureIT(BOARD_AT25_SPI_ID, 0, ISR_Spi);
    SPID_Configure(&spid, BOARD_AT25_SPI_BASE, BOARD_AT25_SPI_ID);
    AT25_Configure(&at25, &spid, BOARD_AT25_NPCS);
    AIC_EnableIT(BOARD_AT25_SPI_ID);
    TRACE_INFO("SPI and AT25 drivers initialized\n\r");
    return FL_SUCCESS;
}


/**
 * @brief Checks, whether the connected flash memory is of AT25/AT26-series.
 * @return true, if any AT25-series Flash detected, otherwise false
 */
bool AT25_Identify(void)
{
    uint32_t jedecId;

    // Read the JEDEC ID of the device
    AT25_ReadJedecID(&jedecId);

    // lookup in table of known devices, setup local descriptor
    if(!AT25_FindDevice(&at25, (unsigned int)jedecId)) {

        TRACE_ERROR("Failed to recognize the device (JEDEC ID is 0x%08X).\n\r", (unsigned int)jedecId);
    } else
    {
        TRACE_INFO("%s serial flash detected\n\r", AT25_Name(&at25));
        TRACE_INFO("Flash has: %d pages with %d bytes each \n\r",
                        AT25_PageNumber(&at25), AT25_PageSize(&at25));
    }

    // check if device is from AT25/AT26 series
    if(  ((jedecId&0x000000FF)==0x1F)               // ATMEL
      && ((((jedecId&0x0000FF00)>>8)&0xE0)==0x40)   // AT25/26-series
      )

      return true;

    return false;
}


#ifdef AT25_FORCE_WEL
/**
 * @brief Forces setting of the 'Write Enable Latch' Bit in Status Register.
 * This is necessary before any erase, program, (un)protect or write status
 * register command.
 */
static void AT25_ForceWEL(void)
{
    // check if write enabled, otherwise force
    regval = AT25_ReadStatusRegister();

    if((regval & AT25_SR_WEL_MASK) != AT25_SR_WEL_ENABLED_MASK) {

        // issue command
        error = AT25_SendCommand(&at25, AT25_WRITE_ENABLE_CMD, 1, 0, 0, 0, 0, 0);
        ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

        // wait until transaction completed
        while(AT25_IsBusy(&at25));
    }

}
#endif


/**
 * @brief Waits for the flash device to become ready to accept new commands.
 * Stops polling the status register at least after certain given cycles
 * (does not block forever).
 */
static void AT25_WaitReady()
{
    uint32_t i=0;

    do {
        regval = AT25_ReadStatusRegister();
        if( (regval & AT25_SR_RDBUSY_MASK) == AT25_SR_RDYBSY_READY_MASK )
            break;
        i++;
    } while(i<0x00FFFFFF);
}


// === read commands (chapter 7) ==============================================

/**
 * @brief Sequentially read up to 256 bytes from a given start address.
 *
 * @param   addr - starting address
 *          pData - user provided area to read to
 *          len - number of bytes to read, (truncated) to 256
 */
void AT25_ReadArray(uint32_t addr, uint8_t *pData, uint16_t len)
{
    // sanity check
    if(len>256)
        len=256;

    // Start a read operation: send command byte +3 address bytes +1 dummy byte,
    // read len bytes
    error = AT25_SendCommand(&at25, AT25_READ_ARRAY_CMD_1,
                                1+3+1, pData, len, (addr & 0x00FFFFFF), 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));
}


// === program and erase commands (chapter 8) =================================

/**
 * @brief Programs the supplied data into a given page which must have been
 * erased previously. Data will be wrapped if end of page gets reached (i.e if
 * starting address is not aligned to a page border)!
 *
 * IMPORTANT REMARK:
 * Due to implementation of the AT91Lib's SPI_SendCommand, while writing out
 * the data buffer byte by byte, the transceiver reads back byte by byte too.
 * This results in _overwriting_ the originating data buffer completely
 * (with 0xFF).
 *
 * @param   addr - starting address to be programmed
 *          pData - points to the data to be written
 *          len - length of data area to be written (0<len<=256)
 *
 * @return  FL_SUCCESS if programming succeeded
 *          FL_ZERO_LEN if no data provided
 *          FL_NOT_ERASED if page was not erased before
 *          FL_PROTECTED if sector belonging to page is protected
 *          FL_ERROR if programming failed otherwise
 */
flash_status_t AT25_ProgramPage(uint32_t addr, uint8_t *pData, uint16_t len)
{
    uint8_t cnt;

    // at least one byte must be written
    if(len==0)
        return FL_ZERO_LEN;

    // sanity check
    if(len>256)
        len = 256;

    // previously check if associated sector locked
    if(SECTOR_PROTECTED == AT25_ReadProtectionRegister(addr))
        return FL_PROTECTED;

    // the page must have been erased previously, check if fulfilled
    // programming itself still works if not erased before,
    // but read and written data will be inconsistent
    AT25_ReadArray(addr & 0xFFFFFF00, buf, 256);
    for(cnt=0; cnt<0xFF; cnt++) {
        if(buf[cnt]!=0xFF)
            return FL_NOT_ERASED;
    }

    AT25_FORCE_WEL();

    // issue Program page command, send 3 address bytes
    error = AT25_SendCommand(&at25, AT25_BYTE_PAGE_PROGRAM_CMD,
                        1+3, pData, len, (addr & 0x00FFFFFF), 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();

    // WEL gets erased always (even if failed)

    // check for programming/erase errors
    regval = AT25_ReadStatusRegister();
    if( (regval & AT25_SR_EPE_MASK) == AT25_SR_EPE_ERROR_MASK )
        return FL_ERROR;

    return FL_SUCCESS;
}


/**
 * @brief Starts a sequential programming procedure where only one byte can be
 * written on each invocation. There will be no wrap-around when reaching the
 * uppermost flash memory address. The memory area to write to must have been
 * erased before.
 *
 * @param   addr - starting address to be programmed
 *          data - the (first) data byte to be written
 *
 * @return  FL_SUCCESS if programming succeeded
 *          FL_PROTECTED if sector belonging to address is protected
 *          FL_NOT_ERASED if page has not been erased previously
 *          FL_ERROR if programming failed otherwise
 */
flash_status_t AT25_SequentialProgramStart(uint32_t addr, uint8_t data)
{
    // the amount of following bytes is yet unknown, but checking if the first
    // byte is within an erased area may be an indication for the following ones
    AT25_ReadArray(addr,buf,1);
    if(*buf!=0xFF)
        return FL_NOT_ERASED;

    // previously check if associated sector locked
    if(SECTOR_PROTECTED == AT25_ReadProtectionRegister(addr))
        return FL_PROTECTED;

    AT25_FORCE_WEL();

    // issue command, send 3 address bytes and program 1st byte
    error = AT25_SendCommand(&at25, AT25_SEQ_PROGRAMMING_MODE_CMD_1,
                                1+3+1, &data, 1, (addr & 0x00FFFFFF), 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();

    // WEL bit stays set as long as AT25_WRITE_DISABLE_CMD is not executed,
    // except if sector was protected - then it gets erased immediately
    // after write attempt

    // check for programming/erase errors
    regval = AT25_ReadStatusRegister();
    if( (regval & AT25_SR_EPE_MASK) == AT25_SR_EPE_ERROR_MASK )
           return FL_ERROR;

    return FL_SUCCESS;
}


/**
 * @brief Writes a byte while being in sequential programming mode.
 * Only one byte can be written on each invocation, there will be no wrap-around
 * upon reaching the uppermost flash memory address. Here programming will be
 * terminated automatically.
 *
 * @param   data - the data byte to be written
 *
 * @return  FL_SUCCESS if programming succeeded
 *          FL_PROTECTED if sector belonging to page was protected
 *          FL_PAGE_BORDER_REACHED if sequential programming mode was left
 *            since page border has been reached
 *          FL_WRONG_WORKING_MODE if device is operating in byte/page programming mode
 *          FL_ERROR if programming failed otherwise
 */
flash_status_t AT25_SequentialProgramByte(uint8_t data)
{
    // determine if sequential programming mode already entered
    regval = AT25_ReadStatusRegister();
    if( (regval&AT25_SR_SPM_MASK) != AT25_SR_SPM_SEQUENTIAL_MASK )
        return FL_WRONG_WORKING_MODE;

    // it is impossible to check, if desired region is erased since the flash device
    // delays write operations (at last until sequential programming mode is left)

    // reasserting WEL bit is not necessary

    // issue command, program one byte
    error = AT25_SendCommand(&at25, AT25_SEQ_PROGRAMMING_MODE_CMD_1,
                                1+1, &data, 1, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();

    // if the SPM mode flag was reset, the page border has been reached
    regval = AT25_ReadStatusRegister();
    if( (regval&AT25_SR_SPM_MASK) != AT25_SR_SPM_SEQUENTIAL_MASK)
        return FL_PAGE_BORDER_REACHED;

    // WEL bit in status register stays set as long as AT25_WRITE_DISABLE_CMD
    // is not executed, except if sector was protected - then it gets erased
    // immediately after write attempt
    if( (regval&AT25_SR_WEL_MASK) != AT25_SR_WEL_ENABLED_MASK)
        return FL_PROTECTED;

    // check for programming/erase errors
    if( (regval & AT25_SR_EPE_MASK) == AT25_SR_EPE_ERROR_MASK )
        return FL_ERROR;

    return FL_SUCCESS;
}


/**
 * @brief leaves the sequential programming mode
 */
void AT25_SequentialProgramLeave(void)
{
    // issue command
    error = AT25_SendCommand(&at25, AT25_WRITE_DISABLE_CMD, 1, 0, 0, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();
}


/**
 * @brief erases a given block entire Flash memory (sets all bits to '1')
 * requires that the WEL bit has been set by executing WRITE_ENABLE_CMD previously
 *
 * @param   addr  - the starting address of the block
 *          blksz - the desired blocksize
 *
 * @return  FL_SUCCESS if erasing succeeded
 *          FL_PROTECTED if any of the affected pages were protected
 *          FL_ERROR if no affected page was protected, but erasing failed
 */
flash_status_t AT25_BlockErase(blocksize_t blksz, uint32_t addr)
{
    uint8_t cmd;

    // previously check if associated sector locked
    if(SECTOR_PROTECTED == AT25_ReadProtectionRegister(addr))
        return FL_PROTECTED;

    // prepare command
    switch(blksz)
    {
        case BLOCKSIZE_64K: cmd = AT25_BLOCK_ERASE_64K_CMD; break;
        case BLOCKSIZE_32K: cmd = AT25_BLOCK_ERASE_32K_CMD; break;
        case BLOCKSIZE_4K:
        default:            cmd = AT25_BLOCK_ERASE_4K_CMD; break;
    }

    AT25_FORCE_WEL();

    // issue Command, send 3 address bytes
    error = AT25_SendCommand(&at25, cmd, 1+3, 0, 0, (addr & 0x00FFFFFF), 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();

    // WEL gets erased always (even if failed)

    // check for programming/erase errors
    regval = AT25_ReadStatusRegister();
    if( (regval & AT25_SR_EPE_MASK) == AT25_SR_EPE_ERROR_MASK )
        return FL_ERROR;

    return FL_SUCCESS;
}


/**
 * @brief erases the entire Flash memory (sets all bits to '1')
 * requires that the WEL bit has been set by executing WRITE_ENABLE_CMD previously
 *
 * @return  FL_SUCCESS if erasing succeeded
 *          FL_PROTECTED if any of the devices pages were protected
 *          FL_ERROR if no affected sector was protected, but erasing failed
 */
flash_status_t AT25_ChipErase(void)
{
    regval = AT25_ReadStatusRegister();
    if((regval & AT25_SR_SWP_MASK) != AT25_SR_SWP_PROTNONE_MASK)
    {
        TRACE_WARNING("%s: Device is protected.\n\r",__FUNCTION__);
        return FL_PROTECTED;
    }

    // enable critical write operation
    AT25_FORCE_WEL();

    // issue command, neither send address nor data bytes
    error = AT25_SendCommand(&at25, AT25_CHIP_ERASE_CMD_1, 1, 0, 0, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();

    // WEL gets erased always (even if failed)

    // check for programming/erase errors
    regval = AT25_ReadStatusRegister();
    if( (regval & AT25_SR_EPE_MASK) == AT25_SR_EPE_ERROR_MASK )
        return FL_ERROR;

    return FL_SUCCESS;
}


// === protection commands and features (chapter 9) ===========================

/**
 * @brief Enables the write mode.
 */
void AT25_WriteEnable(void)
{
    // issue command
    error = AT25_SendCommand(&at25, AT25_WRITE_ENABLE_CMD, 1, 0, 0, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();
}


/**
 * @brief Disables the write mode.
 */
void AT25_WriteDisable(void)
{
    // issue command
    error = AT25_SendCommand(&at25, AT25_WRITE_DISABLE_CMD, 1, 0, 0, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();
}


/**
 * @brief Protects a sector belonging to a given address
 *
 * @param   addr - any address within desired sector
 *
 * @return  FL_SUCCESS if protecting succeeded
 *          FL_ERROR if SPRL forbids protecting of sectors
 */
flash_status_t AT25_ProtectSector(uint32_t addr)
{
    // check if SPRL is set (=protecting forbidden)
    regval = AT25_ReadStatusRegister();
    if((regval & AT25_SR_SPRL_MASK) == AT25_SR_SPRL_LOCKED_MASK)
        return FL_ERROR;

    AT25_FORCE_WEL();

    // issue command, send 3 address bytes, do not read anything
    error = AT25_SendCommand(&at25, AT25_SECTOR_PROTECT_CMD,
                                1+3, 0, 0, (addr & 0x00FFFFFF), 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();

    // WEL gets always reset, so it may not indicate success/failure

    return FL_SUCCESS;
}


/**
 * @brief Unprotects a sector belonging to a given address.
 *
 * @param   addr - any address within desired sector
 *
 * @return  FL_SUCCESS if unprotecting succeeded
 *          FL_ERROR if SPRL forbids unprotecting of sectors
 */
flash_status_t AT25_UnprotectSector(uint32_t addr)
{
    // check if SPRL is set (=unprotecting forbidden)
    regval = AT25_ReadStatusRegister();
    if((regval & AT25_SR_SPRL_MASK) == AT25_SR_SPRL_LOCKED_MASK)
        return FL_ERROR;

    AT25_FORCE_WEL();

    // issue command, send 3 address bytes, do not read anything
    error = AT25_SendCommand(&at25, AT25_SECTOR_UNPROTECT_CMD,
                                1+3, 0, 0, (addr & 0x00FFFFFF), 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();

    // WEL gets always reset, so it may not indicate success/failure

    return FL_SUCCESS;
}


/**
 * @brief Globally protects all sectors,
 * leave the SPRL Bit (global lock bit) as it is.
 */
void AT25_GlobalProtect(void)
{
    // leave SPRL bit untouched (see datasheet, p.19)
    AT25_WriteStatusRegister(0x7F);
}


/**
 * @brief Globally unprotects all sectors,
 * leaves the SPRL Bit (global lock bit) as it is.
 */
void AT25_GlobalUnprotect(void)
{
    // leave SPRL bit untouched (see datasheet, p.19)
    AT25_WriteStatusRegister(0x00);
}


/**
 * @brief Sets the SPRL bit in status register without changing the
 * sector protection states.
 */
void AT25_SetSPRL(void)
{
    // leave individual sector protections untouched (see datasheet, p.20)
    AT25_WriteStatusRegister(0xF0);
}


/**
 * @brief Clears the SPRL bit in status register without changing the
 * sector protection states.
 */
void AT25_ClearSPRL(void)
{
    // leave individual sector protections untouched (see datasheet, p.20)
    AT25_WriteStatusRegister(0x0F);
}


/**
 * @brief Checks whether the sector belonging to a given address is locked or not.
 *
 * @param addr - any address within desired sector
 *
 * @return SECTOR_PROTECTED if sector associated to the address is locked,
 *         SECTOR_UNPROTECTED otherwise
 */
protect_status_t AT25_ReadProtectionRegister(uint32_t addr)
{
    unsigned char status = 0;

    // issue a read command, send 3 dummy address bytes, read 1 byte
    error = AT25_SendCommand(&at25, AT25_READ_SECTOR_PROTECT_REG_CMD,
                                1+3, (unsigned char *) &status, 1, (addr & 0x00FFFFFF), 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    return status?SECTOR_PROTECTED:SECTOR_UNPROTECTED;
}


// === status register commands (chapter 10) ==================================

/**
 * @brief Reads out the status register (1 byte).
 * Suitable for polling since register content gets updated permanently.
 *
 * @return the current status registers content
 */
uint8_t AT25_ReadStatusRegister(void)
{
    unsigned char status = 0;

    // issue a read command
    error = AT25_SendCommand(&at25, AT25_READ_STATUS_REGISTER_CMD,
                                1, (unsigned char *) &status, 1, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    return status;
}


/**
 * @brief Writes a new value to the device status register.
 *
 * @param data - the desired status register value
 */
void AT25_WriteStatusRegister(uint8_t data)
{
    AT25_FORCE_WEL();

    // issue a write command
    error = AT25_SendCommand(&at25, AT25_WRITE_STATUS_REGISTER_CMD,
                                1, (unsigned char*) &data, 1, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();

    // normally we should evaluate the status register
    // (check if WEL was reset; WEL reset <-> flash write protected)
    // but currently there is a permanent pullup.
}


// === other commands and functions (chapter 11) ==============================

/**
 * @brief Read out the manufacturer and device IDs.
 *
 * @param pData - pointer to location where to write the Jedec ID
 */
void AT25_ReadJedecID(uint32_t *pParam)
{
    SANITY_CHECK(pParam);       // ensure being not NULL

    // issue command
    error = AT25_SendCommand(&at25, AT25_READ_DEVICE_IDS_CMD,
                                1, (unsigned char*) pParam, 4, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();
}


/**
 * @brief Put the flash into deep power down mode.
 */
void AT25_DeepPowerDown(void)
{
    // issue command
    error = AT25_SendCommand(&at25, AT25_DEEP_POWER_DOWN_CMD, 1, 0, 0, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // do NOT poll the status register here, since device is asleep
    // and reacts only on a wakeup command
}


/**
 * @brief Resume from deep power down mode. This is the only command which
 * is not ignored (executed) when sleeping!
 */
void AT25_DeepPowerDownResume(void)
{
    // issue command
    error = AT25_SendCommand(&at25, AT25_DEEP_POWER_RESUME_CMD, 1, 0, 0, 0, 0, 0);
    ASSERT(!error, "-F- %s: Could not issue command.\n\r", __FUNCTION__);

    // wait until transaction completed
    while(AT25_IsBusy(&at25));

    // wait for device becoming ready again
    AT25_WaitReady();
}


// === helper function(s) =====================================================

/**
 * @brief Helper function determining (and printing out) each sectors
 * protection state.
 */
void AT25_ShowSectorProtectionState(void)
{
    uint8_t i;

    // sanity check
    if(!at25.pDesc)
        return;

    printf("=====================================================================\r\n");
    printf("sector protection state ('x'=protected): \r\n");

    for(i=0; i<at25.pDesc->sectorCount; i++)
        printf(" %02d", i);
    printf("\r\n");

    for(i=0; i<11; i++)
        printf("  %c", AT25_ReadProtectionRegister(at25.pDesc->sectorStartingAddresses[i])?'x':'-');
    printf("\r\n");
    printf("=====================================================================\r\n");
}


/**
 * @brief Print the status register interpretation in a human-readable format.
 *
 * @param data - the register value to be interpret
 */
void AT25_InterpretStatusRegister(uint8_t data)
{
    printf("=====================================================================\r\n");
    printf("Status:=0x%02X \r\n", data);
    printf(" Sector Protection Registers: %s\r\n", (data&AT25_SR_SPRL_MASK)?"locked":"unlocked");
    printf(" Programming Mode           : %s\r\n", (data&AT25_SR_SPM_MASK)?"sequential":"byte/page");
    printf(" Erase/Program Error        : %s\r\n", (data&AT25_SR_EPE_MASK)?"error":"success");
    printf(" Write Protection Pin       : %s\r\n", (data&AT25_SR_WPP_MASK)?"deasserted":"asserted");
    printf(" Software Protection Status : %s\r\n", (data&AT25_SR_SWP_MASK)?"sectors protected":"sectors unprotected");
    printf(" Write Enable Latch Status  : %s\r\n", (data&AT25_SR_WEL_MASK)?"enabled":"disabled");
    printf(" Ready/Busy Status          : %s\r\n", (data&AT25_SR_RDBUSY_MASK)?"busy":"ready");
    printf("=====================================================================\r\n");
}


/**
 * @brief Helper function dumping a given data area.
 *
 * @param   pData   - pointer to data which to dump
 *          len     - lenght of data area
 *          title   - optional description to be written out
 */
void dump_data(uint8_t* pData, uint16_t len, char* title)
{
    const uint8_t ENTRIES_PER_LINE = 16;
    uint16_t i, cnt=0;

    printf(" ## %s ## \r\n offset | data \r\n", title);

    while(1)
    {
        printf(" 0x%04X | ", cnt);
        for(i=0; i<ENTRIES_PER_LINE; i++)
        {
            printf("%02X ",*pData++);
            len--;
            if(len==0)
            {
                printf("\r\n");
                return;
            }
        }
        printf("\r\n");
        cnt+=ENTRIES_PER_LINE;
    }
}
