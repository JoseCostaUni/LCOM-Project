/**
 * @file game.c
 * @brief Implementation of the chess game logic.
 * 
 * This file contains the functions required to manage the game state, players,
 * board setup, and move validation for a chess game.
 */

#include "game.h"
#include "../view/view.h"

extern enum FlowState current_state;

/**
 * @brief Creates a new game instance.
 *
 * This function Creates a new game by allocating memory for a new game instance and initializes it.
 *
 * @return A pointer to the newly created Game structure, or NULL if memory allocation fails.
 */

struct Game *create_game() {
  struct Game *game = (struct Game *) malloc(sizeof(struct Game));
  if (game == NULL) {
    return NULL;
  }
  return game;
}

/**
 * @brief Destroys the game instance.
 *
 * This function destroys the game by freeing memory allocated for the game instance.
 *
 * @param game A pointer to the Game structure to be destroyed.
 */
void destroy_game(struct Game *game) {
  free(game);
}

/**
 * @brief Changes the state of the game.
 *
 * This function updates the state of the game to the specified state.
 *
 * @param game A pointer to the Game structure.
 * @param state The new state of the game.
 */
void changeState(struct Game *game, enum GameStates state) {
  game->state = state;
}

/**
 * @brief Creates a new Player structure.
 *
 * This function creates a player by dynamically allocating memory for a new Player structure and initializes its members to default values.
 *
 * @return A pointer to the newly created Player structure, or NULL if memory allocation fails.
 */
struct Player *create_player() {

  struct Player *player = (struct Player *) malloc(sizeof(struct Player));
  if (player == NULL) {
    return NULL;
  }
  return player;
}

/**
 * @brief Frees the memory allocated for a player structure.
 *
 * This function destroys the player by freeing memory allocated for a player structure.
 *
 * @param player A pointer to the Player structure to be destroyed.
 */
void destroy_player(struct Player *player) {
  free(player);
}

/**
 * @brief Initializes a clock with the specified minutes and seconds.
 *
 * This function initializes a clock with the specified minutes and seconds. It sets the days, hours, tenths of a second, minutes, and seconds to the provided values.
 *
 * @param clock A pointer to the Clock structure to be initialized.
 * @param minutes The initial minutes for the clock.
 * @param seconds The initial seconds for the clock.
 */
void initClock(struct Clock *clock, int minutes, int seconds) {
  clock->days = 0;
  clock->hours = 0;
  clock->a_tenth_of_a_second = 0;
  clock->minutes = minutes;
  clock->seconds = seconds;
}

/**
 * @brief Initializes a player with the specified parameters.
 *
 * This function initializes a player with the given color (white or black), sets initial flags for winner and draw states to false, and initializes the player's clock with the provided minutes and seconds.
 *
 * @param player A pointer to the Player structure to be initialized.
 * @param isWhite A boolean indicating whether the player is white (true) or black (false).
 * @param minutes The initial minutes for the player's clock.
 * @param seconds The initial seconds for the player's clock.
 */
void init_player(struct Player *player, bool isWhite, int minutes, int seconds) {
  player->isWhite = isWhite;
  player->isWinner = false;
  player->isDraw = false;
  player->canLongCastle = true;
  player->canShortCastle = true;
  initClock(&player->clock, minutes, seconds);
}

/**
 * @brief Changes the turn in the game.
 *
 * This function toggles the turn between white and black players in the game.
 *
 * @param game A pointer to the Game structure representing the current game state.
 */
void changeTurn(struct Game *game) {
  game->isWhiteTurn = !game->isWhiteTurn;
}

/**
 * @brief Moves a piece on the board if the move is legal.
 *
 * This function attempts to move a piece on the board from the initial position to the final position, provided that the move is legal according to the rules of chess. If the move is valid, the function updates the position of the piece on the board.
 *
 * @param game A pointer to the Game structure representing the current game state.
 * @param PieceType The type of the piece being moved.
 * @param piece A pointer to the Piece structure representing the piece being moved.
 * @param init_pos A pointer to the initial Position structure representing the piece's current position.
 * @param final_pos A pointer to the final Position structure representing the desired destination position.
 */
