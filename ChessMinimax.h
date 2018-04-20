/*
 * ChessMinimax.h
 *
 *  Created on: Feb 27, 2018
 *      Author: krimolovsky
 */

#ifndef CHESSMINIMAX_H_
#define CHESSMINIMAX_H_

#include "ChessGame.h"
#include <limits.h>
#include <stdbool.h>

#define minimax_MIN -1001
#define minimax_MAX 1001
#define minimax_ERROR INT_MIN


/*
 * this function computes the score of the board, by the instructions given in class, based on player.
 * for player's win situation (checkmate): 1000.
 * player's lose: -1000.
 * draw - 0.
 * not finite state - each piece with it's own value (+ for the player's and - for the opponent).
 *
 */
int scoringFunctionMinimax(ChessGame* game, int player);

/*
 * this function is called from "scoringFunctionMinimax", specifies to each piece it's value in points.
 */
int scoringFunctionGetPieceScore(char piece);

/*
 * this function uses PRUNING to evaluate the alpha and beta values to each node in the MINIMAX TREE.
 * finite states - if depth is 0 or the the board is a checkmate or draw.
 * any other state - the function calculate the minimax values according to the algorithm.
 * player is used when calling "scoringFunctionMinimax".
 * the function can have memory allocation problems, and it's dealt in the function.
 */
int alphabetaChessMinimax(ChessGame* game, int depth, int alpha, int beta, int maximizingPlayer, int player);

/*
 * this function is the first step in the PRUNING calculation.
 * the separation is made since we want to "remember" the best move, which is chosen only from the first row in the MINIMAX TREE.
 * the function can have memory allocation problems, and it's dealt in the function.
 * the move is stored in details.
 * details is an argument in the function to prevent usage of memory allocation.
 * return:
 * 0- if memory allocation error occurred.
 * 1- otherwise.
 */
int getBestMoveChessMinimax(ChessGame* game, char* details);











#endif /* CHESSMINIMAX_H_ */
