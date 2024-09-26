/**
 * @file graphic.c
 * @brief File containing the implementation of the functions to interact with the graphic card
 */

#include "graphic.h"
#include "../../../sprites/Cursor/cursors.xpm"
#include "../../../sprites/GameElements/blackClock_200.xpm"
#include "../../../sprites/GameElements/boad_450.xpm"
#include "../../../sprites/GameElements/boad_500.xpm"
#include "../../../sprites/GameElements/clock_100.xpm"
#include "../../../sprites/GameElements/clock_150.xpm"
#include "../../../sprites/GameElements/clock_200.xpm"
#include "../../../sprites/Menus/background.xpm"
#include "../../../sprites/Menus/black_wins.xpm"
#include "../../../sprites/Menus/game_type_choice.xpm"
#include "../../../sprites/Menus/instructions.xpm"
#include "../../../sprites/Menus/main_menu.xpm"
#include "../../../sprites/Menus/pause_menu.xpm"
#include "../../../sprites/Menus/white_wins.xpm"
#include "../../../sprites/pieces.xpm"
#include "../kbc/i8042.h"
#include "../kbc/kbc.h"
#include "../keyboard/keyboard.h"
#include <lcom/lcf.h>
#include <machine/int86.h>
#include <stdint.h>
#include <stdio.h>

uint8_t *frontBuffer;      // The front buffer
uint8_t *backBuffer;       // The back buffer
uint8_t *activeBuffer;     // swap active e  back dps desenhas no back e das outro swap
uint8_t *backgroundBuffer; // The active buffer

xpm_image_t whitePawnXPM;
xpm_image_t blackPawnXPM;
xpm_image_t whiteBishopXPM;
xpm_image_t blackBishopXPM;
xpm_image_t whiteKnightXPM;
xpm_image_t blackKnightXPM;
xpm_image_t whiteQueenXPM;
xpm_image_t blackQueenXPM;
xpm_image_t whiteKingXPM;
xpm_image_t blackKingXPM;
xpm_image_t whiteRookXPM;
xpm_image_t blackRookXPM;
xpm_image_t defaultCursorXPM;
xpm_image_t grabbingCursorXPM;
xpm_image_t grabCursorXPM;
xpm_image_t menuXPM;
xpm_image_t backgroundXPM;
xpm_image_t instructionsXPM;
xpm_image_t gameTypeChoiceXPM;
xpm_image_t defaultClock;
xpm_image_t blackWinsXPM;
xpm_image_t whiteWinsXPM;
xpm_image_t pauseMenuXPM;
xpm_image_t board;

xpm_image_t number0XPM;
xpm_image_t number1XPM;
xpm_image_t number2XPM;
xpm_image_t number3XPM;
xpm_image_t number4XPM;
xpm_image_t number5XPM;
xpm_image_t number6XPM;
xpm_image_t number7XPM;
xpm_image_t number8XPM;
xpm_image_t number9XPM;
xpm_image_t twoPointsXPM;

uint32_t bufferSize;

unsigned bytesPerPixel = -1;
uint16_t xRes, yRes;

vbe_mode_info_t mode_info;

/*
Parâmetro reg86_t 	Text Mode 	Graphic Mode
ah 	                  0x00 	        0x4F
al 	                  Ox03 	        0x02
ax 	                  0x0003 	      0x4F02
bx 	                  0x0000 	submode | BIT(14)
intno 	              0x10 	        0x10

*/

/**
 * @brief Sets the graphic mode to the given mode.
 *
 * This function sets the graphic mode to the given mode.
 *
 * @param mode The mode to set.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(set_graphic_mode)(uint16_t *mode) {
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86));

  reg86.intno = 0x10; /* BIOS video services always ten*/
  reg86.ah = 0x4F;    /* Set Video Mode */
  reg86.al = 0x02;    /* Mode 0x105 */
                      // reg86.ax = 0x4F02; /* 0x4F02 */

  /**
   * Sets the mode of the reg86.bx register with the given mode value.
   * The mode value is combined with the BIT(14) flag using the bitwise OR operator.
   *
   * @param mode The mode value to set.
   */
  reg86.bx = *mode | BIT(14);

  if (sys_int86(&reg86) != OK) {
    printf("set_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;
}

/**
 * @brief Sets the text mode.
 *
 * This function sets the text mode.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(set_text_mode)() {
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86));

  reg86.intno = 0x10; /* set_texBIOS video services always ten*/
  reg86.ah = 0x00;    /* Set Video Mode */
  reg86.al = 0x03;    /* Mode 0x105 */
  reg86.ax = 0x0003;  /* 0x4F02 */

  if (sys_int86(&reg86) != OK) {
    printf("set_mode: sys_int86() failed \n");
    return 1;
  }

  printf("Text mode setted\n");
  return 0;
}

