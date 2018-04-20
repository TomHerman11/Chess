/*
 * Game.h
 *
 *  Created on: Feb 25, 2018
 *      Author: krimolovsky
 */

#ifndef CHESSGAME_H_
#define CHESSGAME_H_

#include "ChessHistory.h"

#define BOARD_LENGTH 8
#define DEFAULT_GAME_MODE 1
#define DEFAULT_DIFFICULTY 2
#define DEFAULT_USER_COLOR 1
#define PLAYER_1 'w'
#define PLAYER_2 'b'
#define DEFAULT_CURRENT_PLAYER 1
#define EMPTY_ENTRY '_'

#define GAME_MEMORY_FAILURE 666
#define GAME_GETMOVES_MEMORY_FAILURE 667


typedef struct chess_game_t {
	char gameBoard[BOARD_LENGTH][BOARD_LENGTH];
	int game_mode; //Playing against the computer-1. 2-players mode- 2.
	int difficulty;// The difficulty level. In use only in first game mode.
	int user_color; // The user's color- 1 for white and 0  for black. In use only in first game mode.
	int currentPlayer; //Equals 1 when it's the white player's turn and 0 when it's the black player's.
	int isWhiteKingThreatened; // If the white king is threatened the value is 1. Otherwise-0
	int isBlackKingThreatened; // If the black king is threatened the value is 1. Otherwise-0
	ChessHistory* history;
} ChessGame;



/**
 * Creates a new game with a specified history size.
 * Settings parameters are set to their default values, the
 * board is set to it's starting state.
 * @return
 * NULL if a memory allocation failure occurred.
 * Otherwise, a new game instant is returned.
 */
ChessGame* createChessGame();


/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param game - the source game which will be copied
 *	@return
 *	NULL if either game is NULL or a memory allocation failure occurred.
 *	Otherwise, a new copy of the source game is returned.
 */
ChessGame* copyChessGame(ChessGame* game);


/**
 * Frees all memory allocation associated with the given game.
 * If game==NULL - the function does nothing.
 */
void destoryChessGame(ChessGame* game);

/*
 * Set the settings parameters of the given game to their default values.
 * game_mode- DEFAULT_GAME_MODE
 * difficulty- DEFAULT_DIFFICULTY
 * user_color- DEFAULT_USER_COLOR
 */
void setChessGameSettingsDefault(ChessGame* game);

/*
 * Change the mode of the game to new_game_mode if possible.
 * The function assumes game!=NULL
 * @return
 * If new_game_mode is not a legal mode(1 or 2) then mode change isn't possible,
 * and the function returns 0.
 * If mode change occurred, the function returns 1.
 */
int changeGameMode(ChessGame* game, int new_game_mode);
/*
 * Change the game difficulty to new_difficulty if possible.
 * The function assumes game!=NULL
 * @return
 * If new_difficulty is not a legal difficulty level(1-5) then difficulty change isn't possible,
 * and the function returns 0.
 * If the game's difficulty was changed, the function returns 1.
 */
int changeGameDifficuly(ChessGame* game, int new_difficulty);

/*
 * Change the user's color to new_color if possible.
 * The function assumes game!=NULL
 * @return
 * If new_color is not a legal color (0 or 1) then user_color change isn't possible,
 * and the function returns 0.
 * If the user's color was changed, the function returns 1.
 */
int changeUserColor(ChessGame* game, int new_color);

/*
 * The function receives a valid move and perform it on the game.
 * If the move threatens the opponent's king, the parameter isWhiteKingThreatened/ isBlackKingThreatened
 *  is set to 1, according to the opponrnt's color.
 *
 * @param game- the source game
 * @param pre_x- current position's row
 * @param pre_y- current position's column
 * @param new_x- new position's row
 * @param new_y- new position's column
 */
void setMoveChessGame(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y);

/*
 * Checks if the move <pre_x,pre_y> to <new_x,new_y> is a valid move.
 * The function assumes game!=NULL
 *
 * @param game- the source game
 * @param pre_x- current position's row
 * @param pre_y- current position's column
 * @param new_x- new position's row
 * @param new_y- new position's column
 *
 * @return
 * 1- if either origin position or destination position is not a valid position on the board.
 * 2- if the origin position <pre_x,pre_y> doesn't contain a piece of the current player.
 * 3- if either the the piece doesn't move, i.e. the new position equal to the previous position,
 * 	  the new location contain a piece that belong to the current player, or the move is not
 *    a legal move for the specific piece in the original place.
 * 4- after the move the player's king is threatened.
 * GAME_MEMORY_FAILURE - if a memory allocation failure occurred.
 * 0- if the move is a legal move.
 */
int isValidMoveChessGame(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y);

/*
 * Returns the type of the piece in place <x,y>, assuming <x,y> is a legal place on the
 * board and game!-NULL
 *
 * @param game - the source game
 * @param x- the position's row
 * @param y- the position's column
 *
 * @return
 * If the piece in <x,y> is white- return 1.
 * If the piece is black- return 0.
 * If there is no piece in the given position- return 2.
 */
int checkPieceType(ChessGame* game, char x, char y);

/*
 * Check if the pawn in place <pre_x, pre_y> can be moved to <new_x, new_y>.
 * The function assumes both positions are legal, <pre_x, pre_y> contains a pawn of
 * game->current_player, <pre_x, pre_y>!=<new_x, new_y>, and that game!=NULL
 *
 * @param game- the source game
 * @param pre_x- current position's row
 * @param pre_y- current position's column
 * @param new_x- new position's row
 * @param new_y- new position's column
 *
 * @return
 * 1- if the move is valid
 * 0- otherwise
 */
