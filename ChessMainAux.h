/*
 * ChessMainAux.h
 *
 *  Created on: Feb 28, 2018
 *      Author: krimolovsky
 */

#ifndef CHESSMAINAUX_H_
#define CHESSMAINAUX_H_

#include "ChessMinimax.h"
#include "ChessParser.h"

#define LINELENGTHLOADFILE 64

/*
 * this function is called upon entering the settings state in the game. the function awaits for the settings values from the user.
 * default settings are the initial values.
 * each command is executed by the matching function.
 */
ChessGame* ChessMainSettings(ChessGame* game);

/*
 * to chage the game mode. if wrong value is entered, an error print is made.
 */
void changeGameModeAux(ChessGame* game, int mode);

/*
 * to chage the game difficulty. if wrong value is entered, an error print is made.
 */
void changeGameDifficultyAux(ChessGame* game, int difficulty);

/*
 * to chage the user color. if wrong value is entered, an error print is made.
 */
void changeGameUserColorAux(ChessGame* game, int user_color);

/*
 * to save the game in the path entered. if a file error is occurred, a matching error is printed.
 * if there's a file error, no special handling is made except for printing.
 * in addition, we ASSUME that the writing will be successful and no write errors will occur. (no instructions were given on that subject).
 */
//void saveGameAux(ChessGame* game, char* path);

int saveGameAux(ChessGame* game, char* path);

/*
 * game is not null, checked before called to the function.
 * same as save game, with exception - is destroying the current game and creating a new game.
 * after the creation, the values in the file is uploaded to the game board and settings.
 * calledFromGui - if an memory or file failure occurred, int* loadSuccess is used to inform the calling function!
 *
 */
ChessGame* loadGameAux(ChessGame* game, char* path, int calledFromGui, int* loadSuccess);

/*
 * prints the current settings of the game.
 */
void printSettingsAux(ChessGame* game);

/*
 * this function destroys the game and exits the programm.
 * suitable only for console mode.
 *
 * need the change this function once we get to GUI
 */
void quitGameAuxCONSOLE(ChessGame* game);

/*
 * this function awaits for command from the player while playing the game.
 * each command is handled with its matching function.
 * only on SET MOVE or RESET the current player is changed and function ends.
 */
ChessGame* inGamePlayerCommandsAux(ChessGame* game);

/*
 * sets the move on the game or prints a matching error if the values are illegal.
 */
int setMoveAux(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y, int* need_to_print_board);

/*
 * prints all moves available for the piece at <x,y>.
 * if no piece at <x,y> a matching error is printed.
 */
void getMovesAux(ChessGame* game, char x, char y);

/*
 * undo at most 2 moves at the game. returns the number of undo-s that were made.
 * if no undo-s are available, a matching error is printed.
 * o/w the first undo will be executed.
 * then tries to execute the second undo if available.
 */
int undoMoveAux(ChessGame* game);

/*
 * saves the current game settings, destroying it and building a new one with the same settings as before.
 * after that, the function calls "ChessMainSettings" to enter the settings mode once again.
 */
ChessGame* resetGameAux(ChessGame* game);

/*
 * calls "getBestMoveChessMinimax" to calculate the best move for the computer based on the difficulty given.
 * in case of memory failure in "getBestMoveChessMinimax", the function return error value.
 */
int computerMoveChess(ChessGame *game);

/*
 * calls checkWinnerOrDrawChessGame to check if the game is over, and prints a matching message.
 * if the game is not in a terminal state, the funciton does nothing.
 */
int isGameOverAux(ChessGame *game);

/*
 * the function checks if after a move the opposite king is threatened by the last move.
 * if does, a matching message is printed.
 */
void checkThreatenedAux(ChessGame* game);




#endif /* CHESSMAINAUX_H_ */