void move_piece(struct Game *game, enum PieceType PieceType, struct Piece *piece,
                struct Position *init_pos, struct Position *final_pos) {
  struct Board *board = &game->board;
  if (is_movement_legal(board, PieceType, piece, init_pos, final_pos) && is_inside_board(final_pos) && piece->isWhite == game->isWhiteTurn) {
    for (int i = 0; i < 32; i++) {
      if (game->board.pieces[i].position.x == init_pos->x &&
          game->board.pieces[i].position.y == init_pos->y) {
        game->board.pieces[i].position.x = final_pos->x;
        game->board.pieces[i].position.y = final_pos->y;
        break;
      }
    }
  }
}

/**
 * @brief Checks if the current player's king is in check.
 *
 * This function determines if the current player's king is in check, meaning it is under attack by an opponent's piece and is threatened with capture on the next move.
 *
 * @param game A pointer to the Game structure representing the current game state.
 * @return true if the current player's king is in check, false otherwise.
 */
bool is_check(struct Game *game) {
  struct Board *board = &game->board;
  struct Position king_pos;
  bool isWhite = game->isWhiteTurn;

  for (int i = 0; i < 32; i++) {
    if (board->pieces[i].type == KING) {
      if (board->pieces[i].isWhite != game->isWhiteTurn) {
        king_pos.x = board->pieces[i].position.x;
        king_pos.y = board->pieces[i].position.y;

        break;
      }
    }
  }

  for (int i = 0; i < 32; i++) {
    if (board->pieces[i].isWhite == isWhite && board->pieces[i].type != EMPTY && board->pieces[i].type != KING) {
      struct Movelist possible_moves = get_possible_moves(game, &board->pieces[i]);

      for (int j = 0; j < possible_moves.index; j++) {
        struct Move *move = possible_moves.moves[j];
        struct Position *init_pos = move->init_pos;
        struct Position *final_pos = move->final_pos;

        if (is_movement_legal_without_removing(board, board->pieces[i].type, &board->pieces[i], init_pos, final_pos) && king_pos.x == final_pos->x && king_pos.y == final_pos->y) {
          return true;
        }
      }
      free(possible_moves.moves);
      possible_moves.index = 0;
    }
  }
  return false;
}

/**
 * @brief Checks if the game is in a stalemate state.
 *
 * This function checks if the current player is in a stalemate position, meaning they have no legal moves available and the game is not in checkmate.
 *
 * @param game A pointer to the Game structure representing the current game state.
 * @return true if the game is in a stalemate state, false otherwise.
 */
bool is_stalemate(struct Game *game) {

  if (game->isWhiteTurn) {
    for (int i = 0; i < 16; i++) {
      struct Piece piece = game->White_player.pieces[i];

      if (piece.canMove) {
        return false;
      }
    }
  }
  else {
    for (int i = 0; i < 16; i++) {
      struct Piece piece = game->Black_player.pieces[i];

      if (piece.canMove) {
        return false;
      }
    }
  }

  return true;
}

/**
 * @brief Checks if the game is in a draw state.
 *
 * This function checks if the current game state qualifies as a draw based on the remaining pieces on the board. It considers various conditions such as pawn, queen, rook, bishop, and knight counts to determine if the game is in a draw state.
 *
 * @param game A pointer to the Game structure representing the current game state.
 * @return true if the game is in a draw state, false otherwise.
 */
bool is_draw(struct Game *game) {

  int whiteKnightCount = 0;
  int blackKnightCount = 0;
  int whiteQueenCount = 0;
  int blackQueenCount = 0;
  int whiteBishopCount = 0;
  int blackBishopCount = 0;
  int whiteRookCount = 0;
  int blackRookCount = 0;
  int whitePawnCount = 0;
  int blackPawnCount = 0;

  for (int i = 0; i < 32; i++) {

    switch (game->board.pieces[i].type) {
      case PAWN:
        if (game->board.pieces[i].isWhite) {
          whitePawnCount++;
        }
        else {
          blackPawnCount++;
        }
        break;
      case ROOK:
        if (game->board.pieces[i].isWhite) {
          whiteRookCount++;
        }
        else {
          blackRookCount++;
        }
        break;
      case BISHOP:
        if (game->board.pieces[i].isWhite) {
          whiteBishopCount++;
        }
        else {
          blackBishopCount++;
        }
        break;
      case QUEEN:
        if (game->board.pieces[i].isWhite) {
          whiteQueenCount++;
        }
        else {
          blackQueenCount++;
        }
        break;
      case KNIGHT:
        if (game->board.pieces[i].isWhite) {
          whiteKnightCount++;
        }
        else {
          blackKnightCount++;
        }
        break;
      default:
        break;
    }
  }

  if (whitePawnCount > 0 || blackPawnCount > 0 || blackQueenCount > 0 || whiteQueenCount > 0 || blackRookCount > 0 || whiteRookCount > 0) {
    return false;
  }

  if ((whiteBishopCount == 2 || blackBishopCount == 2)) {
    return false;
  }

  if ((whiteBishopCount == 1 && whiteKnightCount == 1) ||
      (blackBishopCount == 1 && blackKnightCount == 1)) {
    return false;
  }

  return true;
}

