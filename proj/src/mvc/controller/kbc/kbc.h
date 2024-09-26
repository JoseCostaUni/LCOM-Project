/**
 * @file kbc.h
 * @brief File containing the constants and functions to interact with the KBC
*/
#ifndef KBC_H
#define KBC_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"


/**
 * @brief writes a command arguments to the KBC
 * 
 * This function writes a command arguments to the KBC
 * 
 * @param command_byte command to be written
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(kbc_write_command_args)(uint8_t *command_byte);


/**
 * @brief reads the commands of the KBC
 * 
 * This function reads the commands of the KBC
 * 
 * @param data Address of memory to be initialized with the value read from the register
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(read_commands_kbc)(uint8_t *data);


/**
 * @brief sends commands to the KBC
 * 
 * This function sends commands to the KBC
 * 
 * @param command command to be sent
 * @param port port to be used
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(send_commands_kbc)(uint8_t command,int port);


/**
 * @brief enables the KBC interrupts
 * 
 * This function enables the KBC interrupts
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(kbc_enable_interrupts)();



#endif
