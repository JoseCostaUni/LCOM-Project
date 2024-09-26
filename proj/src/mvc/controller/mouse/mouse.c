/**
 * @file mouse.c
 * @brief File containing the implementation of the functions to interact with the mouse
 */

#include "mouse.h"
#include "../../proj/src/mvc/model/game.h"
#include "../../proj/src/sprites/Cursor/cursors.xpm"
#include "../../view/view.h"
#include "../kbc/i8042.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

int hook_id_mouse = 2;

struct packet mouse;

struct cursor cursor;

bool sync_v = false;

int counter_mouse = 0;

extern int counter_packet_print;

extern struct Game *game;

enum InGameStates _current_state = INITIAL;

struct Position initial_pos;

struct Position final_pos;

struct Piece *piece_selected;

struct Position button_position;

/**
 * @brief Subscribes mouse interrupts
 *
 * This function Creates mouse interrupts
 *
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 *
 * @return int 0 upon success, 1 otherwise
 */
void(mouse_ih)() {
  uint8_t data;
  read_commands_kbc(&data);
  parse_values(data, &counter_mouse, &mouse);
}

/**
 * @brief Subscribes mouse interrupts
 *
 * This function subscribes mouse interrupts
 *
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 *
 * @return int 0 upon success, 1 otherwise
 */
int(mouse_subscribe_int)(uint8_t *bit_no) {

  *bit_no = hook_id_mouse;

  if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_COMMAND_BYTE, &hook_id_mouse) != 0) {
    return 1;
  }
  return 0;
}

/**
 * @brief Unsubscribes mouse interrupts
 *
 * This function unsubscribes mouse interrupts
 *
 * @return int 0 upon success, 1 otherwise
 */
int(mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_mouse) != 0) {
    return 1;
  }
  return 0;
}

/**
 * @brief resets the mouse struct
 *
 * @param mouse Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 *
 * @return int 0 upon success, 1 otherwise
 */
int(reset_mouse_struct)(struct packet *mouse) {

  return 0;
}

/**
 * @brief draws the cursor at the start of the game
 *
 * This function draws the cursor at the start of the game
 *
 * @return int 0 upon success, 1 otherwise
 */
int(cursor_draw_start)() {
  cursor.position.x = 0;
  cursor.position.y = 0;
  final_pos.x = 0;
  final_pos.y = 0;
  cursor.type = DEFAULT;
  _current_state = INITIAL;
  return 0;
}

/**
 * @brief parses the values of the mouse packet
 *
 * This function parses the values of the mouse packet
 *
 * @param data data to be parsed
 * @param cnt counter of the packet
 * @param pp packet to be parsed
 *
 * @return int 0 upon success, 1 otherwise
 */
int(parse_values)(uint8_t data, int *cnt, struct packet *pp) {
  if (*cnt == 0) {
    clean_packet(pp);
  }
  *cnt += 1;
  if (*cnt == 1) {
    if ((!sync_v) && ((data & SYNC) == 1)) {
      sync_v = true;
      *cnt = 0;
      return 0;
    }
  }

  switch (*cnt) {
    case 1: {
      pp->lb = (data & LB);
      pp->rb = ((data & RB) >> 1);
      pp->mb = ((data & MB) >> 2);
      pp->delta_x = ((data & MSB_X_DELTA) << 4);
      pp->delta_y = ((data & MSB_Y_DELTA) << 3);
      pp->x_ov = ((data & X_OVFL) >> 6);
      pp->y_ov = ((data & Y_OVFL) >> 7);

      break;
    }
    case 2: {
      pp->delta_x |= data;

      if ((pp->delta_x >> 8) == 1) {
        pp->delta_x |= 0xFF00;
      }
      else {
        pp->delta_x &= 0x00FF;
      }
      break;
    }

    case 3: {
      pp->delta_y |= data;

      if ((pp->delta_y >> 8) == 1) {
        pp->delta_y |= 0xFF00;
      }
      else {
        pp->delta_y &= 0x00FF;
      }

      break;
    }
  }
  counter_packet_print++;
  pp->bytes[*cnt - 1] = data;

  if (*cnt == 3) {
    cursor.position.x += pp->delta_x;
    cursor.position.y -= pp->delta_y;

    if (cursor.position.x >= WIDTH - 20 && cursor.position.x <= WIDTH) {
      cursor.position.x = 5;
    }

    if (cursor.position.y >= HEIGHT - 20 && cursor.position.y <= HEIGHT) {
      cursor.position.y = 5;
    }

    if (cursor.position.x >= WIDTH - 20) {
      cursor.position.x = WIDTH - 25;
    }

    if (cursor.position.y >= HEIGHT - 20) {
      cursor.position.y = HEIGHT - 25;
    }
    in_game_mouse_movement();
  }

  return 0;
}

/**
 * @brief cleans the mouse packet
 *
 * This function cleans the mouse packet
 *
 * @param mouse Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 *
 * @return int 0 upon success, 1 otherwise
 */
void clean_packet(struct packet *mouse) {
  memset(mouse->bytes, 0, sizeof(*mouse->bytes));
  mouse->delta_x = 0x0000;
  mouse->delta_y = 0x0000;
  mouse->lb = 0;
  mouse->mb = 0;
  mouse->rb = 0;
  mouse->x_ov = 0;
  mouse->y_ov = 0;
}

