/*
 * PieceWidget.h
 *
 *  Created on: Mar 12, 2018
 *      Author: tomhe
 */

#ifndef PIECEWIDGET_H_
#define PIECEWIDGET_H_
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "ChessGame.h"
#include "GuiGlobalArgs.h"


//the board has a frame of size 10 pixels
#define BOARD_X_UP 30
#define BOARD_Y_UP 64
#define BOARD_SQUARE_WIDTH 63

typedef struct piece_t PieceWidget;
struct piece_t {
	SDL_Renderer* render;
	SDL_Texture* texture;
	SDL_Rect location;
	void (*action)(void*);
	int move;
	char r_index;
	char c_index;
	char pieceType;
	bool show;
	bool show_above;
	int xDiff;
	int yDiff;
};

Widget* createPieceWidget(
		SDL_Renderer* renderer,
		const char* image,
		SDL_Rect location,
		void (*action)(void*),
		char pieceType,
		char r_index,
		char c_index,
		bool is_transparent,
		int red,
		int green,
		int blue);


/*
 * Frees all memory allocation associated with the given Widget- src.
 */
void destroyPiece(Widget* src);

/*
 * Handles event relating to a piece:
 * - Update a piece location when it's dragged across the board while the left mouse key is pressed, to
 *   reflect the movement to the user.
 * - When a piece is released in a new location (left mouse key is up)- check if the movement from the
 *   previous position to the new onr is a legal move for the piece.
 *   If not- return to the previous position.
 *   Otherwise- execute the move on the game.
 * - When a piece is right clicked- update the necessary flags in the flags_rightclick array received in arg.
 */
void handlePieceEvent(Widget* src, SDL_Event* event, void* arg);

/*
 * Draw the piece.
 */
void drawPiece(Widget*, SDL_Renderer*);

/*
 * Receive a position on the board in pixels, calculates the matching square's row and column,
 * and insert them to pos.
 */
void getPositionOnBoard(int x_pos, int y_pos, char* pos);

/*
 * Receive a row and a column of a square on the board, calculates the square's  x and y positions in pixels,
 * and insert them to pos.
 */
void getGuiPositionFromIndex(char x_pos, char y_pos, int* pos);
#endif /* PIECEWIDGET_H_ */