/**
 * @brief Gets the possible legal moves for a given piece on the board.
 *
 * This function determines all possible legal moves for a given piece on the chessboard based on the current game state. It considers the rules of movement without actually removing any pieces from the board.
 *
 * @param game A pointer to the Game structure representing the current game state.
 * @param piece A pointer to the Piece structure for which possible moves are to be determined.
 * @return A Movelist structure containing an array of Move structures representing the possible moves for the piece.
 */
struct Movelist get_possible_moves(struct Game *game, struct Piece *piece) {
  struct Movelist possible_moves;
  possible_moves.index = 0;
  struct Position pos;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      pos.x = i;
      pos.y = j;

      if (is_movement_legal_without_removing(&game->board, piece->type, piece, &piece->position, &pos)) {
        struct Move *move = malloc(sizeof(struct Move));
        move->init_pos = malloc(sizeof(struct Position));
        move->final_pos = malloc(sizeof(struct Position));
        move->piece = malloc(sizeof(struct Piece));

        move->piece = piece;
        move->init_pos->x = piece->position.x;
        move->init_pos->y = piece->position.y;
        move->final_pos->x = i;
        move->final_pos->y = j;

        possible_moves.moves[possible_moves.index] = move;
        possible_moves.index++;
      }
    }
  }

  return possible_moves;
}

/**
 * @brief Creates a new chessboard.
 *
 * This function creates a new chessboard by dynamically allocating memory for a new Board structure representing the chessboard.
 *
 * @return A pointer to the newly created Board structure, or NULL if memory allocation fails.
 */
struct Board *create_board() {
  struct Board *board = (struct Board *) malloc(sizeof(struct Board));
  if (board == NULL) {
    return NULL;
  }
  return board;
}

/**
 * @brief Destroys the chessboard.
 *
 * This function "Deletes" the chessboard by freeing memory allocated for the chessboard structure.
 *
 * @param board A pointer to the Board structure representing the game board.
 */
void destroy_board(struct Board *board) {
  free(board);
}

/**
 * @brief Initializes the chessboard with pieces in their starting positions.
 *
 * This function initializes the chessboard with pieces in their starting positions. It sets up the board with empty squares and places white and black pieces according to standard chess starting positions.
 *
 * @param board A pointer to the Board structure representing the game board.
 */
void init_board(struct Board *board) {

  board->movesIndex = 0;
  for (int i = 0; i < 1024; i++) {
    board->moves[i] = NULL;
  }

  // Initialize squares
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board->squares[i][j].type = EMPTY;
      board->squares[i][j].position.x = i;
      board->squares[i][j].position.y = j;
      board->squares[i][j].isAlive = false;
      board->squares[i][j].isWhite = false;
      board->squares[i][j].canMove = false;
      board->squares[i][j].hasMoved = false;
    }
  }

  // Initialize white pawns
  for (int j = 0; j < 8; j++) {
    struct Piece whitePawn = {PAWN, {j, 1}, true, true, true, false, false, j + 1};
    board->squares[j][1] = whitePawn;
    board->pieces[j] = whitePawn;
  }

  // Initialize black pawns
  for (int j = 0; j < 8; j++) {
    struct Piece blackPawn = {PAWN, {j, 6}, true, false, true, false, false, 9 + j};
    board->squares[j][6] = blackPawn;
    board->pieces[16 + j] = blackPawn;
  }

  // Initialize white pieces
  struct Piece whitePieces[] = {
    {ROOK, {0, 0}, true, true, true, false, false, 17},
    {KNIGHT, {1, 0}, true, true, true, false, false, 18},
    {BISHOP, {2, 0}, true, true, true, false, false, 19},
    {QUEEN, {3, 0}, true, true, true, false, false, 20},
    {KING, {4, 0}, true, true, true, false, false, 21},
    {BISHOP, {5, 0}, true, true, true, false, false, 22},
    {KNIGHT, {6, 0}, true, true, true, false, false, 23},
    {ROOK, {7, 0}, true, true, true, false, false, 24}};

  for (int i = 0; i < 8; i++) {
    board->squares[i][0] = whitePieces[i];
    board->pieces[8 + i] = whitePieces[i];
  }

  // Initialize black pieces
  struct Piece blackPieces[] = {
    {ROOK, {0, 7}, true, false, true, false, false, 25},
    {KNIGHT, {1, 7}, true, false, true, false, false, 26},
    {BISHOP, {2, 7}, true, false, true, false, false, 27},
    {QUEEN, {3, 7}, true, false, true, false, false, 28},
    {KING, {4, 7}, true, false, true, false, false, 29},
    {BISHOP, {5, 7}, true, false, true, false, false, 30},
    {KNIGHT, {6, 7}, true, false, true, false, false, 31},
    {ROOK, {7, 7}, true, false, true, false, false, 32}};

  for (int i = 0; i < 8; i++) {
    board->squares[i][7] = blackPieces[i];
    board->pieces[24 + i] = blackPieces[i];
  }
}

