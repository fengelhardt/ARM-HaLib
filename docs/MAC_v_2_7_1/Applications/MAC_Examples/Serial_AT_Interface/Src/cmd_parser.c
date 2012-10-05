/**
 * @file sio_handler.c
 *
 * @brief Contains functions for parsing of the commands
 *
 * $Id: cmd_parser.c 28613 2011-09-20 06:56:06Z viswanadham.kotla $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "pal.h"
#include "return_val.h"
#include "cmd_parser.h"
#include "cmd_handler.h"

/* === MACROS ============================================================== */

#define MAX_RSP_SIZE            (1024)

/* === Globals ============================================================= */

/* Buffer for storing responses */
char rsp_buffer[MAX_RSP_SIZE];

/*
 * The pointer to the call-back function that response handler functions call to
 * print information responses.
 */
cmd_response_cb_t response_callback = 0;
/**
 * Table of extended commands
 * Note: 'Command' entry in the table must be a string containing the entire
 * command.
 */
static command_table_t extended_command_table[] =
{
    /*| Command  | Command Type  | Val | Min | Max | Handler function   |*/
    {    "+RESET", CMD_TYPE_EXEC ,    0,    0,    0,  mac_reset_handler },
    {      "+PIB", CMD_TYPE_EXEC ,    0,    0,    0,  mac_pib_handler },
    { "+AUTOADDR", CMD_TYPE_ALL  ,    0,    0,    0,  auto_addr_alloc_handler },
    {    "+ASSOC", CMD_TYPE_SET  ,    0,    0,    0,  mac_assoc_handler },
    {    "+START", CMD_TYPE_SET  ,    0,    0,    0,  mac_start_handler },
    { "+ASSOCRSP", CMD_TYPE_SET  ,    0,    0,    0,  mac_assoc_rsp_handler },
    {     "+SCAN", CMD_TYPE_SET  ,    0,    0,    0,  mac_scan_req_handler },
    {   "+DASSOC", CMD_TYPE_SET  ,    0,    0,    0,  mac_disassoc_handler },
    {     "+DATA", CMD_TYPE_SET  ,    0,    0,    0,  mac_data_req_handler},
    {    "+PURGE", CMD_TYPE_SET  ,    0,    0,    0,  mac_purge_req_handler},
    {     "+POLL", CMD_TYPE_SET  ,    0,    0,    0,  mac_poll_req_handler},
    {     "+SYNC", CMD_TYPE_SET  ,    0,    0,    0,  mac_synq_req_handler},
    {     "+RXEN", CMD_TYPE_SET  ,    0,    0,    0,  mac_rx_enable_req_handler}
};



/**
 * @brief Sends a response to the upper layer that gave the command
 *
 * This function executes a command according to info provided in the
 * command table and returns the result
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
void send_response(command_result_t result, char *rsp_str)
{
    if (response_callback)
    {
        response_callback(result, rsp_str);
    }
}



/**
 * @brief Executes a command from its command table entry
 *
 * This function executes a command according to info provided in the
 * command table and returns the result
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the command
 *
 * @return Result of execution of the command
 */
command_result_t execute_from_table(command_table_t *cmd_table,
                                    char **cmd_line,
                                    cmd_response_cb_t response_cb)
{
    int value;
    command_type_t cmd_type = get_cmd_type(cmd_line);
    char *cmd_ptr;

    if (!(cmd_type & cmd_table->type))
    {
        /* We don't support this operation for this command */
        return CMD_RESULT_ERROR;
    }

    switch (cmd_type)
    {
        case CMD_TYPE_EXEC:
        case CMD_TYPE_SET:
            if (!(cmd_table->value))
            {
                /* If there is no value to be set, call the handler */
                return cmd_table->handler(cmd_table, cmd_line, response_cb);
            }
            cmd_ptr = *cmd_line;
            value = get_number(cmd_line);
            if (cmd_ptr != *cmd_line)
            {
                /* Value was given in the command line so validate and set it */
                if ( (value > cmd_table->max) || (value  < cmd_table->min) )
                {
                    return CMD_RESULT_ERROR;
                }
                *cmd_table->value = value;
            }
            break;

        case CMD_TYPE_GET:
            if (!(cmd_table->value))
            {
                /* If GET is supported and there is no value stored, call execute */
                return cmd_table->handler(cmd_table, cmd_line, response_cb);
            }

            if (cmd_table->cmd[0] == 'S')
            {
                /* Change in format for S-register commands */
                sprintf(rsp_buffer, "%03d", *cmd_table->value);
            }
            else
            {
                sprintf(rsp_buffer, "%s: %d", cmd_table->cmd, *cmd_table->value);
            }
            response_cb(CMD_RESULT_PENDING, rsp_buffer);
            break;

        case CMD_TYPE_TEST:
            /* If the command contains a value, print the max & min values */
            if (cmd_table->value)
            {
                if (cmd_table->min == cmd_table->max)
                {
                    sprintf(rsp_buffer, "%s: (%d)", cmd_table->cmd, cmd_table->min);
                }
                else
                {
                    sprintf(rsp_buffer, "%s: (%d-%d)", cmd_table->cmd,
                            cmd_table->min, cmd_table->max);
                }
                response_cb(CMD_RESULT_PENDING, rsp_buffer);
            }
            /* Otherwise just return OK */
            break;
    }
    return CMD_RESULT_OK;
}



