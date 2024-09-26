#pragma once

#include <lcom/lcf.h>

#include <lcom/lab4.h>

#include <stdbool.h>
#include <stdint.h>

int(read_commands_kbc)(uint8_t *data);
int(send_commands_kbc)(uint8_t command,int port);
int(kbc_enable_interrupts)();