/**
 * @brief disables the mouse report
 *
 * This function disables the mouse report
 *
 * @return int 0 upon success, 1 otherwise
 */
int(disable_mouse_report)() {

  if (send_commands_kbc(WRITE_BYTE_TO_MOUSE, KBC_IN_CMD) != 0) {
    return 1;
  }

  if (send_commands_kbc(DISABLE_DATA_REPORTING, KBC_OUT_CMD) != 0) {
    return 1;
  }

  uint8_t error;
  read_commands_kbc(&error);
  switch (error) {
    case ACK:
      return 0;
      break;
    case NACK:
      printf("Invalid byte due to serial communication error");
      return 1;
      break;
    case ERROR:
      printf("Second consecutive invalid byte");
      return 1;
      break;
  }
  return 0;
}

/**
 * @brief enables the mouse report
 *
 * This function enables the mouse report
 *
 * @return int 0 upon success, 1 otherwise
 */
int(enable_mouse_report)() {

  if (send_commands_kbc(WRITE_BYTE_TO_MOUSE, KBC_IN_CMD) != 0) {
    return 1;
  }

  if (send_commands_kbc(ENABLE_DATA_REPORTING, KBC_OUT_CMD) != 0) {
    return 1;
  }

  uint8_t error;
  read_commands_kbc(&error);
  switch (error) {
    case ACK:
      return 0;
      break;
    case NACK:
      printf("Invalid byte due to serial communication error");
      return 1;
      break;
    case ERROR:
      printf("Second consecutive invalid byte");
      return 1;
      break;
  }
  return 0;
}

/**
 * @brief gets the position of the cursor
 *
 * This function gets the position of the cursor
 *
 * @param cursor Address of memory to be initialized with the value read from the register
 *
 * @return int 0 upon success, 1 otherwise
 */
struct mousePosition(get_position_cursor)(struct cursor *cursor) {
  return cursor->position;
}

/**
 * @brief In game mouse movement
 *
 * This function handles the mouse movement in the game
 *
 * @return int 0 upon success, 1 otherwise
 */
int(in_game_mouse_movement)() {
  switch (_current_state) {
    case INITIAL:

      if (mouse.lb == BUTTON_PRESSED && mouse.rb != BUTTON_PRESSED && mouse.mb != BUTTON_PRESSED) {
        piece_selected = get_piece_from_click(cursor.position.x, cursor.position.y, CELL_SIZE_HEIGHT, &game->board);
        if (piece_selected == NULL) {
          if (cursor.position.x >= 20 && cursor.position.x <= 140 && cursor.position.y >= 20 && cursor.position.y <= 60) {
            change_game_state_to_menu();
          }

          _current_state = INITIAL;
        }
        else {
          printf("piece_selected is not NULL\n");
          printf("white turn %d\n", game->isWhiteTurn);
          printf("piece selected is white %d\n", piece_selected->isWhite);
          initial_pos.x = piece_selected->position.x;
          initial_pos.y = piece_selected->position.y;
          if (!piece_selected->isWhite == game->isWhiteTurn) {
            _current_state = PIECE_SELECTED;
          }
          //}
        }
      }
      break;
    case PIECE_SELECTED:
      printf("PIECE_SELECTED\n");
      if (mouse.lb != BUTTON_PRESSED) {
        _current_state = PIECE_CLICKED;
        cursor.type = HOVERING;
      }
      else {
        _current_state = PIECE_DRAGGED;
        cursor.type = SELECTED;
      }
      break;
    case PIECE_CLICKED:
      printf("PIECE_CLICKED\n");
      if (mouse.lb == BUTTON_PRESSED) {
        _current_state = COMPLETE;
      }
      else if (mouse.rb == BUTTON_PRESSED) {
        _current_state = INITIAL;
      }
      break;
    case PIECE_DRAGGED:
      printf("PIECE_DRAGGED\n");
      if (mouse.lb != BUTTON_PRESSED) {
        _current_state = COMPLETE;
      }
      break;
    case COMPLETE:
      printf("COMPLETE\n");
      cursor.type = DEFAULT;
      _current_state = INITIAL;
      final_pos.x = (cursor.position.x - 200) / CELL_SIZE_WIDTH;
      final_pos.y = (cursor.position.y - 100) / CELL_SIZE_HEIGHT;

      if (change_piece_position(piece_selected, &initial_pos, &final_pos, &game->board)) {
        printf("Piece moved\n");

        changeTurn(game);
      }

      if (game->board.squares[final_pos.x][final_pos.y].type == PAWN) {
        if ((game->board.squares[final_pos.x][final_pos.y].isWhite && final_pos.y == 7) ||
            (!game->board.squares[final_pos.x][final_pos.y].isWhite && final_pos.y == 0)) {
          promote_pawn_to_queen(&game->board, &game->board.squares[final_pos.x][final_pos.y]);
        }
      }

      if (is_check(game)) {
        printf("Check\n");
      }

      piece_selected = NULL;
      break;
  }
  return 0;
}
