/**
 * @file mouse.h
 * @brief File containing the constants and functions to interact with the mouse
*/
#ifndef _MOUSE_H_
#define _LCOM_I8254_H_

#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "../kbc/i8042.h"
#include "../controller.h"
#include "../kbc/kbc.h"



/**
 * @brief this function subscribes mouse interrupts
 * 
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
*/
int(mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief this function unsubscribes mouse interrupts
 *  
*/
int(mouse_unsubscribe_int)();

/**
 * @brief this function reads the status register
 * 
 * @param st Address of memory to be initialized with the value read from the register
*/
int (read_status_register)(uint8_t *st);

/**
 * @brief this function resets the mouse struct
 * 
 * @param mouse Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
*/
int(reset_mouse_struct)(struct packet *mouse);

/**
 * @brief parses the values of the mouse
 * 
 * @param data data to be parsed
 * @param cnt counter of the mouse
 * @param pp packet to be filled
 * 
 * @return 0 upon success, 1 otherwise
 */
int(parse_values)(uint8_t data,int *cnt,struct packet *pp);

/**
 * @brief this function disables the mouse report
 * 
 * @return 0 upon success, 1 otherwise
 */ 
int(disable_mouse_report)();

/**
 * @brief this function enables the mouse report
 * 
 * @return 0 upon success, 1 otherwise
 */
int(enable_mouse_report)();


/**
 * @brief this cleans the packet
 * 
 * @param mouse packet to be cleaned
 */
void clean_packet(struct packet *mouse);

/**
 * @brief this function handles mouse movement during the game.
 * 
 * @return 0 upon success, 1 otherwise
 */
int (in_game_mouse_movement)();

/**
 * @brief this function draws the cursor at the start of the game
 * 
 * @return 0 upon success, 1 otherwise
 */
int(cursor_draw_start)();


/**
 * @brief enum to represent the different states of the game
*/
enum InGameStates{
  INITIAL,
  PIECE_SELECTED,
  PIECE_CLICKED,
  PIECE_DRAGGED,
  COMPLETE,
};



#endif

