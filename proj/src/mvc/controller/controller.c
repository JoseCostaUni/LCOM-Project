/**
 * @file controller.c
 * @brief This file contains the implementation of the controller
 * 
 * This file contains the implementation of the controller
 */

#include "controller.h"
#include "keyboard/keyboard.h"
#include "../controller/rtc/rtc.h"

extern uint8_t scancode;
extern struct scancode_info scan_info;

enum ClickedKey key_pressed = NOKEY;
enum FlowState current_state = MENU;
extern int counter_mouse;

extern struct cursor cursor;
extern struct Position *button_position;

bool can_draw_this = true;
bool game_alredy_started = false;

struct Board tempBoard;

current_date dt = {0,0,0,0,0,0,0};
bool isWhiteTurn = true;


/**
 * @brief Initializes a new game with the specified time limit for each player.
 * 
 * This function initializes a new game with the given time limit for each player. It sets up the game board, initializes the players' clocks, and sets the initial game state.
 * 
 * @param game Pointer to the Game structure.
 * @param minutes The number of minutes for each player.
 * @param seconds The number of seconds for each player.
 */
void init_game(struct Game *game,int minutes, int seconds) {
  // game->White_player = {};
  init_board(&game->board);
  init_player(&game->Black_player, false, minutes, seconds);
  init_player(&game->White_player, true, minutes, seconds);
  game->state = START;
  game->piece_count = 32;
  game->isWhiteTurn = true;

  index_ = 0;
}

/**
 * @brief Main game loop that updates the game state and checks for game-ending conditions.
 * 
 * This function represents the main game loop. It updates the game state, checks for game-ending conditions such as checkmate or timeout, and handles the transition to the winner screen if necessary.
 * 
 * @param game Pointer to the Game structure representing the current game state.
 */
void game_loop(struct Game *game) {

  bool boardChanged = false;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tempBoard.squares[i][j].type != game->board.squares[i][j].type) {
                boardChanged = true;
                break;
            }
        }
        if (boardChanged) {
            break;
        }
    }

    if (boardChanged) {
        tempBoard = game->board;

        if (index_ < 1024) {
            boardArray[index_] = game->board;
            index_++;
            max_index = index_;
        } 
    }
  int king_count = 0;

  for(int i = 0 ; i < 32 ; i++){
    if(game->board.pieces[i].type == KING){
      if(game->board.pieces[i].isWhite)
        king_count--;
      else
        king_count++;
    }
  }

  if(king_count == 1){
    current_state = WINNER_SCREEN;
    dt.day = 0;
    dt.month = 0;
    dt.year = 0;
    dt.hours = 0;
    dt.minutes = 0;
    dt.seconds = 0;

    game_alredy_started = false;

    free(game);
    erase_buffer();
    draw_white_wins();
  }else if(king_count == -1){
    current_state = WINNER_SCREEN;
    dt.day = 0;
    dt.month = 0;
    dt.year = 0;
    dt.hours = 0;
    dt.minutes = 0;
    dt.seconds = 0;

    game_alredy_started = false;

    free(game);
    erase_buffer();
    draw_black_wins();
  }

  if(game->White_player.clock.minutes == 0 && game->White_player.clock.seconds == 0 && game->White_player.clock.a_tenth_of_a_second == 0){
    current_state = WINNER_SCREEN;
    dt.day = 0;
    dt.month = 0;
    dt.year = 0;
    dt.hours = 0;
    dt.minutes = 0;
    dt.seconds = 0;

    game_alredy_started = false;

    free(game);
    erase_buffer();
    draw_black_wins();
  }

  if(game->Black_player.clock.minutes == 0 && game->Black_player.clock.seconds == 0 && game->Black_player.clock.a_tenth_of_a_second == 0){
    current_state = WINNER_SCREEN;
    dt.day = 0;
    dt.month = 0;
    dt.year = 0;
    dt.hours = 0;
    dt.minutes = 0;
    dt.seconds = 0;

    game_alredy_started = false;

    free(game);
    erase_buffer();
    draw_white_wins();
  }
}

/**
 * @brief Parses keyboard input and updates the current key pressed.
 * 
 * This function reads keyboard input, interprets the scan code received, and updates the current key pressed accordingly. It then calls the router function to handle the input.
 */
