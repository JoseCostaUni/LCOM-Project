/**
 * @file enum.h
 * @brief This file contains the declaration of the enumerations that will be used in the game.
 *  
*/

#pragma once

/**
 * @brief Enum representing the type of a chess piece.
 */
enum PieceType {
  PAWN = 0, /**< pawn. */
  ROOK = 1, /**< rook. */
  KNIGHT = 2, /**< knight. */
  BISHOP = 3, /**< bishop. */
  QUEEN = 4, /**< queen. */
  KING = 5, /**< king. */
  CASTLE = 6, /**< castle. */
  EMPTY = 7 /**< empty. */
};

/**
 * @brief Enum representing the type of cursor.
 */
enum CursorType {
  DEFAULT = 0,  /**< Default cursor. */
  HOVERING = 1, /**< Cursor when hovering over a piece. */
  SELECTED = 2  /**< Cursor when a piece is selected. */
};


/**
 * @brief Enum representing the color of a chess piece.
 */
enum PieceColor {
  WHITE = 0,  /**< White piece. */
  BLACK = 1   /**< Black piece. */
};


/**
 * @brief Enum representing the possible states of a chess game.
 */
enum GameStates{
 START,      /**< Game starting state. */
  STALEMATE,  /**< Stalemate state. */
  CHECKMATE,  /**< Checkmate state. */
  CHECK,      /**< Check state. */
  DRAW,       /**< Draw state. */
  END,        /**< Game ending state. */
  ONGOING     /**< Ongoing game state. */
};


/**
 * @brief Enum representing the possible states of the game menu.
 */
enum MenuStates
{
    MAIN_MENU,      /**< Main menu state. */
    GAME_MENU,      /**< Game menu state. */
    SETTINGS_MENU,  /**< Settings menu state. */
    EXIT_MENU       /**< Exit menu state. */
};