/**
 * @brief Checks if there is any piece in front of a given position on the board.
 *
 * This function checks if there is any piece in front of a given initial position on the board in the direction of a given final position.
 *
 * @param board A pointer to the Board structure representing the game board.
 * @param initialPos A pointer to the Position structure representing the initial position.
 * @param finalPos A pointer to the Position structure representing the final position.
 * @return true if there is any piece in front, false otherwise.
 */
bool is_piece_in_front(struct Board *board, struct Position *initialPos, struct Position *finalPos) {
  if (initialPos->x == finalPos->x) {
    if (initialPos->y < finalPos->y) {
      for (int i = initialPos->y + 1; i < finalPos->y; i++) {
        if (board->squares[initialPos->x][i].type != EMPTY) {
          return true;
        }
      }
    }
    else {
      for (int i = initialPos->y - 1; i > finalPos->y; i--) {
        if (board->squares[initialPos->x][i].type != EMPTY) {
          return true;
        }
      }
    }
  }
  else if (initialPos->y == finalPos->y) {
    if (initialPos->x < finalPos->x) {
      for (int i = initialPos->x + 1; i < finalPos->x; i++) {
        if (board->squares[i][initialPos->y].type != EMPTY) {
          return true;
        }
      }
    }
    else {
      for (int i = initialPos->x - 1; i > finalPos->x; i--) {
        if (board->squares[i][initialPos->y].type != EMPTY) {
          return true;
        }
      }
    }
  }
  return false;
}

/**
 * @brief Checks if there is any piece in the diagonal path between two positions.
 *
 * This function checks if there is any piece in the diagonal path between the initial position and the final position on the board.
 *
 * @param board A pointer to the Board structure representing the game board.
 * @param initialPos A pointer to the Position structure representing the initial position.
 * @param finalPos A pointer to the Position structure representing the final position.
 * @return true if there is any piece in the diagonal path, false otherwise.
 */
bool is_piece_in_diagonal(struct Board *board, struct Position *initialPos, struct Position *finalPos) {
  if (initialPos->x < finalPos->x && initialPos->y < finalPos->y) {
    for (int i = initialPos->x + 1, j = initialPos->y + 1; i < finalPos->x && j < finalPos->y; i++, j++) {
      if (board->squares[i][j].type != EMPTY) {
        return true;
      }
    }
  }
  else if (initialPos->x < finalPos->x && initialPos->y > finalPos->y) {
    for (int i = initialPos->x + 1, j = initialPos->y - 1; i < finalPos->x && j > finalPos->y; i++, j--) {
      if (board->squares[i][j].type != EMPTY) {
        return true;
      }
    }
  }
  else if (initialPos->x > finalPos->x && initialPos->y < finalPos->y) {
    for (int i = initialPos->x - 1, j = initialPos->y + 1; i > finalPos->x && j < finalPos->y; i--, j++) {
      if (board->squares[i][j].type != EMPTY) {
        return true;
      }
    }
  }
  else if (initialPos->x > finalPos->x && initialPos->y > finalPos->y) {
    for (int i = initialPos->x - 1, j = initialPos->y - 1; i > finalPos->x && j > finalPos->y; i--, j--) {
      if (board->squares[i][j].type != EMPTY) {
        return true;
      }
    }
  }

  return false;
}

