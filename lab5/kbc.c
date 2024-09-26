#include "lcom/lcf.h"
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

int(kbc_write_command)(uint8_t cmd){ //Usado para passar o valor de 0x20 -> Avisar que vai ler o command byte do kbc através do output buffer e 0x60 -> Avisar que vai enviar um novo command byte do kbc através do 0x64 como argumento
  uint8_t st;
  int count = 0;
  while (1)
  {
      read_status_register(&st);
      if(count == 5){
        return 1;
      }

      if((st & KBC_ST_IBF) == 0){
        sys_outb(KBC_CMD_REG , cmd);
        return 0;
      }else{
        count ++;
        tickdelay(micros_to_ticks(DELAY_US));
      }
  }
  return 0;
}

int(kbc_write_command_args)(uint8_t *command_byte){ //Usado para passar command args para o endereço 0x64
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