/**
 * @brief Sets the frame mode.
 *
 * This function sets the frame mode.
 *
 * @param mode The mode to set.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(set_frame_mode)(uint16_t *mode) {

  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(*mode, &mode_info)) {
    return 1;
  }

  unsigned int bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

  int r;

  // preenchimento dos endereços físicos
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr;
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size;

  // alocação física da memória necessária para o frame buffer
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  // alocação virtual da memória necessária para o frame buffer
  frontBuffer = vm_map_phys(SELF, (void *) physic_addresses.mr_base, frame_size);

  printf("front buffer size : %d\n", frame_size);

  if (frontBuffer == MAP_FAILED)
    panic("couldn't map video memory");

  return 0;
}

/**
 * @brief Gets the horizontal resolution.
 *
 * This function gets the horizontal resolution.
 *
 * @return int The horizontal resolution
 */
int(get_h_res)() {
  return mode_info.XResolution;
}

/**
 * @brief Gets the vertical resolution.
 *
 * This function gets the vertical resolution.
 *
 * @return int The vertical resolution
 */
int(get_v_res)() {
  return mode_info.YResolution;
}

/**
 * @brief Loads the numbers.
 * This function loads the numbers.
 *
 * @return int 0 upon success, 1 otherwise
 */
int load_numbers() {
  for (int i = 0; i < 10; i++) {
    switch (i) {
      case 0:
        xpm_load(number0_20, XPM_8_8_8, &number0XPM);
        break;
      case 1:

        xpm_load(number1_20, XPM_8_8_8, &number1XPM);
        break;
      case 2:
        xpm_load(number2_20, XPM_8_8_8, &number2XPM);
        break;
      case 3:
        xpm_load(number3_20, XPM_8_8_8, &number3XPM);
        break;
      case 4:
        xpm_load(number4_20, XPM_8_8_8, &number4XPM);
        break;
      case 5:
        xpm_load(number5_20, XPM_8_8_8, &number5XPM);
        break;
      case 6:
        xpm_load(number6_20, XPM_8_8_8, &number6XPM);
        break;
      case 7:
        xpm_load(number7_20, XPM_8_8_8, &number7XPM);
        break;
      case 8:
        xpm_load(number8_20, XPM_8_8_8, &number8XPM);
        break;
      case 9:
        xpm_load(number9_20, XPM_8_8_8, &number9XPM);
        break;
      default:
        return 1;
    }
  }
  xpm_load(two_points20, XPM_8_8_8, &twoPointsXPM);
  return 0;
}