void parse_keyboard_input() {

  kbc_ih();

  if (!receive_keyboard_scan(&scan_info, &scancode)) {
    key_pressed = ESC;
    return;
  }

  if (scan_info.size_counter == 1) {
    switch (scan_info.bytes[0]) {
      case W:
        key_pressed = ARROW_UP;
        break;
      case S:
        key_pressed = ARROW_DOWN;
        break;
      case A:
        key_pressed = ARROW_LEFT;
        break;
      case D:
        key_pressed = ARROW_RIGHT;
        break;
      case _ONE:
        key_pressed = ONE;
        break;
      case _TWO:
        key_pressed = TWO;
        break;
      case _THREE:
        key_pressed = THREE;
        break;
      case _FOUR:
        key_pressed = FOUR;
        break;
      case _FIVE:
        key_pressed = FIVE;
        break;  
      case _SIX:
        key_pressed = SIX;
        break;  
      case ESC_BREAK_CODE:
        key_pressed = ESC;
        break;
      default:
        break;
    }
  }
  else {
    switch (scan_info.bytes[1]) {
      case UP_ARROW:
        key_pressed = ARROW_UP;
        break;
      case DOWN_ARROW:
        key_pressed = ARROW_DOWN;
        break;
      case LEFT_ARROW:
        key_pressed = ARROW_LEFT;
        break;
      case RIGHT_ARROW:
        key_pressed = ARROW_RIGHT;
        break;
      case _ENTER_:
        key_pressed = ENTER;
        break;
      default:
        break;
    }
  }
  
  router();
}

/**
 * @brief Parses mouse input and updates the display accordingly.
 * 
 * This function handles mouse input, updates the display, and redraws the board and clock values if the current state is the game. It also draws the mouse cursor.
 */
void parse_mouse_input() {
  mouse_ih();

  if (counter_mouse == 3) {
    counter_mouse = 0;
    if(current_state == GAME){
      erase_buffer();

      swap_BackgroundBuffer();

      draw_board(&game->board);

      draw_clockValue(&game->Black_player, &game->White_player);



      draw_cursor_mouse(cursor.position.x, cursor.position.y , cursor.type);

      
      swap_buffers();
    }
  }
}

int minutes = 0;
int seconds = 0;

/**
 * @brief Routes the program flow based on the current state and user input.
 * 
 * This function manages the program flow, including transitions between different states (such as MENU, NEW_GAME, INSTRUCTIONS, etc.) and handling user input.
 */
