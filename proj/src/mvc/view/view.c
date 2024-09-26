/**
 * @file view.c
 * @brief This file contains the implementation of the functions that will be used to draw the game on the screen.
*/

#include "../controller/graphics/graphic.h"
#include "sprites/pieces.xpm"
#include "sprites/Cursor/cursors.xpm"
#include "view.h"

/**
 * @brief Initializes the graphics mode.
 *  
 * This function initializes the graphics mode with the specified mode.
 * 
 * @param mode Pointer to the mode to be set.
 * @return Return 0 upon success, non-zero otherwise.
*/
int initialize_graphics(uint16_t* mode){

  if(set_frame_mode(mode) != 0 )
    return 1;

  if(set_graphic_mode(mode) != 0 )
    return 1;

  if(allocate_buffers() != 0)
    return 1;
  
  return 0;
}

/**
 * @brief Fills a rectangular area with a specified color.
 * 
 * This function fills a rectangular area defined by the coordinates (x, y) and the dimensions (width, height) with the specified color.
 * 
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color to fill the rectangle with.
 * @return Return 0 upon success, non-zero otherwise.
*/
int (fill)(int x , int y , int width , int height , uint32_t color){

  for(int i = 0 ; i < height; i++){
    if(vg_draw_hline(x, y+i, width, color) != 0){
      return 1;
    }
  }

    return 0;
}

/**
 * @brief Draws the chess board.
 * 
 * This function draws the chess board on the screen.
 * 
 * @param board Pointer to the chess board structure.
 * @return Return 0 upon success, non-zero otherwise.
*/
int (draw_board)(struct Board* board){

  for (int i = 0; i < 32; i++) {
    draw_piece(&board->pieces[i]);
  }

  return 0;
}


/**
 * @brief Draws the chess board except for one piece.
 * 
 * This function draws the chess board on the screen except for the piece with the specified id.
 * 
 * @param id The id of the piece to not draw.
 * @param board Pointer to the chess board structure.
 * @return Return 0 upon success, non-zero otherwise.
*/
int draw_board_except_one_piece(int id , struct Board* board){

  for (int i = 0; i < 32; i++) {
    if(board->pieces[i].id == id){
      continue;
    }
    draw_piece(&board->pieces[i]);
  }
  return 0;
}

/**
 * @brief Draws the clocks.
 * 
 * This function draws the clocks on the screen.
 * 
 * @return Return 0 upon success, non-zero otherwise.
 */
 int draw_Clocks(){ 

   if (draw_xpm(clockXPM150,650, 0)!=0)
   {
     return 1;
   }
   if (draw_xpm(clockXPM150,650, 514)!=0)
   {
     return 1;
   }

   return 0;
 
 }

/**
 * @brief Draws the clocks values.
 * 
 * This function draws the clocks values on the screen.
 * 
 * @param player1 Pointer to the first player structure.
 * @param player2 Pointer to the second player structure.
 * @return Return 0 upon success, 1 otherwise.
 */
int draw_clockValue(struct Player *player1,struct Player *player2){

  draw_number(670,40,player1->clock.minutes/10);
  draw_number(695,40,player1->clock.minutes%10);

  draw_two_points(720 , 40);
  
  draw_number(745,40,player1->clock.seconds/10);
  draw_number(770,40,player1->clock.seconds%10);

  draw_number(670 , 520, player2->clock.minutes/10);
  draw_number(695 , 520, player2->clock.minutes%10);
  
  draw_two_points(720 , 520);
  
  draw_number(745,520,player2->clock.seconds/10);
  draw_number(770,520,player2->clock.seconds%10);

  return 0;
}

/**
 * @brief Draws the background of the game.
 * 
 * This function draws the background of the game, including the game background itself and any additional elements such as clocks or menus.
 * 
 * @return 0 upon success, 1 otherwise.
 */
int draw_backBackGround(){
  

  if(draw_game_background() != 0)
    return 1;
  
  if(draw_board_without_Pieces() != 0)
    return 1;


  

  //if(draw_Clocks())
    //return 1;
/*
  if(draw_clockValue(player1,player2) != 0){
    return 1;
  }
*/
  return 0;
}