/**
 * @brief Checks if a square on the board is occupied by a piece.
 *
 * This function checks if the square at the specified position on the board is occupied by a piece.
 *
 * @param board A pointer to the Board structure representing the game board.
 * @param pos A pointer to the Position structure representing the position to be checked.
 * @return true if the square at the specified position is occupied by a piece, false otherwise.
 */
bool is_square_occupied(struct Board *board, struct Position *pos) {
  return board->squares[pos->x][pos->y].type != EMPTY;
}

/**
 * @brief Checks if a piece can take another piece at a given position.
 *
 * This function determines whether a piece can take another piece located at the specified position on the board. It checks if the target position is occupied by an opponent's piece, considering the color of the pieces involved.
 *
 * @param board A pointer to the Board structure representing the game board.
 * @param pos A pointer to the Position structure representing the position of the piece to be taken.
 * @param piece A pointer to the Piece structure representing the piece attempting to take another piece.
 * @return true if the piece can take the opponent's piece at the specified position, false otherwise.
 */
bool can_take(struct Board *board, struct Position *pos, struct Piece *piece) {
  return board->squares[pos->x][pos->y].type != EMPTY &&
         board->squares[pos->x][pos->y].isWhite != piece->isWhite;
}

/**
 * @brief Checks if a movement of a piece on the board is legal.
 *
 * This function determines whether a movement of a piece from an initial position to a final position on the board is legal according to the rules of chess. It considers the type of the piece, its initial and final positions, and the current state of the board. If the movement is legal, it may also update certain attributes of the piece and the board.
 *
 * @param board A pointer to the Board structure representing the game board.
 * @param PieceType The type of the piece being moved.
 * @param piece A pointer to the Piece structure representing the piece being moved.
 * @param init_pos A pointer to the initial Position structure.
 * @param final_pos A pointer to the final Position structure.
 * @return true if the movement is legal and false otherwise.
 */