void router() {
  struct Board tempBoard ;
  switch (current_state) {
    case MENU:
      switch (key_pressed) {
        case ONE:
          current_state = NEW_GAME;
          draw_game_mode_menu();
          key_pressed = NOKEY;
          break;
        case TWO:
          
          if(game_alredy_started == false){
            key_pressed = ONE;
          }else{
            current_state = LOAD_GAME;
            key_pressed = NOKEY;
          }
          router();
          break;
        case THREE:
          current_state = INSTRUCTIONS;
          draw_game_instructions();
          key_pressed = NOKEY;
          break;
        case FOUR:
          current_state = EXIT;
          key_pressed = ESC;
          break;
        default:
          break;
      }
      break;
    case NEW_GAME:
  
    if(game_alredy_started == true && game != NULL){
      game_alredy_started = false;
      free(game);
    }

    switch (key_pressed) {
      case ONE:
        game_alredy_started = true;

        minutes = 1;
        seconds = 0;
        game = create_game();

        init_game(game, minutes,seconds);
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break;
      case TWO:
        game_alredy_started = true;
        minutes = 5;
        seconds = 0;
        game = create_game();

          init_game(game, minutes,seconds);
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break;  
      case THREE:
        game_alredy_started = true;
        minutes = 10;
        seconds = 0;
        game = create_game();

          init_game(game, minutes,seconds);
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break;  
      case FOUR:
        game_alredy_started = true;
        minutes = 30;
        seconds = 0;
        game = create_game();
        init_game(game, minutes,seconds);
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break; 
      case FIVE:
        game_alredy_started = true;
        minutes = 59;
        seconds = 58;
        game = create_game();

        init_game(game, minutes,seconds);
        key_pressed = NOKEY;
        current_state = GAME;
        router();
        break;
      case SIX:
        key_pressed = NOKEY;
        current_state = MENU;
        draw_menu(0,0);
        break;
      case NOKEY:

        break;
      default:

        break;
      }
    break;
    case LOAD_GAME:

      if(game_alredy_started == false){
        current_state = MENU;
        key_pressed = NOKEY;
      }else{
        key_pressed = NOKEY;
        current_state = GAME;
        if (dt.day != 0 && dt.month != 0 && dt.year != 0 && dt.hours != 0 && dt.minutes != 0 && dt.seconds != 0) {
          
          current_date dt2;
          rtc_get_current_date(&dt2);

          print_time(&dt);
          print_time(&dt2);

          int start_total_seconds = dt.hours * 3600 + dt.minutes * 60 + dt.seconds;
          int end_total_seconds = dt2.hours * 3600 + dt2.minutes * 60 + dt2.seconds;

          int diff_seconds = end_total_seconds - start_total_seconds;
          if (diff_seconds < 0) {
              diff_seconds += 24 * 3600;
          }

          int diff_minutes = diff_seconds / 60;
          int remaining_seconds = diff_seconds % 60;

          if (isWhiteTurn) {
              game->White_player.clock.minutes -= diff_minutes;
              game->White_player.clock.seconds -= remaining_seconds;

              if (game->White_player.clock.seconds < 0) {
                  game->White_player.clock.minutes -= 1;
                  game->White_player.clock.seconds += 60;
              }

              if (game->White_player.clock.minutes < 0) {
                  game->White_player.clock.minutes = 0;
                  game->White_player.clock.seconds = 0;
              }
          } else {
              game->Black_player.clock.minutes -= diff_minutes;
              game->Black_player.clock.seconds -= remaining_seconds;

              if (game->Black_player.clock.seconds < 0) {
                  game->Black_player.clock.minutes -= 1;
                  game->Black_player.clock.seconds += 60;
              }

              if (game->Black_player.clock.minutes < 0) {
                  game->Black_player.clock.minutes = 0;
                  game->Black_player.clock.seconds = 0;
              }
            }

          if(game->White_player.clock.minutes <= 0 && game->White_player.clock.seconds <= 0){
            current_state = WINNER_SCREEN;
            game_alredy_started = false;
            dt.day = 1;
            dt.month = 1;
            dt.year = 1;
            dt.hours = 1;
            dt.minutes = 1;
            dt.seconds = 1;
            free(game);
            erase_buffer();
            draw_black_wins();
          }

          if(game->Black_player.clock.minutes <= 0 && game->Black_player.clock.seconds <= 0){
            current_state = WINNER_SCREEN;
            game_alredy_started = false;
            dt.day = 0;
            dt.month = 0;
            dt.year = 0;
            dt.hours = 0;
            dt.minutes = 0;
            dt.seconds = 0;
            free(game);
            erase_buffer();
            draw_white_wins();
          }
        }
      }

      break;
    case INSTRUCTIONS:
      switch (key_pressed){
      case ESC:
        current_state = MENU;
        key_pressed = NOKEY;

        draw_menu(0,0);

        break;
      case ONE:
        current_state = MENU;
        key_pressed = NOKEY;

        draw_menu(0,0);

        break;  
      default:
        break;
      }
      break;
    case GAME:

      switch (key_pressed)
      {
      case NOKEY:
        
          tempBoard = game->board;
          erase_buffer();

          draw_backBackGround(&game->White_player, &game->Black_player);

          copy_BackGroundBuffer();

          draw_board(&game->board);

          swap_buffers();

          can_draw_this = true;
          break;
      case ARROW_LEFT:
        
        index_--;
        if(index_ <= 0){
          index_ = 0;
        }


        tempBoard = boardArray[index_];

        erase_buffer();

        swap_BackgroundBuffer();

        draw_board(&tempBoard);

        swap_buffers();

        can_draw_this = false;
        break;
      case ARROW_RIGHT:
        
        index_++;
        
        if((index_-1) >= max_index){
          index_ = max_index-2;
        }
         tempBoard = boardArray[index_];
         erase_buffer();

        swap_BackgroundBuffer();

        draw_board(&tempBoard);

        swap_buffers();
        can_draw_this = false;
        break;
      case ARROW_DOWN:
        
        index_ = 0;
         tempBoard = boardArray[index_];

         erase_buffer();

        swap_BackgroundBuffer();

        draw_board(&tempBoard);

        swap_buffers();
        can_draw_this = false;

        break;
      case ARROW_UP:
        key_pressed = NOKEY;
        index_ = max_index;
        can_draw_this = true;
        router();
        break;
      
      default:
        break;
      }
      

      break;
    case PAUSE_MENU:
      switch (key_pressed){
      case ONE:
        current_state = GAME;
        key_pressed = NOKEY;
        break;
      case TWO:
        
        while (true){
            int ret = rtc_get_current_date(&dt);
            if (ret == -1) 
              return ;
            else if (ret == 1) 
              continue;
            break;
        }

        FILE *file = fopen("current_current_date.txt", "w");

        if (file == NULL) {
            printf("Error opening file\n");
            break; 
        } else {
            printf("File opened successfully\n");
        }

        fprintf(file, "%02d-%02d-%04d %02d:%02d:%02d\n",
                dt.day, dt.month, dt.year, dt.hours, dt.minutes, dt.seconds);
        
        fclose(file);

        isWhiteTurn = game->isWhiteTurn;

        current_state = MENU;
        key_pressed = NOKEY;

        draw_menu(0,0);

        break;  
      default:
        break;
      }
      break;
    break;
    case EXIT:

      current_state = MENU;

      key_pressed = ESC;

      erase_buffer();

      draw_menu(0,0);

      swap_buffers();

      router();
      break;
    case WINNER_SCREEN:
      switch (key_pressed){
      case ONE:
        current_state = MENU;

        erase_buffer();

        draw_menu(0,0);

        swap_buffers();

        key_pressed = NOKEY;

        break;
      default:
        break;
      }
      break;  
  }
}