/**
 * @brief Loads the xpm.
 *
 * This function loads the xpm.
 *
 * @param img The image to load.
 * @param pieceType The piece type.
 * @param isWhite If the piece is white.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(load_xpm)(xpm_map_t img, enum PieceType pieceType, bool isWhite) {

  switch (pieceType) {
    case PAWN:
      if (isWhite)
        xpm_load(img, XPM_8_8_8, &whitePawnXPM);
      else
        xpm_load(img, XPM_8_8_8, &blackPawnXPM);
      break;
    case BISHOP:
      if (isWhite)
        xpm_load(img, XPM_8_8_8, &whiteBishopXPM);
      else
        xpm_load(img, XPM_8_8_8, &blackBishopXPM);
      break;
    case KNIGHT:
      if (isWhite)
        xpm_load(img, XPM_8_8_8, &whiteKnightXPM);
      else
        xpm_load(img, XPM_8_8_8, &blackKnightXPM);
      break;
    case QUEEN:
      if (isWhite)
        xpm_load(img, XPM_8_8_8, &whiteQueenXPM);
      else
        xpm_load(img, XPM_8_8_8, &blackQueenXPM);
      break;
    case KING:
      if (isWhite)
        xpm_load(img, XPM_8_8_8, &whiteKingXPM);
      else
        xpm_load(img, XPM_8_8_8, &blackKingXPM);
      break;
    case ROOK:
      if (isWhite)
        xpm_load(img, XPM_8_8_8, &whiteRookXPM);
      else
        xpm_load(img, XPM_8_8_8, &blackRookXPM);
      break;
    default:
      return 1;
      break;
  }
  return 0;
}

/**
 * @brief Loads the xpm clocks board.
 *
 * This function loads the xpm clocks board.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(load_xpm_clocks_board)() {
  xpm_load(clockXPM150, XPM_8_8_8, &defaultClock);
  xpm_load(boad_450, XPM_8_8_8, &board);
  return 0;
}

/**
 * @brief Loads the xpm cursors.
 * 
 * This function loads the xpm cursors.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(load_xpm_cursor)() {
  xpm_load(cursor_default, XPM_8_8_8, &defaultCursorXPM);
  xpm_load(cursor_grabbing, XPM_8_8_8, &grabbingCursorXPM);
  xpm_load(cursor_grab, XPM_8_8_8, &grabCursorXPM);
  return 0;
}

/**
 * @brief Draws the pixel.
 *
 * This function draws the pixel.
 *
 * @param x The x position.
 * @param y The y position.
 * @param color The color.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if (x > mode_info.XResolution || y > mode_info.YResolution)
    return 0;

  bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  unsigned int index = (mode_info.XResolution * y + x) * bytesPerPixel;

  if (memcpy(&backBuffer[index], &color, bytesPerPixel) == NULL)
    return 0;

  return 0;
}

/**
 * @brief Draws the horizontal line.
 *
 * This function draws the horizontal line.
 *
 * @param x The x position.
 * @param y The y position.
 * @param len The length.
 * @param color The color.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (int i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0) {
      return 1;
    }
  }

  return 0;
}

/**
 * @brief Draws the vertical line.
 *
 * This function draws the vertical line.
 *
 * @param x The x position.
 * @param y The y position.
 * @param len The length.
 * @param color The color.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (unsigned i = 0; i < len; i++) {
    if (vg_draw_pixel(x, y + i, color) != 0) {
      return 1;
    }
  }

  return 0;
}

/**
 * @brief Draws the rectangle.
 *
 * This function draws the rectangle.
 *
 * @param x The x position.
 * @param y The y position.
 * @param width The width.
 * @param height The height.
 * @param color The color.
 *
 * @return int 0 upon success, 1 otherwise
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  for (unsigned i = 0; i < height; i++)
    if (vg_draw_hline(x, y + i, width, color) != 0) {
      return 1;
    }

  return 0;
}

/**
 * @brief Adjusts the color.
 * 
 * This function adjusts the color.
 * 
 * @param color The color.
 * @param new_color The new color.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(adjust_color)(uint32_t color, uint16_t *new_color) {

  if (mode_info.BitsPerPixel == 32) {
    *new_color = color;
    return 0;
  }
  else {
    uint32_t mask = BIT(mode_info.BitsPerPixel) - 1;
    *new_color = color & mask;
  }

  return 0;
}

/**
 * @brief erases the buffer.
 * 
 * This function erases the buffer.
 */
void erase_buffer() {
  memset(backBuffer, 0, bufferSize);
}

/**
 * @brief erases the front buffer.
 * 
 * This function erases the front buffer.
 */
void erase_frontBuffer() {
  memset(frontBuffer, 0, bufferSize);
}

/**
 * @brief erases the background buffer.
 * 
 * This function erases the background buffer.
 */
void erase_backgroundBuffer() {
  memset(backgroundBuffer, 0, bufferSize);
}

/**
 * @brief Swaps the buffers.
 * 
 * This function swaps the buffers.
 */
void swap_buffers() {
  bufferSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
  memcpy(frontBuffer, backBuffer, bufferSize);
}

/**
 * @brief Swaps the background buffer.
 * 
 * This function swaps the background buffer.
 */
void swap_BackgroundBuffer() {
  bufferSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
  memcpy(backBuffer, backgroundBuffer, bufferSize);
}

/**
 * @brief Copies the background buffer.
 * 
 * This function copies the background buffer.
 */
void copy_BackGroundBuffer() {
  bufferSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
  memcpy(backgroundBuffer, backBuffer, bufferSize);
}

