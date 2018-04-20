/*
 * ChessMinimax.c
 *
 *  Created on: Feb 27, 2018
 *      Author: krimolovsky
 */

#include "ChessMinimax.h"

int scoringFunctionMinimax(ChessGame* game, int player) {
	int result=0;
	int check_winner_result=2;
	check_winner_result=checkWinnerOrDrawChessGame(game);
	//player is the winner
	if (player==check_winner_result) {
		return 1000;
	}
	//opposite player is the winner
	if (player==1-check_winner_result) {
		return -1000;
	}
	//there's a draw
	if (check_winner_result==3) {
		return 0;
	}
	//no special events, need to calculate by pieces on board
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			if (checkPieceType(game,i+'1', j+'A')==player) { //player's piece
				result+=scoringFunctionGetPieceScore(game->gameBoard[i][j]);
			}
			else if (checkPieceType(game,i+'1', j+'A')==1-player) { //opponent's piece
				result-=scoringFunctionGetPieceScore(game->gameBoard[i][j]);
			}
		}
	}
	return result;
}

int scoringFunctionGetPieceScore(char piece) {
	if (piece=='m' || piece=='M') {
		return 1;
	}
	if (piece=='n' || piece=='N') {
		return 3;
	}
	if (piece=='b' || piece=='B') {
		return 3;
	}
	if (piece=='r' || piece=='R') {
		return 5;
	}
	if (piece=='q' || piece=='Q') {
		return 9;
	}
	if (piece=='k' || piece=='K') {
		return 100;
	}
	if (piece=='_') {
		return 0;
	}
	return 0;
}

int alphabetaChessMinimax(ChessGame* game, int depth, int alpha, int beta, int maximizingPlayer, int player) {
	int score=0;
	int tmp_score=0;
	ChessGame* game_copy=NULL;

	//case of terminal node due to depth
	if (depth==0) {
		score=scoringFunctionMinimax(game, player);
		return score;
	}

	//case of terminal node due to winner/draw
	if (checkWinnerOrDrawChessGame(game)!=2) { //there's a winner (0 for black, 1 for white) or draw(3). 2 means not a terminal case.
		score=scoringFunctionMinimax(game, player); //MAYBE CHANGE DUE TO COMPLEXITY
		return score;
	}

	score=maximizingPlayer ? minimax_MIN : minimax_MAX; //setting initial value before checking possible moves
	//moving across all pieces of current player
	for (int j=0; j<BOARD_LENGTH; j++) {
		for (int i=0; i<BOARD_LENGTH; i++) {
			if(checkPieceType(game, i+'1', j+'A')==game->currentPlayer){
				//now checking all possible moves for the piece we found
				for(int l=0; l<BOARD_LENGTH;l++){
					for(int k=0; k<BOARD_LENGTH; k++){
						if (!isValidMoveChessGame(game, i+'1',j+'A', k+'1',l+'A')) { //meaning there is a valid move
							//creating new copy & making a move
							game_copy=copyChessGame(game);
							if (game_copy==NULL) {
								return minimax_ERROR; //sign of failure!
							}
							setMoveChessGame(game_copy, i+'1',j+'A', k+'1',l+'A');

							//now "alphabeta" pruning
							if (maximizingPlayer) {
								tmp_score=alphabetaChessMinimax(game_copy, depth-1, alpha, beta, false, player);
								if (tmp_score==minimax_ERROR) { //memory failure in a further call to function
									destoryChessGame(game_copy);
									return minimax_ERROR;
								}

								score=tmp_score>score ? tmp_score : score; //score=max(score,tmp_score)
								alpha=score>alpha ? score : alpha; //alpha=max(alpha,score);
							}
							else {
								tmp_score=alphabetaChessMinimax(game_copy, depth-1, alpha, beta, true, player);
								if (tmp_score==minimax_ERROR) { //memory failure in a further call to function
									destoryChessGame(game_copy);
									return minimax_ERROR;
								}

								score=tmp_score<score ? tmp_score : score; //score=min(score,tmp_score)
								beta=score<beta? score: beta; //beta=min(beta,score)
							}
							destoryChessGame(game_copy);
							if(beta<=alpha){
								return score; //no need to check further moves - tree cutting!
							}
						}
					}
				}
			}
		}
	}
	//in case there were no cuts
	return score;
}

int getBestMoveChessMinimax(ChessGame* game, char* details) {
	/* no terminal cases are checked here: (this function is called only once per move)
	 * game->difficulty>0 otherwise it's not a legal game settings
	 * state of draw or winning was already checked before calling this function.
	 */

	int alpha=minimax_MIN;
	int beta=minimax_MAX;
	int tmp_score=0;
	int maximizingPlayer=0;
	ChessGame* game_copy=NULL;
	char details_to_return[4]={'\0'};

	for (int j=0; j<BOARD_LENGTH; j++) {
		for (int i=0; i<BOARD_LENGTH; i++) {
			if(checkPieceType(game, i+'1', j+'A')==game->currentPlayer){
				//now checking all possible moves for the piece we found
				for(int l=0; l<BOARD_LENGTH;l++){
					for(int k=0; k<BOARD_LENGTH; k++){
						if (!isValidMoveChessGame(game, i+'1',j+'A', k+'1',l+'A')) { //meaning there is a valid move
							//creating new copy & making a move
							game_copy=copyChessGame(game);
							if (game_copy==NULL) {
								return 0;
							}
							setMoveChessGame(game_copy, i+'1',j+'A', k+'1',l+'A');
							tmp_score=alphabetaChessMinimax(game_copy, game->difficulty-1, alpha, beta, maximizingPlayer, game->currentPlayer);
							destoryChessGame(game_copy);
							if (tmp_score==minimax_ERROR) { //a failure occurred during minimax
								return 0;
							}

							if (alpha<tmp_score) {
								alpha=tmp_score;
								details_to_return[0]=i+'1'; //from row
								details_to_return[1]=j+'A'; //from column
								details_to_return[2]=k+'1'; //to row
								details_to_return[3]=l+'A'; //to column
							}
						}
					}
				}
			}
		}
	}
	strcpy(details, details_to_return);
	if(details_to_return==NULL){
		return 0;
	}
	return 1;
}