/**
 * @brief Decreases the timer of the current player and updates the display if necessary.
 * 
 * This function decreases the timer of the current player (either White or Black) by one tenth of a second. If the timer reaches zero, it decrements the minutes and seconds accordingly. It also updates the display if `can_draw_this` is true.
 */
// min == 0 , segundos passa para a esquerda e os tenth_of_a_second passam pra direita
void decrease_player_timer() {

  if (game->isWhiteTurn) {
    game->White_player.clock.a_tenth_of_a_second--;
    /*
    MIN , SEG , SEG/10
    30    00     00;
    29    59     10;
    00    59     9;
    */
   
    if (game->White_player.clock.a_tenth_of_a_second <= 0) {
      if (game->White_player.clock.seconds == 0 && game->White_player.clock.minutes == 0) {
        game->White_player.clock.a_tenth_of_a_second = 0;
      }
      else {
        game->White_player.clock.a_tenth_of_a_second = 9;
        if (game->White_player.clock.seconds == 0) {
          if (game->White_player.clock.minutes > 0) {
            game->White_player.clock.minutes--;
            game->White_player.clock.seconds = 59;
          }
          else {
            game->White_player.clock.seconds = 0;
          }
        }
        else {
          game->White_player.clock.seconds--;
        }
      }
    }
    
  }
  else {
    game->Black_player.clock.a_tenth_of_a_second--;
   
    /*
    MIN , SEG , SEG/10
    30    00     00;
    29    59     10;
    00    59     9;
    */
   
    if (game->Black_player.clock.a_tenth_of_a_second <= 0) {
      if (game->Black_player.clock.seconds == 0 && game->Black_player.clock.minutes == 0) {
          
        game->Black_player.clock.a_tenth_of_a_second = 0;
      }
      else {
        game->Black_player.clock.a_tenth_of_a_second = 9;
        if (game->Black_player.clock.seconds == 0) {
          if (game->Black_player.clock.minutes > 0) {
            game->Black_player.clock.minutes--;
            game->Black_player.clock.seconds = 59;
          }
          else {
            game->Black_player.clock.seconds = 0;
          }
        }
        else {
          game->Black_player.clock.seconds--;
        }
      }
    }
  }

  if(can_draw_this){

  erase_buffer();

  swap_BackgroundBuffer();

  draw_board(&game->board);

  draw_clockValue(&game->Black_player, &game->White_player);

  draw_cursor_mouse(cursor.position.x, cursor.position.y , cursor.type);

  swap_buffers();
  }
}


/**
 * @brief Changes the game state to the pause menu.
 *
 * This function changes the current state of the game to the pause menu state. It erases the buffer and draws the pause menu on the screen.
 *
 * @return 0 upon successful state change.
 */
int (change_game_state_to_menu)(){
  current_state = PAUSE_MENU;
  erase_buffer();
  draw_pause_menu();
  return 0;
}