/**
 * @brief Allocates the buffers.
 * 
 * This function allocates the buffers.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int allocate_buffers() {

  bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  bufferSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

  backBuffer = (uint8_t *) malloc(bufferSize);
  backgroundBuffer = (uint8_t *) malloc(bufferSize);

  if (backBuffer == NULL || backgroundBuffer == NULL) {
    printf("Error: Failed to allocate memory for the buffers\n");
    return 1;
  }

  return 0;
}

/**
 * @brief Draws the xpm.
 * 
 * This function draws the xpm.
 * 
 * @param img The image to draw.
 * @param x The x position.
 * @param y The y position.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_xpm)(xpm_map_t img, uint16_t x, uint16_t y) {

  xpm_image_t image;
  uint8_t *map;
  map = xpm_load(img, XPM_8_8_8, &image);
  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type))
        if (vg_draw_pixel(x + j, y + i, color)) {
          return 1;
        }
    }
  }
  swap_buffers();
  return 0;
}

/**
 * @brief Draws the Black Piece.
 * 
 * This function draws the Black Piece.
 * 
 * @param x The x position.
 * @param y The y position.
 * @param pieceType The piece type.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_black_piece)(uint16_t x, uint16_t y, enum PieceType pieceType) {

  xpm_image_t image;

  switch (pieceType) {
    case PAWN:
      image = blackPawnXPM;
      break;
    case BISHOP:
      image = blackBishopXPM;
      break;
    case KNIGHT:
      image = blackKnightXPM;
      break;
    case QUEEN:
      image = blackQueenXPM;
      break;
    case KING:
      image = blackKingXPM;
      break;
    case ROOK:
      image = blackRookXPM;
      break;
    default:
      return 1;
      break;
  }

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(x + j, y + i, color)) {
          return 1;
        }
      }
    }
  }

  return 0;
}

/**
 * @brief Draws the cursor mouse.
 * 
 * This function draws the cursor mouse.
 * 
 * @param x The x position.
 * @param y The y position.
 * @param cursorType The cursor type.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_cursor_mouse)(uint16_t x, uint16_t y, enum CursorType cursorType) {
  xpm_image_t image;

  switch (cursorType) {
    case DEFAULT:
      image = defaultCursorXPM;
      break;
    case HOVERING:
      image = grabCursorXPM;
      break;
    case SELECTED:
      image = grabbingCursorXPM;
      break;
    default:
      break;
  }

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(x + j, y + i, color)) {
          return 1;
        }
      }
    }
  }

  swap_buffers();
  return 0;
}

/**
 * @brief Draws the White Piece.
 * 
 * This function draws the White Piece.
 * 
 * @param x The x position.
 * @param y The y position.
 * @param pieceType The piece type.
 */
int(draw_white_piece)(uint16_t x, uint16_t y, enum PieceType pieceType) {

  xpm_image_t image;

  switch (pieceType) {
    case PAWN:
      image = whitePawnXPM;
      break;
    case BISHOP:
      image = whiteBishopXPM;
      break;
    case KNIGHT:
      image = whiteKnightXPM;
      break;
    case QUEEN:
      image = whiteQueenXPM;
      break;
    case KING:
      image = whiteKingXPM;
      break;
    case ROOK:
      image = whiteRookXPM;
      break;
    default:
      return 1;
      break;
  }

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(x + j, y + i, color)) {
          return 1;
        }
      }
    }
  }

  return 0;
}


