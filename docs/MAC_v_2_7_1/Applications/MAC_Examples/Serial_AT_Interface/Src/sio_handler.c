/**
 * @file sio_handler.c
 *
 * @brief SIO service implementation for RDK230.
 *
 * $Id: sio_handler.c 28944 2011-10-18 11:27:09Z viswanadham.kotla $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifdef SIO_HUB

/* === INCLUDES ============================================================ */

#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "pal.h"
#include "return_val.h"
#include "bmm.h"
#include "qmm.h"
#include "tal.h"
#include "ieee_const.h"
#include "private_const.h"
#include "stack_config.h"
#include "mac_msg_const.h"
#include "mac_api.h"
#include "mac_msg_types.h"
#include "mac_data_structures.h"
#include "mac_internal.h"
#include "mac.h"
#include "pal.h"
#include "sio_handler.h"
#include "cmd_parser.h"

/* === MACROS ============================================================== */

/* UART communication framing */

/**
 * Initial state
 */
#define SIO_STATE_READY                 (1)

/**
 * 'A' has been received from the command line
 */
#define SIO_STATE_A                     (2)

/**
 * "AT" has been received, waiting for the cmd line itself to be received
 */
#define SIO_STATE_CMD                   (3)

/**
 * A command line has been received and is ready for execution
 */
#define SIO_STATE_EXECUTE               (4)

/**
 * Waiting for the result of a prevoius command
 */
#define SIO_STATE_RESULT                (5)

/**
 * Size of the RX buffer
 */
#define SIO_RX_BUF_SIZE                 (400)

#define DATA_BUF_SIZE                    (40)

/* === Types =============================================================== */

typedef struct result_code_table_tag
{
    command_result_t result_code;
    char *result_string;
} result_code_table_t;

/* === Globals ============================================================= */

/**
 * This is the receive buffer of the UART.
 */
static char sio_rx_buf[SIO_RX_BUF_SIZE];

/**
 * This pointer points to the next free element inside the
 * receive buffer of the UART.
 */
static char *sio_rx_ptr;

/**
 * This is the receiver state of the UART. (\ref SIO_STATE_READY,
 *   \ref SIO_STATE_A, \ref UART_RX_STATE_DATA, or
 *   \ref UART_RX_STATE_EOT)
 */
static volatile uint8_t sio_rx_state;

/**
 * This is the length of the message should be received.
 */
static uint8_t sio_rx_length;

/**
 * Buffer used to hold data received over UART
 */
static uint8_t data[DATA_BUF_SIZE];

/**
 * Number of bytes of data received
 */
static uint8_t data_length = 0;

/**
 * Index of the current byte of data being processed
 */
static uint8_t rx_index = 0;

/* AT Command related parameters that can be set from UI */
static uint8_t verbose = 1, echo = 1, quiet = 0;

/* S-register storage variables */
static uint8_t s0 = 0, s2 = 1, s3 = '\r', s4 = '\n', s5 = '\b';


/* Command Handlers - have to be declared before the tables are declared */
static command_result_t sreg_cmd_handler(command_table_t *table_entry,
                                         char **cmd_line,
                                         cmd_response_cb_t response_cb);

static command_result_t reset_cmd_handler(command_table_t *table_entry,
                                          char **cmd_line,
                                          cmd_response_cb_t response_cb);

static char *get_result_string(uint8_t result_code);

/**
 * Table of S-register commands
 * Note: S-Register entry in the table must be a string containing the
 * letter 'S' and the register number
 */
static command_table_t sreg_table[] =
{
    /* | S-Register | Command Type  | Value |  Min  |  Max    | */
    {           "S0", CMD_TYPE_VALUE,    &s0,      0,    255  },
    {           "S2", CMD_TYPE_VALUE,    &s2,      1,    255  },
    {           "S3", CMD_TYPE_VALUE,    &s3,      1,    255  },
    {           "S4", CMD_TYPE_VALUE,    &s4,      1,    255  },
    {           "S5", CMD_TYPE_VALUE,    &s5,      1,    32   }
};


/**
 * Table of basic commands
 * Note: 'Command' entry in the table must be a string containing ONLY one
 * character since all supported basic commands are single character commands
 */
