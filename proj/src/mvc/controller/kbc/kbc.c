/**
 * @file kbc.c
 * @brief File containing the implementation of the functions to interact with the KBC
*/

#include <lcom/lcf.h>

#include <lcom/lab4.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "kbc.h"
#include "../keyboard/keyboard.h"


/**
 * @brief reads the commands of the KBC
 * 
 * This function reads the commands of the KBC
 * 
 * @param data Address of memory to be initialized with the value read from the register
 * 
 * @return int 0 upon success, 1 otherwise
*/
int(read_commands_kbc)(uint8_t *data){
  int x = 5;
  uint8_t st;
  while(x != 0){
    if(util_sys_inb(STATUS_BYTE,&st) != 0){
      continue;
    }

    if(st & OUT_BUFF_FULL){
      if(util_sys_inb(OUTPUT_BUFFER,data) != 0){
        return 1;
      }

      if(((st & MOUSE_STATUS_ERRORS) == 0) && ((st & AUX_STATUS_REG) != 0)){
        return 0;
      }

    }
    x--;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

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
int(send_commands_kbc)(uint8_t command,int port){
  int x = 5;
  uint8_t st;
  while(x != 0){
    if(util_sys_inb(STATUS_BYTE,&st) != 0){
      return 1;
    }

    if((st & INPUT_BUFF_FULL) == 0){
      if(sys_outb(port,command) != 0){
        return 1;
      }
      return 0;
    }
    x--;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

/**
 * @brief writes a command arguments to the KBC
 * 
 * This function writes a command arguments to the KBC
 * 
 * @param command_byte command byte to be written
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(kbc_write_command_args)(uint8_t *command_byte){
  uint8_t st;
  int count = 0;
  while (1)
  {
      read_status_register(&st);
      if(count == 5){
        return 1;
      }

      if((st & KBC_ST_IBF) == 0){
        sys_outb(IN_KBC_COMMAND_ARGS , *command_byte);
        return 0;
      }else{
        count ++;
        tickdelay(micros_to_ticks(DELAY_US));
      }
  }
  return 0;
}