/**
 * @brief Executes an extended command
 *
 * This function looks for an extended command in the given command line and if
 * it matches, executes the command. If not, it returns ERROR.
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 *
 * @return Result of execution of the command
 */
command_result_t execute_ext_cmd(char **cmd_line, cmd_response_cb_t response_cb)
{
    command_table_t *cmd_table = 0;
    int i, cmd_len;
    char *cmd_ptr = *cmd_line;

    for (i = 0; i < sizeof(extended_command_table) / sizeof(command_table_t); i++)
    {
        char *cmd = extended_command_table[i].cmd;
        cmd_len = strlen(cmd);

        /*
         * The command is considered to be matching if the first 'cmd_len'
         * characters match with that of the command table entry and the next
         * character is not an alphabet.
         */
        if ( !(strncmp(cmd, cmd_ptr, cmd_len) || isalpha(cmd_ptr[cmd_len])) )
        {
            /* Match found */
            cmd_table = &extended_command_table[i];
            *cmd_line = cmd_ptr + cmd_len;
            /*
             * Store the pointer to the function that gets called to print the
             * information response.
             */
            response_callback = response_cb;
            /* Execute the command from the table */
            return execute_from_table(cmd_table, cmd_line, response_cb);
        }
    }

    return CMD_RESULT_ERROR;
}



/**
 * @brief Extracts a decimal number from the given command line
 *
 * This function looks for a number (decimal) in the given command string and
 * returns its value, if found. It also updates the command string pointer to
 * point to the end of the number.
 *
 * @param cmd_line Pointer to the command line string pointer
 *
 * @return the number as an integer
 */
int get_number (char **cmd_line)
{
    int value;
    char *cmd_ptr = *cmd_line;
    value = strtol((char const *)cmd_ptr, (char **)cmd_line, 0);
    return value;
}



/**
 * @brief Extracts a hexadecimal number from the given command line
 *
 * This function looks for a hexadecimal number in the given command string and
 * returns its value, if found. It also updates the command string pointer to
 * point to the end of the number.
 *
 * @param cmd_line Pointer to the command line string pointer
 *
 * @return the number as an integer
 */
int get_hex_number (char **cmd_line)
{
    int value;
    char *cmd_ptr = *cmd_line;
    value = strtol((char const *)cmd_ptr, (char **)cmd_line, 16);
    return value;
}



/**
 * @brief Extracts a hex byte from the given command line
 *
 * This function looks for a single byte hexadecimal number in the given command
 * string and returns its value, if found. It also updates the command string
 * pointer to point to the end of the number.
 *
 * @param cmd_line Pointer to the command line string pointer
 *
 * @return the number as an integer
 */
uint8_t get_hex_byte (char **cmd_line)
{
    char temp[] = {0, 0, 0}, *cmd_ptr = *cmd_line;
    int i;

    for (i = 0; i < sizeof(char) * 2; i++)
    {
        temp[i] = *cmd_ptr;
        if (isxdigit(temp[i]))
        {
            cmd_ptr++;
        }
    }

    *cmd_line = cmd_ptr;
    return strtol(temp, 0, 16);
}



/**
 * @brief Determines the type of the current command
 *
 * This function looks determines the type of the current command.
 * A command of the form atx[..x]=y is interpreted as a SET command.
 * A command of the form atx[..x]? is interpreted as a GET command.
 * A command of the form atx[..x]=? is interpreted as a TEST command.
 * A command of the form atx[..x] is interpreted as an EXECUTE command.
 * (x can be any character. The part given in brackets [] is optional.)
 *
 * @param cmd_line Pointer to the command line string pointer
 *
 * @return the command type CMD_TYPE_SET, CMD_TYPE_GET, CMD_TYPE_TEST
 * or CMD_TYPE_EXEC
 */
command_type_t get_cmd_type (char **cmd_line)
{
    command_type_t cmd_type = CMD_TYPE_EXEC;
    char *cmd_ptr = *cmd_line;

    switch (*cmd_ptr)
    {
        case '=':
            cmd_ptr++;
            if (*cmd_ptr == '?')
            {
                cmd_ptr++;
                cmd_type = CMD_TYPE_TEST;
            }
            else
            {
                cmd_type = CMD_TYPE_SET;
            }
            break;

        case '?':
            cmd_ptr++;
            cmd_type = CMD_TYPE_GET;
    }
    *cmd_line = cmd_ptr;
    return cmd_type;
}


/**
 * @brief Checks whether an additional argument is present on te command line
 *
 * This function looks for a comma (',') in the command line. If it is present
 * it means that there are more arguments coming up, so it returns 1. If not, it
 * returns 0.
 *
 * @param cmd_line Pointer to the command line string pointer
 *
 * @return 1 if another argument is present, 0 if otherwise
 */
uint8_t get_next_arg (char **cmd_line)
{
    if (**cmd_line == ',')
    {
        (*cmd_line)++;
        return 1;
    }
    return 0;
}
