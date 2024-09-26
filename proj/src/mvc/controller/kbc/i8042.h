#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IRQ_COMMAND_BYTE (IRQ_REENABLE | IRQ_EXCLUSIVE)

#define IRQ_MOUSE 12 // IRQ 12

#define STATUS_BYTE 0x64

#define OUTPUT_BUFFER 0x60

#define INPUT_BUFFER 0x60

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

#define IRQ_KEYBOARD 1

#define OK 0

#define IRQ_REENABLE_EXCLUSIVE (IRQ_REENABLE | IRQ_EXCLUSIVE)

#define IN_KBC_COMMANDS 0x64

#define IN_KBC_COMMAND_ARGS 0x60

#define OUT_SCANCODES 0x60

#define OUT_KBC_COMMAND_RET 0x60

#define ESC_BREAK_CODE 0x81

#define W 0x11

#define S 0x1F

#define A 0x1E

#define D 0x20

#define _ONE 0x2

#define _TWO 0x3

#define _THREE 0x4

#define _FOUR 0x5

#define _FIVE 0x6

#define _SIX 0x7

#define UP_ARROW 0x48

#define DOWN_ARROW 0x50

#define LEFT_ARROW 0x4B

#define RIGHT_ARROW 0x4D

#define _ENTER_ 0x1C

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
