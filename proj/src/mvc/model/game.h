/**
 * @file game.h
 * @brief Header file containing declarations for the game logic and data structures.
 *
 * This file contains declarations for various data structures and functions
 * related to the game logic, including player information, board management,
 * piece movement, and game state management.
 */

#include <lcom/lcf.h>
#include <stdio.h>

#include "enum.h"

#pragma once



/**
 * @brief Structure representing a position on the chessboard.
 */
struct Position {
  uint8_t x;  /**< x coordinate */
  uint8_t y; /**< y coordinate */
};

/**
 * @brief Structure representing the position of the mouse cursor.
 */
struct mousePosition{
  uint32_t x;  /**< x coordinate */
  uint32_t y; /**< y coordinate */
};

/**
 * @brief Structure representing the clock used to keep track of the game time.
 */
struct Clock {
  int days;  /**< days */
  int seconds; /**< seconds */
  int minutes; /**< minutes */
  int hours; /**< hours */
  int a_tenth_of_a_second; /**< a tenth of a second */
};

/**
 * @brief Structure representing the cursor used to interact with the game.
 */
struct cursor {
  struct mousePosition position; /**< position of the cursor */
  enum CursorType type; /**< type of the cursor */
};

/**
 * @brief Structure representing a piece on the chessboard.
 */
struct Piece
{
  enum PieceType type; /**< type of the piece */
  struct Position position; /**< position of the piece */
  bool isAlive; /**< whether the piece is alive */
  bool isWhite; /**< whether the piece is white */
  bool canMove; /**< whether the piece can move */
  bool hasMoved; /**< whether the piece has moved */
  bool isSelected; /**< whether the piece is selected */
  int id; /**< id of the piece */
};

/**
 * @brief Structure representing a move made by a player.
 */
struct Move {
  struct Piece *piece; /**< piece that was moved */
  struct Position *init_pos; /**< initial position of the piece */
  struct Position *final_pos; /**< final position of the piece */
};

/**
 * @brief Structure representing a list of moves.
 */
struct Movelist {
  struct Move *moves[1024]; /**< array of moves */
  int index; /**< index of the last move */
};


/**
 * @brief Structure representing the game board.
 */
struct Board {
  struct Piece pieces[32]; /**< array of pieces */
  struct Piece squares[8][8]; /**< array of squares */
  char* moves[1024]; /**< array of moves */
  int movesIndex;   /**< index of the last move */
};


/**
 * @brief Structure representing a player.
 */
struct Player
{
  struct Piece pieces[16]; /**< array of pieces */
  struct Clock clock; /**< clock of the player */
  bool isWhite; /**< whether the player is white */
  bool isWinner; /**< whether the player is the winner */
  bool isDraw; /**< whether the player is in a draw */
  bool canLongCastle; /**< whether the player can long castle */
  bool canShortCastle; /**< whether the player can short castle */
};

/**
 * @brief Structure representing the game.
 */
struct Game{
  struct Board board;  /**< board of the game */
  struct Player Black_player; /**< black player */
  struct Player White_player; /**< white player */
  enum GameStates state;   /**< state of the game */
  uint8_t piece_count; /**< number of pieces */
  bool isWhiteTurn; /**< whether it is white's turn */
};

/**
 * @brief Creates a new game.
 * 
 * @return Pointer to the new game
 * 
 * @return Whether the game was created
 */
struct Game* create_game();

/**
 * @brief Destroys a game.
 * 
 * @param game Pointer to the game to be destroyed
 * 
 * @return Whether the game was destroyed
 */
void destroy_game(struct Game* game);

/**
 * @brief Initializes a game.
 * 
 * @param game Pointer to the game to be initialized
 * 
 */
struct Player* create_player();

/**
 * @brief Destroys a player.
 * 
 * @param player Pointer to the player to be destroyed
 *
 */
void destroy_player(struct Player* player);

/**
 * @brief Initializes a player.
 * 
 * @param player Pointer to the player to be initialized
 * @param isWhite Whether the player is white
 * @param minutes Initial minutes on the clock
 * @param seconds Initial seconds on the clock
 * 
 */
void init_player(struct Player *player, bool isWhite , int minutes, int seconds);

/**
 * @brief Initializes a clock.
 * 
 * @param clock Pointer to the clock to be initialized
 * @param minutes Initial minutes on the clock
 * @param seconds Initial seconds on the clock
 * 
 */
void initClock(struct Clock *clock , int minutes, int seconds);


/**
 * @brief Changes the state of the game.
 *
 * @param game Pointer to the game instance.
 * @param state The new state of the game.
 * 
 */
void changeState(struct Game *game, enum GameStates state);

/**
 * @brief Changes the turn of the game.
 *
 * @param game Pointer to the game instance.
 * 
 */
void changeTurn(struct Game *game);

/**
 * @brief Moves a chess piece on the board.
 *
 * @param game Pointer to the game instance.
 * @param PieceType The type of the piece.
 * @param piece Pointer to the piece to be moved.
 * @param init_pos Pointer to the initial position of the piece.
 * @param final_pos Pointer to the final position of the piece.
 * 
 */
