#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IRQ_COMMAND_BYTE (IRQ_REENABLE | IRQ_EXCLUSIVE)

#define IRQ_MOUSE 12 // IRQ 12

#define DELAY_US 20000 //Define o tempo de delay

#define STATUS_BYTE 0x64

#define OUTPUT_BUFFER 0x60

#define INPUT_BUFFER 0x60

#define DISABLE_MOUSE 0xA7

#define ENABLE_MOUSE 0xA8

#define CHECK_MOUSE_INTERFACE 0xA9

#define WRITE_BYTE_TO_MOUSE 0xD4

#define AUX_STATUS_REG BIT(5)

#define READ_COMMAND_BYTE 0x20

#define WRITE_COMMAND_BYTE 0x60

#define MOUSE_STATUS_ERRORS (BIT(6) | BIT(7))

#define OUT_BUFF_FULL BIT(0) // Verifica se o out buffer está cheio (tem informação)

#define INPUT_BUFF_FULL BIT(1) // Verifica se o out buffer está cheio (tem informação)

#define LB BIT(0)

#define RB BIT(1)

#define MB BIT(2)

#define MSB_X_DELTA BIT(4)

#define MSB_Y_DELTA BIT(5)

#define X_OVFL BIT(6)

#define Y_OVFL BIT(7)

#define SYNC BIT(3)

#define ENABLE_DATA_REPORTING 0xF4

#define DISABLE_DATA_REPORTING 0xF5

#define WRITE_BYTE_TO_MOUSE 0xD4

#define ACK 0xF4

#define NACK 0xFE

#define KBC_IN_CMD 0x64

#define KBC_OUT_CMD 0x60

#define ERROR 0xFC

#define BUTTON_PRESSED 1

#endif

