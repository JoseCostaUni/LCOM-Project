#ifndef KBC_H
#define KBC_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

int(kbc_write_command)(uint8_t cmd);
int(kbc_write_command_args)(uint8_t *command_byte);



#endif
