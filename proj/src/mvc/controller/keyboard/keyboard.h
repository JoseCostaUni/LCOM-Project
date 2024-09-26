/**
 * @file keyboard.h
 * @brief File containing the constants and functions to interact with the keyboard
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../kbc/i8042.h"

/**
 * @brief Struct to represent the scancode information.
*/
struct scancode_info {
  uint8_t size_counter;
  bool make_code;
  uint8_t bytes[2];
};


/**
 * @brief Subscribes keyboard interrupts
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * 
 * @return int 0 upon success, -1 otherwise
*/
int keyboard_subscribe_int(uint8_t *bit_no);


/**
 * @brief Unsubscribes keyboard interrupts
 * 
 * @return int 0 upon success, -1 otherwise
*/
int keyboard_unsubscribe_int();


/**
 * @brief Reads a register from the keyboard
 * 
 * @param reg Register to be read
 * @param st Address of memory to be initialized with the value read from the register
 * 
 * @return int 0 upon success, -1 otherwise
*/
int read_status_register(uint8_t *st);


/**
 * @brief Reads the output buffer
 * 
 * @param out Address of memory to be initialized with the value read from the output buffer
 * 
 * @return int 0 upon success, -1 otherwise
 */
int read_out_buffer(uint8_t *out);

/**
 * @brief Receives the scancode from the keyboard
 * 
 * @param scan_info Address of memory to be initialized with the scancode information
 * @param scancode Address of memory to be initialized with the scancode
 * 
 * @return true if the scancode was received, false otherwise
 */
bool(receive_keyboard_scan)(struct scancode_info *scan_info, uint8_t *scancode);


/**
 * @brief Cleans the scancode information
 * 
 * @param scan Address of memory to be initialized with the scancode information
 */
void(clean_scan_info)(struct scancode_info *scan);


/**
 * @brief Tests if the status polling is valid
 * 
 * @param st Status to be tested
 */
bool(test_status_polling)(uint8_t st);


/**
 * @brief Enables the keyboard interrupts
 * 
 * @return int 0 upon success, -1 otherwise
 */
int(kbc_enable_interrupts)();


/**
 * @brief Try to read the output buffer
 * 
 * @param out Address of memory to be initialized with the value read from the output buffer
 */
int try_read_out_buffer(uint8_t *out);


/**
 * @brief Checks if the ESC key was pressed
 * 
 * @return int 0 upon success, -1 otherwise
 */
int (check_ESC)();


/**
 * @brief Breaks from the cycles
 * 
 * @return int 0 upon success, -1 otherwise
 */
int (break_from_cycles)();

#endif /* KEYBOARD_H */