bool is_movement_legal(struct Board *board, enum PieceType PieceType, struct Piece *piece,
                       struct Position *init_pos, struct Position *final_pos) {
  switch (PieceType) {
    case PAWN:
      if (!piece->hasMoved) {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if (((init_pos->y - final_pos->y) <= 2 &&
             (init_pos->y - final_pos->y) > 0 && (piece->isWhite == false)) ||
            ((init_pos->y - final_pos->y) >= -2 &&
             (init_pos->y - final_pos->y) < 0 && (piece->isWhite == true))) {

          if (!isSquareOccupied && !isPieceInFront && init_pos->x == final_pos->x) {
            piece->hasMoved = true;
            return true;
          }

          if (can_take(board, final_pos, piece) && (abs(init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == -1) && piece->isWhite == true) {
            piece->hasMoved = true;
            return true;
          }

          if (can_take(board, final_pos, piece) && (abs(init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == 1) && piece->isWhite == false) {
            return true;
          }

          return false;
        }
      }
      else {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if (((init_pos->y - final_pos->y) == 1 && piece->isWhite == false) ||
            ((init_pos->y - final_pos->y) == -1 && piece->isWhite == true)) {

          if (!isSquareOccupied && !isPieceInFront && init_pos->x == final_pos->x) {
            return true;
          }

          if (can_take(board, final_pos, piece) && (abs(init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == -1) && piece->isWhite == true) {
            remove_piece_from_board(board, final_pos);
            piece->hasMoved = true;
            return true;
          }

          if (can_take(board, final_pos, piece) && (abs(init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == 1) && piece->isWhite == false) {
            remove_piece_from_board(board, final_pos);
            return true;
          }

          return false;
        }
      }
      break;

    case ROOK:
      if (init_pos->x == final_pos->x || init_pos->y == final_pos->y) {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if (!isSquareOccupied && !isPieceInFront) {
          piece->hasMoved = true;
          return true;
        }

        if (can_take(board, final_pos, piece) && (init_pos->x == final_pos->x || init_pos->y == final_pos->y) && !isPieceInFront) {
          remove_piece_from_board(board, final_pos);
          piece->hasMoved = true;
          return true;
        }
      }
      return false;
      break;

    case KNIGHT:
      if (is_inside_board(final_pos)) {
        int x_diff = init_pos->x - final_pos->x;
        int y_diff = init_pos->y - final_pos->y;
        bool isSquareOccupied = is_square_occupied(board, final_pos);
        if ((abs(x_diff) == 2 && abs(y_diff) == 1) || (abs(x_diff) == 1 && abs(y_diff) == 2)) {
          if (!isSquareOccupied) {
            return true;
          }

          if (can_take(board, final_pos, piece)) {
            remove_piece_from_board(board, final_pos);
            return true;
          }
        }
      }
      break;

      return false;
      break;
    case BISHOP:
      if (is_inside_board(final_pos)) {
        if (abs(init_pos->x - final_pos->x) == abs(init_pos->y - final_pos->y)) {
          bool isPieceInDiagonal = is_piece_in_diagonal(board, init_pos, final_pos);
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if (isSquareOccupied && !isPieceInDiagonal && can_take(board, final_pos, piece)) {
            remove_piece_from_board(board, final_pos);
            return true;
          }

          if (!isSquareOccupied && !isPieceInDiagonal) {
            return true;
          }
        }
        return false;
      }
      break;
    case QUEEN:
      if (is_inside_board(final_pos)) {
        if (init_pos->x == final_pos->x || init_pos->y == final_pos->y) {
          bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if (!isSquareOccupied && !isPieceInFront) {
            piece->hasMoved = true;
            return true;
          }

          if (can_take(board, final_pos, piece) && !isPieceInFront && (init_pos->x == final_pos->x || init_pos->y == final_pos->y)) {
            remove_piece_from_board(board, final_pos);
            piece->hasMoved = true;
            return true;
          }
        }
        else if (abs(init_pos->x - final_pos->x) == abs(init_pos->y - final_pos->y)) {
          bool isPieceInDiagonal = is_piece_in_diagonal(board, init_pos, final_pos);
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if (isSquareOccupied && !isPieceInDiagonal && can_take(board, final_pos, piece)) {
            remove_piece_from_board(board, final_pos);
            return true;
          }

          if (!isSquareOccupied && !isPieceInDiagonal) {
            return true;
          }
        }
        return false;
      }
      break;
    case KING:
      if (is_inside_board(final_pos)) {
        if (abs(init_pos->x - final_pos->x) <= 1 && abs(init_pos->y - final_pos->y) <= 1) {
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if (!isSquareOccupied) {
            return true;
          }

          if (can_take(board, final_pos, piece) && board->squares[final_pos->x][final_pos->y].type != KING) {
            remove_piece_from_board(board, final_pos);
            return true;
          }
        }
      }
      return false;
      break;
    case CASTLE:
      /* code */
      break;
    default:
      break;
  }

  return false;
}

/**
 * @brief Checks if a given position is within the boundaries of the board.
 * 
 * This function determines whether the provided position lies within the valid range of the board's coordinates (0 to 7 for both x and y axes).
 * 
 * @param pos A pointer to the Position structure containing the coordinates to be checked.
 * @return true if the position is inside the board boundaries, false otherwise.
 */
bool is_inside_board(struct Position *pos) {
  return (pos->x >= 0 && pos->x < 8 && pos->y >= 0 && pos->y < 8);
}

/**
 * @brief Retrieves a piece from the board based on a click position.
 * 
 * This function calculates the board position based on the click coordinates, converts them to board coordinates, and returns the piece located at that position if it exists and the board position is valid.
 * 
 * @param click_x The x-coordinate of the click.
 * @param click_y The y-coordinate of the click.
 * @param square_size The size of each square on the board in pixels.
 * @param board A pointer to the Board structure containing the current game state.
 * @return A pointer to the Piece structure located at the click position, or NULL if no piece is present or the position is invalid.
 */
struct Piece *get_piece_from_click(int click_x, int click_y, int square_size, struct Board *board) {

  struct Position pos;

  pos.x = (click_x - 200) / square_size;
  pos.y = (click_y - 100) / square_size;

  if (board == NULL) {
    printf("null board\n");
    return NULL;
  }

  if (is_inside_board(&pos)) {

    if (board->squares[pos.x][pos.y].type != EMPTY) {
      return &board->squares[pos.x][pos.y];
    }
  }

  return NULL;
}

/**
 * @brief Changes the position of a piece on the board if the move is legal.
 *
 * This function checks if the movement of a piece from its initial position to the final position   
is legal and within the board boundaries. If the move is valid, it updates the piece's position 
on the board and in the piece's own data structure, and advances the piece.
 *
 * @param piece A pointer to the Piece structure to be moved.
 * @param init_pos A pointer to the Position structure representing the initial position of the piece.
 * @param final_pos A pointer to the Position structure representing the final position of the piece.
 * @param board A pointer to the Board structure containing the current game state.
 * @return true if the piece's position was successfully changed, false otherwise.
 */
bool change_piece_position(struct Piece *piece,
                           struct Position *init_pos, struct Position *final_pos, struct Board *board) {

  if (is_movement_legal(board, piece->type, piece, init_pos, final_pos) && is_inside_board(final_pos)) {
    for (int i = 0; i < 32; i++) {
      if (board->pieces[i].position.x == init_pos->x && board->pieces[i].position.y == init_pos->y) {
        advance_piece(piece, final_pos, board);
        board->pieces[i].position.x = final_pos->x;
        board->pieces[i].position.y = final_pos->y;
        piece->position.x = final_pos->x;
        piece->position.y = final_pos->y;
        board->squares[final_pos->x][final_pos->y] = *piece;
        board->squares[init_pos->x][init_pos->y].type = EMPTY;
        return true;
        break;
      }
    }
  }
  return false;
}

/**
 * @brief Removes a piece from the board at the specified position.
 * 
 * This function searches for a piece at the given position on the board and removes it. It sets the board square at the position to be empty and updates the piece's position and type to indicate it has been removed.
 * 
 * @param board A pointer to the Board structure representing the game board.
 * @param pos A pointer to the Position structure indicating the coordinates of the piece to be removed.
 */
void remove_piece_from_board(struct Board *board, struct Position *pos) {
  for (int i = 0; i < 32; i++) {
    if (board->pieces[i].position.x == pos->x && board->pieces[i].position.y == pos->y) {

      board->squares[pos->x][pos->y].type = EMPTY;
      board->squares[pos->x][pos->y].isWhite = false;
      board->squares[pos->x][pos->y].hasMoved = false;
      board->pieces[i].type = EMPTY;
      board->pieces[i].position.x = -1;
      board->pieces[i].position.y = -1;
      break;
    }
  }
}


/**
 * @brief Checks if a piece's movement from an initial position to a final position is legal without removing the piece.
 * 
 * This function verifies the legality of a piece's movement on the board based on its type and the positions involved. It considers specific rules for each piece type, such as pawns, rooks, knights, bishops, queens, and kings.
 * 
 * @param board A pointer to the Board structure representing the game board.
 * @param PieceType An enumeration value representing the type of the piece (e.g., PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING).
 * @param piece A pointer to the Piece structure representing the piece to be moved.
 * @param init_pos A pointer to the Position structure indicating the initial position of the piece.
 * @param final_pos A pointer to the Position structure indicating the final position of the piece.
 * @return true if the movement is legal, false otherwise.
 */
bool is_movement_legal_without_removing(struct Board *board, enum PieceType PieceType, struct Piece *piece,
                                        struct Position *init_pos, struct Position *final_pos) {
  switch (PieceType) {
    case PAWN:
      if (!piece->hasMoved) {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if (((init_pos->y - final_pos->y) <= 2 &&
             (init_pos->y - final_pos->y) > 0 && (piece->isWhite == false)) ||
            ((init_pos->y - final_pos->y) >= -2 &&
             (init_pos->y - final_pos->y) < 0 && (piece->isWhite == true))) {

          if (!isSquareOccupied && !isPieceInFront && init_pos->x == final_pos->x) {
            return true;
          }

          if (can_take(board, final_pos, piece) && (abs(init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == -1) && piece->isWhite == true) {
            return true;
          }

          if (can_take(board, final_pos, piece) && (abs(init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == 1) && piece->isWhite == false) {
            return true;
          }

          return false;
        }
      }
      else {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if (((init_pos->y - final_pos->y) == 1 && piece->isWhite == false) ||
            ((init_pos->y - final_pos->y) == -1 && piece->isWhite == true)) {

          if (!isSquareOccupied && !isPieceInFront && init_pos->x == final_pos->x) {
            return true;
          }

          if (can_take(board, final_pos, piece) && (abs(init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == -1) && piece->isWhite == true) {
            return true;
          }

          if (can_take(board, final_pos, piece) && (abs(init_pos->x - final_pos->x) == 1 && (init_pos->y - final_pos->y) == 1) && piece->isWhite == false) {
            return true;
          }

          return false;
        }
      }
      break;

    case ROOK:
      if (init_pos->x == final_pos->x || init_pos->y == final_pos->y) {
        bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
        bool isSquareOccupied = is_square_occupied(board, final_pos);

        if (!isSquareOccupied && !isPieceInFront) {
          return true;
        }

        if (can_take(board, final_pos, piece) && (init_pos->x == final_pos->x || init_pos->y == final_pos->y)) {
          return true;
        }
      }
      return false;
      break;

    case KNIGHT:
      if (is_inside_board(final_pos)) {
        int x_diff = init_pos->x - final_pos->x;
        int y_diff = init_pos->y - final_pos->y;
        bool isSquareOccupied = is_square_occupied(board, final_pos);
        if ((abs(x_diff) == 2 && abs(y_diff) == 1) || (abs(x_diff) == 1 && abs(y_diff) == 2)) {
          if (!isSquareOccupied) {
            return true;
          }

          if (can_take(board, final_pos, piece)) {
            return true;
          }
        }
      }
      break;

      return false;
      break;
    case BISHOP:
      if (is_inside_board(final_pos)) {
        if (abs(init_pos->x - final_pos->x) == abs(init_pos->y - final_pos->y)) {
          if (!is_square_occupied(board, final_pos) && !is_piece_in_diagonal(board, init_pos, final_pos)) {
            bool isPieceInDiagonal = is_piece_in_diagonal(board, init_pos, final_pos);
            bool isSquareOccupied = is_square_occupied(board, final_pos);

            if (isSquareOccupied && can_take(board, final_pos, piece)) {
              return true;
            }

            if (!isSquareOccupied && !isPieceInDiagonal) {
              return true;
            }
          }
        }
        return false;
      }
      break;
    case QUEEN:
      if (is_inside_board(final_pos)) {
        if (init_pos->x == final_pos->x || init_pos->y == final_pos->y) {
          bool isPieceInFront = is_piece_in_front(board, init_pos, final_pos);
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if (!isSquareOccupied && !isPieceInFront) {
            return true;
          }

          if (can_take(board, final_pos, piece) && (init_pos->x == final_pos->x || init_pos->y == final_pos->y)) {
            return true;
          }
        }
        else if (abs(init_pos->x - final_pos->x) == abs(init_pos->y - final_pos->y)) {
          bool isPieceInDiagonal = is_piece_in_diagonal(board, init_pos, final_pos);
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if (isSquareOccupied && can_take(board, final_pos, piece)) {
            return true;
          }

          if (!isSquareOccupied && !isPieceInDiagonal) {
            return true;
          }
        }
        return false;
      }
      break;
    case KING:
      if (is_inside_board(final_pos)) {
        if (abs(init_pos->x - final_pos->x) <= 1 && abs(init_pos->y - final_pos->y) <= 1) {
          bool isSquareOccupied = is_square_occupied(board, final_pos);

          if (!isSquareOccupied) {
            return true;
          }

          if (can_take(board, final_pos, piece)) {
            return true;
          }
        }
      }
      return false;
      break;
    case CASTLE:
      /* code */
      break;
    default:
      break;
  }

  return false;
}

/**
 * @brief Promotes a pawn to a queen on the board.
 *
 * This function promotes a specified pawn to a queen. It updates the type of the piece in the pieces array and on the board's squares array. The function returns the index of the piece in the pieces array or -1 if not found.
 *
 * @param board A pointer to the Board structure representing the game board.
 * @param pawn A pointer to the Piece structure representing the pawn to be promoted.
 * @return The index of the promoted piece in the board's pieces array, or -1 if the pawn is not found.
 */
int promote_pawn_to_queen(struct Board *board, struct Piece *pawn) {

  for (int i = 0; i < 32; i++) {
    if (board->pieces[i].position.x == pawn->position.x && board->pieces[i].position.y == pawn->position.y) {
      board->pieces[i].type = QUEEN;
      board->squares[pawn->position.x][pawn->position.y].type = QUEEN;
      pawn->type = QUEEN;
      return i;
    }
  }
  return -1;
}
