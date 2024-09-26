#include "mvc/controller/graphics/VBE.h"
#include "mvc/controller/kbc/i8042.h"
#include "mvc/controller/kbc/kbc.h"
#include "mvc/controller/keyboard/keyboard.h"
#include "mvc/controller/mouse/mouse.h"
#include "mvc/controller/timer/timer.h"
#include "mvc/controller/rtc/rtc.h"
#include "mvc/model/game.h"
#include "sprites/Cursor/cursors.xpm"
#include "sprites/pieces.xpm"
#include "mvc/controller/controller.h"
#include <lcom/lcf.h>
#include <machine/int86.h>
#include <stdio.h>

/**
 * @file main.c
 * @brief File containing the main loop of the program
 */
//extern int counter;
int elapsed_seconds = true;

const int desiredFrameRate = 30;
const double targetFrameTime = 1.0 / desiredFrameRate;

bool isRunning = true;

uint8_t timer_hook_id = 0;
uint8_t keyboard_hook_id = 1;
uint8_t mouse_hook_id = 2;
uint8_t rtc_hook_id = 8;

int counter_packet_print = 0;

extern int counter_mouse;

extern struct cursor cursor;

extern enum ClickedKey key_pressed;
extern enum FlowState current_state;

extern struct Board *board;
extern struct Game *game;
extern struct Menu *menu;
/*
int rtc_test_date(void) {
    date_time dt;
    while (true){
        int ret = rtc_get_datetime(&dt);
        if (ret == -1) return -1;
        else if (ret == 1) continue;
        show_date(&dt);
        printf(" ");
        show_time(&dt);
        printf("\n");
        return 0;
    }
}
*/
extern uint32_t counter;

int main(int argc, char *argv[]) {

  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/g1/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/g1/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/**
 * @brief Initializes the program
 * 
 * @return int 0 upon success, 1 otherwise
*/
int setup() {

  uint16_t mode = VBE_800x600_DC;

  if (initialize_graphics(&mode) != 0)
    return 1;

  load_xpm(Chess_plt45, PAWN, WHITE);
  load_xpm(Chess_black_plt45, PAWN, BLACK);
  load_xpm(Chess_blt45, BISHOP, WHITE);
  load_xpm(Chess_black_blt45, BISHOP, BLACK);
  load_xpm(Chess_nlt45, KNIGHT, WHITE);
  load_xpm(Chess_black_nlt45, KNIGHT, BLACK);
  load_xpm(Chess_qlt45, QUEEN, WHITE);
  load_xpm(Chess_black_qlt45, QUEEN, BLACK);
  load_xpm(Chess_klt45, KING, WHITE);
  load_xpm(Chess_black_klt45, KING, BLACK);
  load_xpm(Chess_rlt45, ROOK, WHITE);
  load_xpm(Chess_black_rlt45, ROOK, BLACK);
  
  load_xpm(Chess_plt45, PAWN, WHITE);
  load_xpm(Chess_black_plt45, PAWN, BLACK);
  load_xpm(Chess_blt45, BISHOP, WHITE);
  load_xpm(Chess_black_blt45, BISHOP, BLACK);
  load_xpm(Chess_nlt45, KNIGHT, WHITE);
  load_xpm(Chess_black_nlt45, KNIGHT, BLACK);
  load_xpm(Chess_qlt45, QUEEN, WHITE);
  load_xpm(Chess_black_qlt45, QUEEN, BLACK);
  load_xpm(Chess_klt45, KING, WHITE);
  load_xpm(Chess_black_klt45, KING, BLACK);
  load_xpm(Chess_rlt45, ROOK, WHITE);
  load_xpm(Chess_black_rlt45, ROOK, BLACK);

  load_xpm_cursor();
  load_xpm_menu();
  load_xpm_game_background();
  load_xpm_clocks_board();
  load_numbers();
  cursor_draw_start();

  return 0;
}

/**
 * @brief Exits the program
 * 
 * @return int 0 upon success, 1 otherwise
*/
int _exit_() {

  if (set_text_mode() != 0)
    return 1;
  if (timer_unsubscribe_int() != 0)
    return 1;
  if (keyboard_unsubscribe_int() != 0)
    return 1;
  if (mouse_unsubscribe_int() != 0)
    return 1;
  if(rtc_unsubscribe_int() != 0){
    return 1;
  }
  if (vg_exit() != 0)
    return 1;

  return 0;
}

/**
 * @brief Main loop of the program
 * 
 * @param argc number of arguments
 * @param argv arguments
 * 
 * @return int 0 upon success, 1 otherwise
*/
int(proj_main_loop)(int argc, char *argv[]) {

  int ipc_status, r;
  message msg;
  uint8_t irq_timer, irq_keyboard, irq_mouse , irq_rtc;

  if (enable_mouse_report() != 0) {
    return 1;
  }

  if (timer_subscribe_int(&irq_timer) != 0)
    return 1;
  if (keyboard_subscribe_int(&irq_keyboard) != 0)
    return 1;
  if (mouse_subscribe_int(&irq_mouse) != 0)
    return 1;
  if(rtc_subscribe_int(&irq_rtc) != 0){
    return 1;
  }
  setup();

/*  if (draw_backBackGround() != 0) {
    return 1;
  }

  copy_BackGroundBuffer();

  if (draw_board(board) != 0) {
    return 1;
  }

  struct Position initalPos = board->pieces[0].position;
  initalPos.y = 1;
  initalPos.x = 0;
  board->pieces[0].position.x = 4;
  board->pieces[0].position.y = 7;

  if (return_to_initial_pos(&board->pieces[0], &initalPos, board) != 0) {
    return 1;
  }

*/
  if(draw_menu(0,0) != 0){
    return 1;
  }

  swap_buffers();
  int x = 10;
  while (isRunning) {
    x--;
    
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer) {
            if(current_state == GAME){
              timer_int_handler();
              if (counter % 6 == 0) {
                decrease_player_timer();
                game_loop(game);
              }
            }
          }

          if (msg.m_notify.interrupts & irq_mouse || msg.m_notify.interrupts & BIT(irq_keyboard)) {
      
            uint32_t status;
            sys_inb(STATUS_BYTE, &status);
            if (status & OUT_BUFF_FULL) {
              if (status & AUX_STATUS_REG) {
                parse_mouse_input();
              }
              else {
                parse_keyboard_input();

                if (key_pressed == ESC && current_state == MENU) {
                  printf("esc pressed");
                  isRunning = false;
                }
              }
            }
          }

          break;
        default:
          break;
      }
    }
    
  }

  _exit_();

  return 0;
}