/**
 * @brief Draws the background of the game without erasing existing elements.
 * 
 * This function draws the background of the game without erasing existing elements,
 * such as the game board, allowing for incremental updates to the display.
 * 
 * @return 0 upon success, 1 otherwise.
 */
int (draw_BackGround_Without_Erase)(){

  if(draw_board_without_Pieces() != 0)
    return 1;

  return 0;
}

/**
 * @brief Draws a pawn piece on the game board.
 * 
 * This function draws a pawn piece on the game board at the specified position.
 * 
 * @param piece Pointer to the pawn piece to be drawn.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_pawn)(struct Piece* piece){

  int initialX = piece->position.x * (400/8) + 175 + 25;
  int initialY = 40 + 60 + 50 * piece->position.y;

  if(piece->isWhite){
    draw_white_piece(initialX, initialY , piece->type);
  }else{

    draw_black_piece(initialX, initialY , piece->type);
  }

  return 0;
}

/**
 * @brief Draws a rook piece on the game board.
 * 
 * This function draws a rook piece on the game board at the specified position.
 * 
 * @param piece Pointer to the rook piece to be drawn.
 * @return 0 upon success, 1 otherwise
 */
int (draw_rook)(struct Piece* piece){
  int initialX = piece->position.x * (400/8) + 25 + 175;
  int initialY = 42 + 60 + 50 * piece->position.y;

  if(piece->isWhite){
    draw_white_piece(initialX, initialY , piece->type);
  }else{
    draw_black_piece(initialX, initialY , piece->type);
  }

  return 0;
}

/**
 * @brief Draws a knight piece on the game board.
 * 
 * This function draws a knight piece on the game board at the specified position.
 * 
 * @param piece Pointer to the knight piece to be drawn.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_knight)(struct Piece* piece){
  int initialX = piece->position.x * (400/8) + 25 + 175;
    int initialY = 42 + 60 + 50 * piece->position.y;

  if(piece->isWhite){
    draw_white_piece(initialX, initialY , piece->type);
  }else{
    draw_black_piece(initialX, initialY , piece->type);
  }

  return 0;
}

/**
 * @brief Draws a bishop piece on the game board.
 * 
 * This function draws a bishop piece on the game board at the specified position.
 * 
 * @param piece Pointer to the bishop piece to be drawn.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_bishop)(struct Piece* piece){
  int initialX = piece->position.x * (400/8) + 25 + 175;
   int initialY = 42 + 60 + 50 * piece->position.y;

  if(piece->isWhite){
    draw_white_piece(initialX, initialY , piece->type);
  }else{
    draw_black_piece(initialX, initialY , piece->type);
  }

  return 0;
}

/**
 * @brief Draws a queen piece on the game board.
 * 
 * This function draws a queen piece on the game board at the specified position.
 * 
 * @param piece Pointer to the queen piece to be drawn.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_queen)(struct Piece* piece){
  
  int initialX = piece->position.x * (400/8) + 25 + 175;
  int initialY = 42 + 60 + 50 * piece->position.y;

  if(piece->isWhite){
    draw_white_piece(initialX, initialY , piece->type);
  }else{
    draw_black_piece(initialX, initialY , piece->type);
  }

  return 0;
}

/**
 * @brief Draws a king piece on the game board.
 * 
 * This function draws a king piece on the game board at the specified position.
 * 
 * @param piece Pointer to the king piece to be drawn.
 * @return 0 upon success, 1 otherwise.
 */
int (draw_king)(struct Piece* piece){
  int initialX = piece->position.x * (400/8) + 25 + 175;
   int initialY = 42 + 60 + 50 * piece->position.y;

  if(piece->isWhite){
    draw_white_piece(initialX, initialY , piece->type);
  }else{
    draw_black_piece(initialX, initialY , piece->type);
  }

  return 0;
}

/**
 * @brief Draws a piece on the game board based on its type.
 * 
 * This function draws a piece on the game board based on its type (pawn, rook, knight, bishop, queen, or king).
 * 
 * @param piece Pointer to the piece to be drawn.
 * @return 0 upon success, 1 otherwise.
 */
