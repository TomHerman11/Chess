/*
 * main.c
 *
 *  Created on: Feb 25, 2018
 *      Author: krimolovsky
 */

#include "ConsoleMain.h"

int consoleMain() {

	ChessGame* game;
	int error_check=0;
	printf(" Chess\n-------\n");
	game=createChessGame();
	game=ChessMainSettings(game);
	if(game==NULL){
		printf("Error: Could not create a new game. Exiting...\n");
		return 0;
	}
	while(1){
		if(game->game_mode==1 && game->currentPlayer!=game->user_color){//if computer's turn
			error_check=computerMoveChess(game);
			if (error_check==0) {
				printf("Error: Could not calculate computer's move. Exiting...\n");
				destoryChessGame(game);
				return 0;
			}
		}
		else{
			game=inGamePlayerCommandsAux(game);
		}
		checkThreatenedAux(game);
		if(isGameOverAux(game)){
			destoryChessGame(game);
			return 0;
		}
	}
	return 0;
}
