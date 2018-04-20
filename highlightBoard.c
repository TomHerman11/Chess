/*
 * highlightBoard.c
 *
 *  Created on: Mar 18, 2018
 *      Author: tomhe
 */

#include "highlightBoard.h"

Highlight* createHighlight (SDL_Renderer* rend, SDL_Rect rect, int red, int green, int blue, int alpha) {
	Highlight* highlight=calloc(1, sizeof(Highlight));
	if (highlight==NULL) {
		return NULL;
	}
	highlight->rect=rect;
	highlight->red=red;
	highlight->green=green;
	highlight->blue=blue;
	highlight->alpha=alpha;

	//give the rectangle it's color
	SDL_SetRenderDrawColor(rend, red, green, blue, alpha);
	SDL_RenderFillRect(rend, &rect);

	return highlight;
}

void destoryHightlight(Highlight* highlight) {
	if (highlight!=NULL) {
		free(highlight);
	}
}

void updateHighlightColor(Highlight* highlight, int red, int green, int blue, int alpha) {
	if (highlight!=NULL) {
		highlight->red=red;
		highlight->green=green;
		highlight->blue=blue;
		highlight->alpha=alpha;
	}
}

void createArrayofHighlights (SDL_Renderer* rend, Highlight* array_of_highlights[BOARD_LENGTH][BOARD_LENGTH]) {
	//FIRST INDEX - ROW
	//SECOND INDEX - COLUMN
	int is_create_successed=1;
	char* error;

	for (int i=BOARD_LENGTH-1; i>=0; i--) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			SDL_Rect rect = { .x = BOARD_X+j*SQUARE_LENGTH , .y = BOARD_Y+(7-i)*SQUARE_LENGTH,.w = SQUARE_LENGTH, .h = SQUARE_LENGTH };
			array_of_highlights[i][j]=createHighlight(rend, rect, 255, 255, 255, 100);
			if (array_of_highlights[i][j]==NULL) {
				is_create_successed=0;
			}
		}
	}
	if (!is_create_successed) { //meaning some of the highlights are null!
		error="Error: error during initialization of Get Moves array. Some of the available moves will not be presented.\n";
		errorMessageBox(error);
		printf("%s" ,error);
	}
}

void destoryArrayofHightlights(Highlight* highlight[BOARD_LENGTH][BOARD_LENGTH]) {
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			destoryHightlight(highlight[i][j]); //checked if null in destoryHightlight
		}
	}
}

void drawHighlightArray(SDL_Renderer* rend, Highlight* highlight[BOARD_LENGTH][BOARD_LENGTH]) {
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			Highlight* myHighlight=highlight[i][j];
			if (myHighlight!=NULL) {
				SDL_SetRenderDrawColor(rend, myHighlight->red, myHighlight->green, myHighlight->blue, myHighlight->alpha);
				SDL_RenderFillRect(rend, &(myHighlight->rect));
			}
		}
	}
}

int highlightGetMoves(ChessGame* game, char x, char y, Highlight* highlight[BOARD_LENGTH][BOARD_LENGTH]) {
	char* error;
	int piece_type=0;
	ChessGame *game_copy=NULL;
	ChessGame *game_copy2=NULL;
	bool isThreatened=false;
	bool isCapturing=false;

	piece_type=checkPieceType(game, x, y); //get piece type, important to know which square is good

	game_copy=copyChessGame(game);
	if(game_copy==NULL){
		error="Error: error in calculating moves, please try again\n";
		errorMessageBox(error);
		printf("%s" ,error);
		return 0;

	}
	(game_copy->currentPlayer)=piece_type; //player in game_copy has the same color as the piece

	for(int i=0; i<BOARD_LENGTH; i++){
		for(int j=0; j<BOARD_LENGTH;j++){
			isThreatened=false;
			isCapturing=false;
			if (!isValidMoveChessGame(game_copy, x,y, i+'1',j+'A')){
				//this square is valid! (isValid returns error number and not boolean value..., 0 means OK)

				//checking if the piece is threatened
				game_copy2=copyChessGame(game_copy);
				if (game_copy2==NULL) {
					error="Error: error in calculating moves, please try again\n";
					makeHighlightInvisible(highlight);
					errorMessageBox(error);
					printf("%s" ,error);
					return 0;
				}
				setMoveChessGame(game_copy2, x,y, i+'1',j+'A');
				if(isThreatend(game_copy2, i+'1',j+'A')){
					isThreatened=true;
				}
				destoryChessGame(game_copy2);

				//checking if the piece captures a rival's piece in the new position
				//position can't hold a piece of the same color- checked in isValidChessGame
				if(game_copy->gameBoard[i][j]!=EMPTY_ENTRY) {
					isCapturing=true;
				}

				//now coloring the square!
				if (isThreatened && isCapturing) { //change color to yellow, since it's threatened and capturing!
					updateHighlightColor(highlight[i][j], 255, 255, 0, 100);
					continue;
				}

				if (isThreatened && !isCapturing) { //change color to red, since it's only threatened!
					updateHighlightColor(highlight[i][j], 255, 0, 0, 100);
					continue;
				}
				if (!isThreatened && isCapturing) { //change color to green, since it's only capturing!
					updateHighlightColor(highlight[i][j], 0, 200, 0, 100);
					continue;
				}
				if (!isThreatened && !isCapturing) { //this square is just available for a move and not more than that. set color to blue
					updateHighlightColor(highlight[i][j], 0, 0, 255, 100);
					continue;
				}
			}

			//this square is not available, change alpha to 0 (not visible)
			updateHighlightColor(highlight[i][j], 255, 255, 255, 0);

			//for next iteration
			isThreatened=false;
			isCapturing=false;
		}
	}
	destoryChessGame(game_copy);
	return 1; //all was well.
}

void makeHighlightInvisible(Highlight* highlight[BOARD_LENGTH][BOARD_LENGTH]) {
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			if (highlight[i][j]!=NULL) {
				updateHighlightColor(highlight[i][j], 255, 255, 255, 0);
			}
		}
	}
}