static command_table_t basic_command_table[] =
{
    /* | Command | Command Type  |  Value    | Min | Max | Handler function  |*/
    {         "Q", CMD_TYPE_ALL  ,     &quiet,    0,    1,                 0 },
    {         "E", CMD_TYPE_ALL  ,      &echo,    0,    1,                 0 },
    {         "S", CMD_TYPE_EXEC ,          0,    0,    0,  sreg_cmd_handler },
    {         "Z", CMD_TYPE_EXEC ,          0,    0,    0, reset_cmd_handler },
    {         "V", CMD_TYPE_ALL  ,   &verbose,    0,    1,                 0 }
};

/**
 * Table of result codes and corresponding strings
 */
result_code_table_t result_code_table[] =
{
    {    CMD_RESULT_OK,      "OK"     },
    {    CMD_RESULT_ERROR,   "ERROR"  },
    {    CMD_RESULT_PENDING, "PENDING"}
};

/* === Prototypes ========================================================== */

static inline void process_incoming_sio_data(void);
static inline void process_command(void);
static command_result_t handle_nxt_command(char **cmd_line);
static command_result_t execute_basic_command(char **cmd_line,
                                              cmd_response_cb_t response_cb);
static void cmd_response_cb(command_result_t result, char *response_str);
static void sio_send_cmd_result(command_result_t result);
static inline void sio_send_string(char *s);
static inline void sio_send_char(uint8_t c);
static void sio_send_crlf(void);
static void sio_send_cr(void);

static void echo_string(char *s);
static void echo_char(char c);

/* === Implementation ====================================================== */

/**
 * @brief Initializes the SIO handler
 */
void init_sio(void)
{
    sio_rx_state = SIO_STATE_READY;
}



/**
 * @brief Reads and processes the bytes received
 */
void sio_task()
{
    /* Rx processing */
    if (data_length == 0)
    {
        /* No data to process, read the stream IO */
        rx_index = 0;
        data_length = pal_sio_rx(SIO_CHANNEL, data, 0xFF);
    }
    else    /* Data has been received, process the data */
    {
        /* Process each single byte */
        process_incoming_sio_data();
        data_length--;
        rx_index++;
    }

    /* Command Processing */
    if (sio_rx_state == SIO_STATE_EXECUTE)
    {
        process_command();
    }
}



/**
 * @brief State machine that handles command line input and editing
 *
 * This function accepts a valid command line (a sequence of characters starting
 * with "AT" and ending with "\r", ignoring case and all spaces in between) from
 * the user. It also includes proper handling for echo (on/off), backspace and
 * case insensitivity (converts all to uppercase). Once the entire command line
 * has been received, it proceeds to execute the command.
 */
static inline void process_incoming_sio_data(void)
{
    switch (sio_rx_state)
    {
        case SIO_STATE_READY:
            if (toupper(data[rx_index]) == 'A')
            {
                sio_rx_state = SIO_STATE_A;
                echo_char(data[rx_index]);
            }
            break;

        case SIO_STATE_A:
            switch (toupper(data[rx_index]))
            {
                case 'T':
                    sio_rx_state = SIO_STATE_CMD;
                    sio_rx_length = 0;
                    echo_char(data[rx_index]);
                    break;

                case 'A':
                    sio_rx_state = SIO_STATE_A;
                    echo_char(data[rx_index]);
                    break;

                case '/':
                    /* Redo last command */
                    sio_rx_state = SIO_STATE_EXECUTE;
                    sio_rx_ptr = sio_rx_buf;
                    sio_rx_buf[sio_rx_length++] = 0;
                    echo_char(data[rx_index]);
                    break;

                default:
                    sio_rx_state = SIO_STATE_READY;
            }
            break;

        case SIO_STATE_CMD:
            if (data[rx_index] == s3)
            {
                /* Return signals end of command */
                sio_rx_state = SIO_STATE_EXECUTE;
                sio_rx_buf[sio_rx_length++] = 0;
                sio_rx_ptr = sio_rx_buf;
                echo_char(data[rx_index]);

            }
            else if (data[rx_index] == s5)
            {
                /* Handling Backspace - send: bksp, space, bksp */
                if (sio_rx_length > 0)
                {
                    sio_rx_length--;
                    echo_string("\b \b");
                }
            }
            else if (data[rx_index] == ' ')
            {
                /* Ignore Spaces, just echo them */
            }
            else if (isprint(data[rx_index]))
            {
                sio_rx_buf[sio_rx_length++] = toupper(data[rx_index]);
                echo_char(data[rx_index]);
            }
            break;

        case SIO_STATE_RESULT:
        case SIO_STATE_EXECUTE:
            if (toupper(data[rx_index]) == 'A')
            {
                /* Cancel Execution if new command line is received */
                sio_rx_state = SIO_STATE_A;
                echo_char(data[rx_index]);
            }
            break;

        default:
            sio_rx_state = SIO_STATE_READY;
            break;
    }
}





