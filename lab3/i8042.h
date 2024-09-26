#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IRQ_KEYBOARD 1

#define IRQ_REENABLE_EXCLUSIVE (IRQ_REENABLE | IRQ_EXCLUSIVE)

#define IN_KBC_COMMANDS 0x64

#define IN_KBC_COMMAND_ARGS 0x60

#define OUT_SCANCODES 0x60

#define OUT_KBC_COMMAND_RET 0x60

#define STATUS_BYTE 0x64

#define ESC_BREAK_CODE 0x81

#define PARITY_ERROR BIT(7) // Parity error ativa em 1 no bit 7

#define TIMEOUT_ERROR BIT(6) // Timeout error ativa em 1 no bit 6

#define OUT_BUFF_FULL BIT(0) // Verifica se o out buffer está cheio (tem informação)

#define KEYBOARD_STATUS_ERRORS (BIT(6) | BIT(7)) // Macro com os erros de cada keyboard

#define DELAY_US 20000 //Define o tempo de delay

#define KBC_ST_REG 0x64

#define KBC_CMD_REG 0x64

#define AUX_STATUS_REG BIT(5)

#define ENABLE_INTERRUPT_OBF_KEYBOARD BIT(0)

#define READ_COMMAND_BYTE 0x20

#define MAKE_CODE 0xE0

#define WRITE_COMMAND_BYTE 0x60

#define KBC_ST_IBF BIT(1)



#endif