int draw_piece(struct Piece* piece){
  switch (piece->type)
  {
  case PAWN:
    draw_pawn(piece);
    break;
  case ROOK:
    draw_rook(piece);
    break;
  case KNIGHT:
    draw_knight(piece);
    break;
  case BISHOP:
    draw_bishop(piece);
    break;
  case QUEEN:
    draw_queen(piece);
    break;
  case KING:
    draw_king(piece);
    break;
  default:
    return 0;
  }

  return 0;
}

/**
 * @brief Draws the cursor on the game board.
 * 
 * This function draws the cursor (mouse pointer) on the game board at the specified position.
 * 
 * @param cursor Pointer to the cursor structure containing position and type.
 * @param board Pointer to the game board.
 * @return 0 upon success, 1 otherwise.
 */
int draw_cursor(struct cursor* cursor,struct Board *board){
  swap_BackgroundBuffer();
  draw_board(board);
  draw_cursor_mouse(cursor->position.x, cursor->position.y , cursor->type);
  swap_buffers();
  return 0;
}

/**
 * @brief Moves a piece to its initial position on the game board.
 * 
 * This function animates the movement of a piece back to its initial position on the game board.
 * 
 * @param piece Pointer to the piece to be moved.
 * @param initialPos Pointer to the initial position of the piece.
 * @param board Pointer to the game board.
 * @return 0 upon success, 1 otherwise.
 */
int return_to_initial_pos(struct Piece* piece, struct Position* initialPos , struct Board* board) {


    int initialX = initialPos->x * (400/8) + 200;
    int initialY = initialPos->y * CELL_SIZE_HEIGHT + 102;

    int currentX = piece->position.x * (400/8) + 200;
    int currentY = piece->position.y * CELL_SIZE_HEIGHT + 102;

    if (currentX == initialX && currentY == initialY) {
        return 0; 
    }

    double m = 0;
    if (currentX != initialX) {
        m = (double)(currentY - initialY) / (currentX - initialX);
    }
    double b = currentY - m * currentX;

    while (currentX != initialX || currentY != initialY) {

        if (currentX != initialX) {
            if (currentX > initialX) {
                currentX--;
            } else {
                currentX++;
            }
        }

        if (currentX != initialX) {
            currentY = (int)(m * currentX + b);
        } else if (currentY != initialY) {
            if (currentY > initialY) {
                currentY--;
            } else {
                currentY++;
            }
        }

        swap_BackgroundBuffer();
        draw_board_except_one_piece(piece->id , board);
        
        if (piece->isWhite) {
            draw_white_piece(currentX, currentY , piece->type);
        } else {
            draw_black_piece(currentX, currentY , piece->type);
        }
        swap_buffers();
    }

    piece->position.x = initialPos->x;
    piece->position.y = initialPos->y;

    return 0;
}


/**
 * @brief Advances a piece to a new position on the game board.
 * 
 * This function animates the movement of a piece to a new position on the game board.
 * 
 * @param piece Pointer to the piece to be moved.
 * @param initialPos Pointer to the initial position of the piece.
 * @param board Pointer to the game board.
 * @return 0 upon success, 1 otherwise.
 */
int (advance_piece)(struct Piece* piece , struct Position* initialPos , struct Board* board){
    int initialX = initialPos->x * (400/8) + 200;
    int initialY = initialPos->y * CELL_SIZE_HEIGHT + 102;

    int currentX = piece->position.x * (400/8) + 200;
    int currentY = piece->position.y * CELL_SIZE_HEIGHT + 102;

    if (currentX == initialX && currentY == initialY) {
        return 0; 
    }

    double m = 0;
    
    if (currentX != initialX) {
        m = (double)(currentY - initialY) / (currentX - initialX);
    }
    double b = currentY - m * currentX;

    while (currentX != initialX || currentY != initialY) {

        if (currentX != initialX) {
            if (currentX > initialX) {
                currentX--;
            } else {
                currentX++;
            }
        }

        if (currentX != initialX) {
            currentY = (int)(m * currentX + b);
        } else if (currentY != initialY) {
            if (currentY > initialY) {
                currentY--;
            } else {
                currentY++;
            }
        }

        swap_BackgroundBuffer();
        draw_board_except_one_piece(piece->id , board);

        if (piece->isWhite) {
            draw_white_piece(currentX, currentY , piece->type);
        } else {
            draw_black_piece(currentX, currentY , piece->type);
        }
        swap_buffers();
    }

    return 0;
}