/**
 * @brief Processes a single command received from the terminal
 *
 * This function executes the first command in the received command line and
 * according to the result, proceeds to the next command (if the previous
 * command executed successfully) or halts execution (in case of error).
 */
static inline void process_command(void)
{
    command_result_t result = handle_nxt_command(&sio_rx_ptr);

    switch (result)
    {
        case CMD_RESULT_OK:
            /* Remain in the state to execute the next command */
            break;

        case CMD_RESULT_PENDING:
            /* Wait for result */
            sio_rx_state = SIO_STATE_RESULT;
            break;

        case CMD_RESULT_ERROR:
            /* Error in command */
            sio_rx_state = SIO_STATE_READY;
            sio_send_cmd_result(CMD_RESULT_ERROR);
            break;

        case CMD_RESULT_FINISHED:
            /* No more commands to execute */
            sio_rx_state = SIO_STATE_READY;
            sio_send_cmd_result(CMD_RESULT_OK);
            break;

        default:
            sio_rx_state = SIO_STATE_READY;
            /* TODO: Remove Debug Code */
            while (1);
    }
}



/**
 * @brief Parses and executes the next command in a given command line
 *
 * This function searches through the given command line, executes the first
 * command it finds and returns the result. It also updates the pointer to point
 * to the end of the executed command.
 *
 * @param cmd_line Pointer to the command line string pointer
 *
 * @return Result of execution of the command
 */
static command_result_t handle_nxt_command(char **cmd_line)
{
    command_result_t result;

    if (**cmd_line == 0)
    {
        return CMD_RESULT_FINISHED;
    }

    result = execute_basic_command(cmd_line, cmd_response_cb);

    if (result == CMD_RESULT_OK)
    {
        return result;
    }
    else
    {
        return execute_ext_cmd(cmd_line, cmd_response_cb);
    }
}



/**
 * @brief Executes a basic command in the given command line
 *
 * This function searches through the given command line, executes the first
 * basic command it finds and returns the result. It also updates the pointer
 * to point to the end of the executed command and sends the responses, if any,
 * through the given callback function pointer.
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 *
 * @return Result of execution of the command
 */
static command_result_t execute_basic_command(char **cmd_line,
                                              cmd_response_cb_t response_cb)
{
    command_table_t *cmd_table = 0;
    int i;
    char *cmd_ptr = *cmd_line;

    if (*cmd_ptr == ';')
    {
        /* Semicolon signals end of the previous command.. skip it */
        *cmd_line = ++cmd_ptr;
    }

    for (i = 0 ; i < sizeof(basic_command_table) / sizeof(command_table_t); i++)
    {
        if ( *basic_command_table[i].cmd == *cmd_ptr )
        {
            cmd_table = &basic_command_table[i];
            *cmd_line = ++cmd_ptr;
            return execute_from_table(cmd_table, cmd_line, response_cb);
        }
    }
    return CMD_RESULT_ERROR;
}



/**
 * @brief This function handles sending of the responses for executed commands
 *
 * This function sends responses for executed commands over SIO and also
 * changes the state of the SIO state machine, if required, accoring to the
 * result of execution of the command.
 *
 * @param result Result of execution
 * @param response_str String containing the response to be printed
 */
static void cmd_response_cb(command_result_t result, char *response_str)
{
    /* Print the response string according to the verbosity settings */
    if (verbose)
    {
        sio_send_crlf();
    }

    sio_send_string(response_str);
    sio_send_crlf();

    switch (result)
    {
        case CMD_RESULT_OK:
            if (sio_rx_state == SIO_STATE_RESULT)
            {
                /* If the result is OK, start executing the next command */
                sio_rx_state = SIO_STATE_EXECUTE;
            }
            break;

        case CMD_RESULT_PENDING:
            /* Execution is still pending, so do nothing */
            break;

        case CMD_RESULT_FINISHED:
            /*
             * The entire command line has been executed so print response and
             * go back to ready state
             */
            sio_rx_state = SIO_STATE_READY;
            sio_send_cmd_result(CMD_RESULT_OK);
            break;

        default:
            /*
             * Execution failed so print error response and go back to ready
             * state
             */
            sio_rx_state = SIO_STATE_READY;
            sio_send_cmd_result(result);
            break;
    }
}



