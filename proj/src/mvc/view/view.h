/**
 * @file view.h
 * @brief This file contains the declaration of the functions that will be used to draw the game on the screen.
*/

#include "../controller/graphics/graphic.h"
#include <machine/int86.h>
#include "lcom/lcf.h"
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
#include "sprites/Menus/go_to_menu.xpm"
#include "../controller/controller.h"

/**
 * @brief Initializes the graphics mode.
 * 
 * @param mode The graphics mode to initialize.
 * @return Return 0 upon success, non-zero otherwise.
 */
int initialize_graphics(uint16_t* mode);

/**
 * @brief Fills a rectangular area with a specified color.
 * 
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color to fill the rectangle with.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (fill)(int x , int y , int width , int height , uint32_t color);

/**
 * @brief Draws the chess board.
 * 
 * @param board Pointer to the chess board structure.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_board)(struct Board* board);

/**
 * @brief Draws the chess board except for one piece.
 * 
 * @param id The id of the piece to not draw.
 * @param board Pointer to the chess board structure.
 * @return Return 0 upon success, non-zero otherwise.
 */
int draw_board_except_one_piece(int id , struct Board* board);

/**
 * @brief Draws BackGround.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int draw_backBackGround();

/**
 * @brief Draws BackGround without erasing the previous one.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_BackGround_Without_Erase)();

/**
 * @brief Draws the clocks.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_Clocks)();

/**
 * @brief Draws Piece.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int draw_piece();

/**
 * @brief Draws Initial Position.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int draw_initial_pos();

/**
 * @brief Draws Cursor.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int draw_cursor(struct cursor* cursor,struct Board *board);

/**
 * @brief Draws the clock value.
 * 
 * @param player1 Pointer to the first player structure.
 * @param player2 Pointer to the second player structure.
 * @return Return 0 upon success, non-zero otherwise.
 */
int draw_clockValue(struct Player *player1,struct Player *player2);


/**
 * @brief Returns the piece to its initial position.
 * 
 * @param piece Pointer to the piece structure.
 * @param initialPos Pointer to the initial position structure.
 * @param board Pointer to the board structure.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (return_to_initial_pos)(struct Piece* piece , struct Position* initialPos , struct Board * board);

/**
 * @brief Advances the piece to the next position.
 * 
 * @param piece Pointer to the piece structure.
 * @param initialPos Pointer to the initial position structure.
 * @param board Pointer to the board structure.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (advance_piece)(struct Piece* piece , struct Position* initialPos , struct Board* board);


/**
 * @brief Draws Pawn.
 * 
 * @param piece Pointer to the piece structure.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_pawn)(struct Piece* piece);

/**
 * @brief Draws Rook.
 * 
 * @param piece Pointer to the piece structure.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_rook)();

/**
 * @brief Draws Knight.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_knight)();

/**
 * @brief Draws Bishop.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_bishop)();

/**
 * @brief Draws Queen.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_queen)();

/**
 * @brief Draws King.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_king)();

/**
 * @brief Draws Castle.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
int (draw_menu_buttonss)();
