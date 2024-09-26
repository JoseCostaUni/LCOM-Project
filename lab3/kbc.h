#ifndef KBC_H
#define KBC_H

#include <stdbool.h>
#include <stdint.h>

int(kbc_write_command)(uint8_t cmd);
int(kbc_write_command_args)(uint8_t *command_byte);



#endif