/**
 * @brief This function prints the result of execution of a command
 *
 * This function prints the result (eg. OK, ERROR or 0, 4 etc.) according to the
 * parameter passed to it, after first checking the verbosity level to provide
 * the suitable response.
 *
 * @param result Result of execution
 */
static void sio_send_cmd_result(command_result_t result)
{
    char *result_str;
    static char rsp_buffer[100];

    if (quiet)
    {
        return;
    }

    result_str = get_result_string((uint8_t)result);

    if (!result_str)
    {
        sprintf(rsp_buffer, "Unknown ERROR %d", (uint8_t)result);
        result_str = rsp_buffer;
    }

    if (verbose == 0)
    {
        sprintf(rsp_buffer, "%d", (uint8_t)result);
        sio_send_string(rsp_buffer);
        sio_send_cr();
    }
    else
    {
        sio_send_crlf();
        sio_send_string(result_str);
        sio_send_crlf();
    }
}



/**
 * @brief Sends a given string to the terminal over SIO
 *
 * @param s String to be sent
 */
static inline void sio_send_string(char *s)
{
    while (*s != 0)
    {
        sio_send_char(*(s++));
    }
}



/**
 * @brief Sends a given character to the terminal over SIO
 *
 * @param c Character to be sent
 */
static inline void sio_send_char(uint8_t c)
{
    pal_sio_tx(SIO_CHANNEL, &c, 1);
}



/**
 * @brief This function sends CR character to SIO
 */
static void sio_send_cr(void)
{
    sio_send_char(s3);
}



/**
 * @brief This function sends CR & LF to SIO
 */
static void sio_send_crlf(void)
{
    sio_send_char(s3);
    sio_send_char(s4);
}



/**
 * @brief Sends the given string to SIO if echo is enabled
 *
 * @param s String to be sent
 */
static void echo_string(char *s)
{
    if (echo)
    {
        sio_send_string(s);
    }
}



/**
 * @brief Sends the given character to SIO if echo is enabled
 *
 * @param c Character to be echoed
 */
static void echo_char(char c)
{
    if (echo)
    {
        sio_send_char(c);
    }
}



/**
 * @brief This function handles the reset command
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the given
 * command
 *
 * @return Result of execution of the command
 */
static command_result_t reset_cmd_handler(command_table_t *table_entry,
                                          char **cmd_line,
                                          cmd_response_cb_t response_cb)
{

    /*  Apply soft reset */
#if(PAL_TYPE == ATXMEGA256A3)
    CCP = 0xD8;
    RST.CTRL = 0x01;
#endif

    /* Keep compiler happy */
    table_entry = table_entry;
    response_cb = response_cb;
    cmd_line = cmd_line;

    return CMD_RESULT_OK;
}


/**
 * @brief This function handles the s-register access commands
 *
 * @param cmd_line Pointer to the command line string pointer
 * @param response_cb Pointer to the callback function to be called to send the
 * response string
 * @param table_entry Pointer to the command table entry holding the given
 * command
 *
 * @return Result of execution of the command
 */
static command_result_t sreg_cmd_handler(command_table_t *table_entry,
                                         char **cmd_line,
                                         cmd_response_cb_t response_cb)
{
    int i, s_no;

    s_no = get_number(cmd_line);

    for (i = 0 ; i < sizeof(sreg_table) / sizeof(command_table_t); i++)
    {
        char *temp = sreg_table[i].cmd;
        temp++;
        if (get_number(&temp) == s_no)
        {
            return execute_from_table(&sreg_table[i], cmd_line, response_cb);
        }
    }
    /* Keep compiler happy */
    table_entry = table_entry;

    return CMD_RESULT_ERROR;
}



/**
 * @brief Converts a result code into a corresponding string
 *
 * @param result_code the result code as an 8-bit ineger
 *
 * @return string corresponding to the code or null if code is invalid
 */
static char *get_result_string(uint8_t result_code)
{
    int i = 0;
    for (; i < sizeof(result_code_table) / sizeof(result_code_table_t); i++)
    {
        if (result_code_table[i].result_code == result_code)
        {
            return result_code_table[i].result_string;
        }
    }
    return 0;
}



/**
 * @brief This function transmits the message over the Serial.
 * The message will be formated into "SOT LENGTH message EOT" format.
 * @param message This is the pointer to the message. First Byte is length.
 */
void sio_write(uint8_t *message)
{
    message = message;
}

#endif /* SIO_HUB */

/* EOF */