/**
 * @brief Draws the board without pieces.
 * 
 * This function draws the board without pieces.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_board_without_Pieces)() {

  xpm_image_t image = board;

  int x = 175;
  int y = 75;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(x + j, y + i, color)) {
          return 1;
        }
      }
    }
  }

  return 0;
}

/**
 * @brief Draws the both clocks.
 * 
 * This function draws the both clocks.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int draw_both_clocks() {
  xpm_image_t image = defaultClock;
  int x = 650;
  int y = 514;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(j + x, i + y, color))
          return 1;
      }
    }
  }

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(j + x, i, color))
          return 1;
      }
    }
  }

  return 0;
}

/**
 * @brief Loads the xpm menu.
 * 
 * This function loads the xpm menu.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int load_xpm_menu() {
  xpm_load(main_menu, XPM_8_8_8, &menuXPM);
  xpm_load(instructions, XPM_8_8_8, &instructionsXPM);
  xpm_load(game_type_choice, XPM_8_8_8, &gameTypeChoiceXPM);
  xpm_load(black_wins, XPM_8_8_8, &blackWinsXPM);
  xpm_load(white_wins, XPM_8_8_8, &whiteWinsXPM);
  xpm_load(pause_menu, XPM_8_8_8, &pauseMenuXPM);
  return 0;
}


/**
 * @brief Loads the xpm game background.
 * 
 * This function loads the xpm game background.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int load_xpm_game_background() {
  xpm_load(background, XPM_8_8_8, &backgroundXPM);
  return 0;
}


/**
 * @brief Draws the menu.
 * 
 * This function draws the menu.
 * 
 * @param x The x position.
 * @param y The y position.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_menu)(uint16_t x, uint16_t y) {

  xpm_image_t image = menuXPM;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(x + j, y + i, color)) {
          return 1;
        }
      }
    }
  }

  swap_buffers();

  return 0;
}


/**
 * @brief Draws the game background.
 * 
 * This function draws the game background.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_game_background)() {
  xpm_image_t image = backgroundXPM;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(j, i, color)) {
          return 1;
        }
      }
    }
  }

  swap_buffers();

  return 0;
}


/**
 * @brief Draws the game instructions.
 * 
 * This function draws the game instructions.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_game_instructions)() {
  xpm_image_t image = instructionsXPM;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(j, i, color)) {
          return 1;
        }
      }
    }
  }

  swap_buffers();

  return 0;
}


/**
 * @brief Draws the game mode menu.
 * 
 * This function draws the game mode menu.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_game_mode_menu)() {
  xpm_image_t image = gameTypeChoiceXPM;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(j, i, color)) {
          return 1;
        }
      }
    }
  }

  swap_buffers();

  return 0;
}



/**
 * @brief Draws the black wins.
 * 
 * This function draws the black wins.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_black_wins)() {
  xpm_image_t image = blackWinsXPM;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(j, i, color)) {
          return 1;
        }
      }
    }
  }

  swap_buffers();

  return 0;
}


/**
 * @brief Draws the white wins.
 * 
 * This function draws the white wins.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_white_wins)() {
  xpm_image_t image = whiteWinsXPM;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(j, i, color)) {
          return 1;
        }
      }
    }
  }

  swap_buffers();

  return 0;
}


/**
 * @brief Draws the pause menu.
 * 
 * This function draws the pause menu.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_pause_menu)() {
  xpm_image_t image = pauseMenuXPM;

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(j, i, color)) {
          return 1;
        }
      }
    }
  }

  swap_buffers();

  return 0;
}

/**
 * @brief Draws the number.
 * 
 * This function draws the number.
 * 
 * @param x The x position.
 * @param y The y position.
 * @param number The number.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_number)(uint16_t x, uint16_t y, int number) {
  xpm_image_t image;
  switch (number) {
    case 0:
      image = number0XPM;
      break;
    case 1:
      image = number1XPM;
      break;
    case 2:
      image = number2XPM;
      break;
    case 3:
      image = number3XPM;
      break;
    case 4:
      image = number4XPM;
      break;
    case 5:
      image = number5XPM;
      break;
    case 6:
      image = number6XPM;
      break;
    case 7:
      image = number7XPM;
      break;
    case 8:
      image = number8XPM;
      break;
    case 9:
      image = number9XPM;
      break;
    default:
      return 1;
      break;
  }

  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(x + j, y + i, color)) {
          return 1;
        }
      }
    }
  }
  return 0;
}

/**
 * @brief Draws the two points.
 * 
 * This function draws the two points.
 * 
 * @param x The x position.
 * @param y The y position.
 * 
 * @return int 0 upon success, 1 otherwise
 */
int(draw_two_points)(uint16_t x, uint16_t y) {
  xpm_image_t image = twoPointsXPM;
  for (unsigned int i = 0; i < image.height; i++) {
    for (unsigned int j = 0; j < image.width; j++) {

      uint16_t bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
      uint32_t byte_index = (image.width * i + j) * bytes_p_pixel;
      uint32_t color = 0;
      memcpy(&color, image.bytes + byte_index, bytes_p_pixel);
      if (color != xpm_transparency_color(image.type)) {
        if (vg_draw_pixel(x + j, y + i, color)) {
          return 1;
        }
      }
    }
  }
  return 0;
}
