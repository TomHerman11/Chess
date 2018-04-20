/*
 * highlightBoard.h
 *
 *  Created on: Mar 18, 2018
 *      Author: tomhe
 */

#ifndef HIGHLIGHTBOARD_H_
#define HIGHLIGHTBOARD_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "ChessGame.h"
#include "Button.h"
#include "GuiGlobalArgs.h"
#include "ChessMainAux.h"
#include "GuiMainAux.h"

#define BOARD_LENGTH 8
#define BOARD_X 30
#define BOARD_Y 64
#define SQUARE_LENGTH 63


typedef struct highlight_t Highlight;
struct highlight_t {
	SDL_Rect rect;
	int red;
	int green;
	int blue;
	int alpha;
};

/*
 * the function receives a Highlight array of size 64 (same as number of squares in chess board.
 * returns:
 * each highlight has a basic color white and alpha=100.
 * updating the colors is made through calling to highlightGetMoves.
 *
 * ERRORS:
 * if the is not created due to memory allocation failure, we will notify the user, and continue the game.
 * same if one of the highlights is not created.
 */
void createArrayofHighlights (SDL_Renderer* rend, Highlight* array_of_highlights[BOARD_LENGTH][BOARD_LENGTH]);

/*
 * destroy the array of highlights.
 */
void destoryArrayofHightlights(Highlight* highlight[BOARD_LENGTH][BOARD_LENGTH]);

/*
 * ASSUMPTION - get moves will be called only from a square that contains a piece.
 * each square is colored in it's matching color. a square with not meaning will have a transparent color.
 * returns 0 for error. o/w 1
 */
int highlightGetMoves(ChessGame* game, char x, char y, Highlight* highlight[BOARD_LENGTH][BOARD_LENGTH]);

/*
 * used to draw the highlights on the board.
 */
void drawHighlightArray(SDL_Renderer* rend, Highlight* highlight[BOARD_LENGTH][BOARD_LENGTH]);

/*
 * once the highlighting is not needed, we need the make the squares invisible~
 */
void makeHighlightInvisible(Highlight* highlight[BOARD_LENGTH][BOARD_LENGTH]);

#endif /* HIGHLIGHTBOARD_H_ */
