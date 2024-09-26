/**
 * @file controller.h
 * @brief This file contains the declaration of the functions that will be used to control the game. It also contains the declaration of the global variables that will be used to control the game.
 * 
 *  
*/

#pragma once

#include <lcom/lcf.h>
#include <stdio.h>
#include "../model/game.h"
#include "../view/view.h"
#include "keyboard/keyboard.h"
#include "rtc/rtc.h"
#include "rtc/I_RTC.h"

#include "mouse/mouse.h"
#include "graphics/graphic.h"

/**
 * @brief Enumerated type for the keys that can be pressed.
 * 
 */
enum ClickedKey{
  NOKEY, /**< No key was pressed. */
  ARROW_UP, /**< The up arrow key was pressed. */
  ARROW_DOWN, /**< The down arrow key was pressed. */
  ARROW_LEFT, /**< The left arrow key was pressed. */
  ARROW_RIGHT, /**< The right arrow key was pressed. */
  ENTER, /**< The enter key was pressed. */
  ESC, /**< The escape key was pressed. */
  ONE, /**< The 1 key was pressed. */
  TWO,   /**< The 2 key was pressed. */
  THREE, /**< The 3 key was pressed. */
  FOUR, /**< The 4 key was pressed. */
  FIVE, /**< The 5 key was pressed. */
  SIX, /**< The 6 key was pressed. */
  SPACE, /**< The space key was pressed. */
};

/**
 * @brief Enumerated type for the states that the game can be in.
 * 
 */
enum FlowState{
  MENU, /**< The game is in the menu state. */
  GAME, /**< The game is in the game state. */
  NEW_GAME, /**< The game is in the new game state. */
  LOAD_GAME, /**< The game is in the load game state. */
  PAUSE_MENU, /**< The game is in the pause menu state. */
  INSTRUCTIONS, /**< The game is in the instructions state. */
  WINNER_SCREEN, /**< The game is in the winner screen state. */
  EXIT, /**< The game is in the exit state. */
};


/**
 * @brief Pointer to the current game instance.
 */
struct Game *game;

/**
 * @brief Array to store game boards.
 */
struct Board boardArray[1024];

/**
 * @brief Index variable for the boardArray.
 */
int index_;

/**
 * @brief Maximum index value for the boardArray.
 */
int max_index;

/**
 * @brief Parses keyboard input.
 */
void parse_keyboard_input();

/**
 * @brief Parses mouse input.
 */
void parse_mouse_input();

/**
 * @brief Updates the game state.
 */
void update_game();

/**
 * @brief Updates the menu state.
 */
void update_menu();

/**
 * @brief Routes to the appropriate functionality based on the current state.
 */
void router();


/**
 * @brief Initializes the game.
 *
 * @param game Pointer to the game structure.
 * @param minutes Number of minutes for player clocks.
 * @param seconds Number of seconds for player clocks.
 */
void init_game(struct Game *game,int minutes, int seconds);

/**
 * @brief Game loop function.
 *
 * @param game Pointer to the game structure.
 */
void game_loop(struct Game * game);


/**
 * @brief Decreases the player timer based on the current turn.
 */
void decrease_player_timer();

/**
 * @brief Changes the game state to the pause menu.
 *
 * This function changes the current state of the game to the pause menu state.
 * It erases the buffer and draws the pause menu on the screen.
 *
 * @return 0 upon successful state change.
 */
int(change_game_state_to_menu)();
