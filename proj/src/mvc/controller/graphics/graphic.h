/**
 * @file graphic.h
 * @brief File containing the constants and functions to interact with the graphic card
*/

#include <machine/int86.h>
#include "lcom/lcf.h"
#include "VBE.h"
#include "../../model/game.h"
#include "../mouse/mouse.h"
#include "sprites/GameElements/clock_150.xpm"
#include "sprites/Characters/Characters20x32/0_20.xpm"
#include "sprites/Characters/Characters20x32/1_20.xpm"
#include "sprites/Characters/Characters20x32/2_20.xpm"
#include "sprites/Characters/Characters20x32/3_20.xpm"
#include "sprites/Characters/Characters20x32/4_20.xpm"
#include "sprites/Characters/Characters20x32/5_20.xpm"
#include "sprites/Characters/Characters20x32/6_20.xpm"
#include "sprites/Characters/Characters20x32/7_20.xpm"
#include "sprites/Characters/Characters20x32/8_20.xpm"
#include "sprites/Characters/Characters20x32/9_20.xpm"
#include "sprites/Characters/Characters20x32/two_point.xpm"

#pragma once

#define MAX_PIXELS 75

//int (vg_exit)(); dado por LCF

/**
 * @brief sets the graphic mode
 * 
 * 
 * @param mode mode to be set
 * 
 * @return 0 upon success, 1 otherwise
 */
int (set_graphic_mode)(uint16_t* mode);


/**
 * @brief sets the frame mode
 * 
 * 
 * @param mode mode to be set
 * 
 * @return 0 upon success, 1 otherwise
 */
int (set_frame_mode)(uint16_t* mode);


/**
 * @brief sets the text mode
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int (set_text_mode)();


/**
 * @brief loads the xpm
 * 
 * 
 * @param img xpm to be loaded
 * @param pieceType type of the piece
 * @param isWhite if the piece is white
 * 
 * @return 0 upon success, 1 otherwise
 */
int (load_xpm)(xpm_map_t img ,enum PieceType pieceType , bool isWhite);


/**
 * @brief loads the xpm cursor
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int (load_xpm_cursor)();


/**
 * @brief loads the xpm menu
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int load_xpm_menu();


/**
 * @brief loads the xpm game background
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int (load_numbers)();


/**
 * @brief loads the xpm game background
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int (load_xpm_clocks_board)();


/**
 * @brief loads the xpm game background
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int allocate_buffers();


/**
 * @brief swaps the buffers
 * 
 * 
 */
void swap_buffers();


/**
 * @brief swaps the background buffer
 * 
 * 
 */
void swap_BackgroundBuffer();


/**
 * @brief copies the buffer
 * 
 * 
 */
void copy_BackGroundBuffer();


/**
 * @brief erases the buffer
 * 
 * 
 */
void erase_buffer();

/**
 * @brief gets the horizontal resolution
 * 
 * 
 * @return horizontal resolution
 */
int (get_h_res)();


/**
 * @brief gets the vertical resolution
 * 
 * 
 * @return vertical resolution
 */
int (get_v_res)();
  
/**
 * @brief draws a pixel
 * 
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param color color of the pixel
 * 
 * @return 0 upon success, 1 otherwise
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);



/**
 * @brief adjusts the color
 * 
 * 
 * @param color color to be adjusted
 * @param new_color new color
 * 
 * @return 0 upon success, 1 otherwise
 */
int (adjust_color)(uint32_t color, uint16_t* new_color);


/**
 * @brief draws the xpm
 * 
 * 
 * @param img xpm to be drawn
 * @param x x coordinate
 * @param y y coordinate
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_xpm) (xpm_map_t img, uint16_t x, uint16_t y);



/**
 * @brief draws the black piece
 * 
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param pieceType type of the piece
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_black_piece)(uint16_t x, uint16_t y , enum PieceType pieceType);


/**
 * @brief draws the white piece
 * 
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param pieceType type of the piece
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_white_piece)(uint16_t x, uint16_t y , enum PieceType pieceType);


/**
 * @brief draws the both clocks
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int draw_both_clocks();




/**
 * @brief draws the board with the pieces
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_board_without_Pieces)();


/**
 * @brief draws the cursor
 * 
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param cursorType type of the cursor
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_cursor_mouse)(uint16_t x, uint16_t y , enum CursorType cursorType);


/**
 * @brief draws the menu
 * 
 * 
 * @param x x coordinate
 * @param y y coordinate
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_menu)(uint16_t x, uint16_t y);

/**
 * @brief draws the number
 * 
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param number number to be drawn
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_number)(uint16_t x , uint16_t y , int number);


/**
 * @brief draws the two points
 * 
 * 
 * @param x x coordinate
 * @param y y coordinate
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_two_points)(uint16_t x , uint16_t);


/**
 * @brief draws the game background
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int(draw_game_background)();


/**
 * @brief loads the xpm game background
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int load_xpm_game_background();

/**
 * @brief draws the game instructions
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int(draw_game_instructions)();


/**
 * @brief draws the game mode menu
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int(draw_game_mode_menu)();


/**
 * @brief erases the front buffer
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
void erase_frontBuffer();


/**
 * @brief erases the background buffer
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
void erase_backgroundBuffer();


/**
 * @brief draws the black wins
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int draw_black_wins();


/**
 * @brief draws the white wins
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int draw_white_wins();


/**
 * @brief draws the pause menu
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int draw_pause_menu();


/**
 * @brief waits for the ESC key
 * 
 * 
 * @return 0 upon success, 1 otherwise
 */
int (wait_for_ESC_)();