void move_piece(struct Game *game, enum PieceType PieceType, struct Piece * piece,
                struct Position *init_pos, struct Position *final_pos);

/**
 * @brief Checks if the game is in a stalemate situation.
 *
 * @param game Pointer to the game instance.
 * @return true if the game is in a stalemate situation, false otherwise.
 */
bool is_stalemate(struct Game *game);

/**
 * @brief Checks if the game is in a draw situation.
 *
 * @param game Pointer to the game instance.
 * @return true if the game is in a draw situation, false otherwise.
 */
bool is_draw(struct Game *game);

/**
 * @brief Retrieves all possible moves for a given piece in the current game state.
 * 
 * @param game A pointer to the Game structure representing the current state of the game.
 * @param piece A pointer to the Piece structure representing the piece for which possible moves need to be determined.
 * 
 * @return A Movelist structure containing a list of possible moves for the given piece.
 */
struct Movelist get_possible_moves(struct Game *game, struct Piece *piece);

/**
 * @brief Creates and initializes a new game board.
 * 
 * @return A pointer to the newly created Board structure.
 */
struct Board* create_board();


/**
 * @brief Initializes a game board.
 * 
 * @param board Pointer to the Board structure to be initialized.
 */
void init_board(struct Board *board);

/**
 * @brief Checks if a movement of a specific piece type is legal from an initial position to a final position on the board.
 * 
 * @param board A pointer to the Board structure.
 * @param PieceType The type of the piece.
 * @param piece A pointer to the Piece structure.
 * @param init_pos A pointer to the initial Position structure.
 * @param final_pos A pointer to the final Position structure.
 * @return true if the movement is legal, false otherwise.
 */
bool is_movement_legal(struct Board *board, enum PieceType PieceType, struct Piece * piece, 
                  struct Position *init_pos , struct Position *final_pos);


/**
 * @brief Checks if a square on the board is occupied.
 * 
 * @param board A pointer to the Board structure.
 * @param pos A pointer to the Position structure.
 * @return true if the square is occupied, false otherwise.
 */
bool is_square_occupied(struct Board *board, struct Position *pos);

/**
 * @brief Checks if a piece can take another piece on a specific position on the board.
 * 
 * @param board A pointer to the Board structure.
 * @param pos A pointer to the Position structure.
 * @param piece A pointer to the Piece structure.
 * @return true if the piece can take another piece, false otherwise.
 */
bool can_take(struct Board *board, struct Position *pos, struct Piece * piece);

/**
 * @brief Checks if a position is inside the boundaries of the board.
 * 
 * @param pos A pointer to the Position structure.
 * @return true if the position is inside the board, false otherwise.
 */
bool is_inside_board(struct Position *pos);

/**
 * @brief Retrieves the piece located at the given click coordinates on the board.
 * 
 * @param click_x The x-coordinate of the click.
 * @param click_y The y-coordinate of the click.
 * @param square_size The size of each square on the board.
 * @param board A pointer to the Board structure.
 * @return A pointer to the Piece structure if a piece is found at the click coordinates, NULL otherwise.
 */
struct Piece* get_piece_from_click(int click_x, int click_y, int square_size, struct Board* board);

/**
 * @brief Changes the position of a piece on the board from an initial position to a final position.
 * 
 * @param piece A pointer to the Piece structure to be moved.
 * @param init_pos A pointer to the initial Position structure.
 * @param final_pos A pointer to the final Position structure.
 * @param board A pointer to the Board structure.
 * @return true if the piece's position was successfully changed, false otherwise.
 */
bool change_piece_position(struct Piece *piece,struct Position *init_pos, struct Position *final_pos, struct Board *board);

/**
 * @brief Removes a piece from the board at the specified position.
 * 
 * @param board A pointer to the Board structure.
 * @param pos A pointer to the Position structure indicating the position of the piece to be removed.
 */
void remove_piece_from_board(struct Board *board, struct Position *pos);

/**
 * @brief Checks if a movement of a specific piece type is legal without actually removing the piece from the board.
 * 
 * @param board A pointer to the Board structure.
 * @param PieceType The type of the piece.
 * @param piece A pointer to the Piece structure.
 * @param init_pos A pointer to the initial Position structure.
 * @param final_pos A pointer to the final Position structure.
 * @return true if the movement is legal, false otherwise.
 */
bool is_movement_legal_without_removing(struct Board *board, enum PieceType PieceType, struct Piece * piece, struct Position *init_pos , struct Position *final_pos);

/**
 * @brief Checks if the current player is in check.
 * 
 * @param game A pointer to the Game structure.
 * @return true if the current player is in check, false otherwise.
 */
bool is_check(struct Game *game);

/**
 * @brief Promotes a pawn to a queen on the board.
 * 
 * @param board A pointer to the Board structure.
 * @param pawn A pointer to the pawn Piece structure to be promoted.
 * @return The index of the promoted queen in the board's pieces array, or -1 if the promotion fails.
 */
int promote_pawn_to_queen(struct Board *board, struct Piece *pawn);