int isValidMovePawn(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y);

/*
 * Check if the rook in place <pre_x, pre_y> can be moved to <new_x, new_y>.
 * The function assumes both positions are legal, <pre_x, pre_y> contains a rook,
 * <pre_x, pre_y>!=<new_x, new_y>, <new_x, new_y> doesn't contain a piece of the same color as the
 * given rook, and that game!=NULL
 *
 * @param game- the source game
 * @param pre_x- current position's row
 * @param pre_y- current position's column
 * @param new_x- new position's row
 * @param new_y- new position's column
 *
 * @return
 * 1- if the move is valid
 * 0- otherwise
 */
int isValidMoveRook(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y);

/*
 * Check if the bishop in place <pre_x, pre_y> can be moved to <new_x, new_y>.
 * The function assumes both positions are legal, <pre_x, pre_y> contains a bishop,
 * <pre_x, pre_y>!=<new_x, new_y>, <new_x, new_y> doesn't contain a piece of the same color as the
 * given bishop, and that game!=NULL
 *
 * @param game- the source game
 * @param pre_x- current position's row
 * @param pre_y- current position's column
 * @param new_x- new position's row
 * @param new_y- new position's column
 *
 * @return
 * 1- if the move is valid
 * 0- otherwise
 */
int isValidMoveBishop(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y);

/*
 * Check if the queen in place <pre_x, pre_y> can be moved to <new_x, new_y>.
 * The function assumes both positions are legal, <pre_x, pre_y> contains a queen,
 * <pre_x, pre_y>!=<new_x, new_y>, <new_x, new_y> doesn't contain a piece of the same color as the
 * given queen, and that game!=NULL
 *
 * @param game- the source game
 * @param pre_x- current position's row
 * @param pre_y- current position's column
 * @param new_x- new position's row
 * @param new_y- new position's column
 *
 * @return
 * 1- if the move is valid
 * 0- otherwise
 */
int isValidMoveQueen(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y);

/*
 * Check if the king in place <pre_x, pre_y> can be moved to <new_x, new_y>.
 * The function assumes both positions are legal, <pre_x, pre_y> contains a king,
 * <pre_x, pre_y>!=<new_x, new_y>, <new_x, new_y> doesn't contain a piece of the same color as the
 * given king, and that game!=NULL
 *
 * The function checks only if the move follow the rules of king's movements and not if the
 * king is in a threatened position.
 *
 * @param pre_x- current position's row
 * @param pre_y- current position's column
 * @param new_x- new position's row
 * @param new_y- new position's column
 *
 * @return
 * 1- if the move is valid
 * 0- otherwise
 */
int isValidMoveKing(char pre_x, char pre_y, char new_x, char new_y);

/*
 * Check if the rook in place <pre_x, pre_y> can be moved to <new_x, new_y>.
 * The function assumes both positions are legal, <pre_x, pre_y> contains a knight,
 * <pre_x, pre_y>!=<new_x, new_y>, <new_x, new_y> doesn't contain a piece of the same color as the
 * given knight, and that game!=NULL
 *
 * @param pre_x- current position's row
 * @param pre_y- current position's column
 * @param new_x- new position's row
 * @param new_y- new position's column
 *
 * @return
 * 1- if the move is valid
 * 0- otherwise
 */
int isValidMoveKnight(char pre_x, char pre_y, char new_x, char new_y);

/*
 * Check if the piece in place <x,y> is threatened.
 * @return
 * 0- if the piece isn't threatened.
 * 1- if the piece is threatened.
 */
int isThreatend(ChessGame* game, char x, char y);


/*
 * Checks if the king of current player is threatened. Used in set move, undo, and load game.
 */
int checkIsKingThreatened(ChessGame* game, int currentPlayer);

/*
 * Check if the move <pre_x, pre_y> to <new_x, new_x> is a legal move for the specific piece in <pre_x, pre_y>
 * according to the rules of the game.
 * Assumptions: both positions are legal, <pre_x, pre_y>!=<new_x, new_y>,
 * <pre_x, pre_y> and <new_x, new_y> don't contain pieces of the same player, and that game!=NULL
 *
 * getting valid different positions that contains game pieces of different colors
 */
int isValidPieceMove(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y);

/*
 * Print the board of the given game, assuming game!=NULL
 */
void printBoardChessGame(ChessGame* game);

/*
 * Print the available move for the piece in position <x,y> on the game's board.
 * If the move is threatened by an opponent- "*" is printed next to the move.
 * If the move captures a piece- "^" is printed next to the move.
 *
 * @return
 * 1- <x,y> is an invalid position on the board.
 * 2- position <x,y> doesn't contain a player piece (the position is empty).
 * GAME_GETMOVES_MEMORY_FAILURE- memory allocation error occurred.
 * 0- otherwise.
 */
int getMoves(ChessGame* game, char x, char y);


/*
 * Undo the previous move if possible. If consoleModeFlag==1, print a message describing the move to undo.
 * After the move is undone, update the isWhiteKingThreatened and isBlackKingThreatened parameters.
 *
 * @return
 * 1- if there are no moves to undo.
 * 0- otherwise.
 */
int undoMoveChessGame(ChessGame* game, int consoleModeFlag);

/*
 * Checks if there's a winner:
 * returns:
 * 0 - black wins
 * 1 - white wins
 * 2 - no winner nor draw
 * 3 - draw
 * GAME_MEMORY_FAILURE- memory allocation failure occurred.
 */
int checkWinnerOrDrawChessGame(ChessGame* game);



#endif /* CHESSGAME_H_ */
