/*
 * PieceWidget.c
 *
 *  Created on: Mar 12, 2018
 *      Author: tomhe
 */

#include "PieceWidget.h"

extern ChessGame* game;
extern Widget* piecesPosArr[BOARD_LENGTH][BOARD_LENGTH];

Widget* createPieceWidget(
	SDL_Renderer* renderer,
	const char* image,
	SDL_Rect location,
	void (*action)(void*),
	char piece_type,
	char r_index,
	char c_index,
	bool is_transparent,
	int red,
	int green,
	int blue)
{
	// allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	if (res == NULL)
		return NULL;

	PieceWidget* data = (PieceWidget*) malloc(sizeof(PieceWidget));
	if (data == NULL) {
		free(res);
		return NULL;
	}

	// we use the surface as a temp var
	SDL_Surface* surface = SDL_LoadBMP(image);
	if (surface == NULL) {
		free(res);
		free(data);
		return NULL;
	}

	// set a specific color (white: 255,255,255) to transparent
	if (is_transparent) {
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, red, green, blue));
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(surface);
		return NULL ;
	}

	// surface is not needed after texture is created
	SDL_FreeSurface(surface);

	// store button & widget details
	data->texture = texture;
	data->location = location;
	data->action = action;
	data->pieceType=piece_type;
	data->r_index= r_index;
	data->c_index= c_index;
	data->show= 1;
	data->show_above=0;
	data->move=0;
	data->xDiff=0;
	data->yDiff=0;
	res->destroy = destroyPiece;
	res->draw = drawPiece;
	res->handleEvent = handlePieceEvent;
	res->data = data;

	return res;
}

void destroyPiece(Widget* src)
{
	PieceWidget* piece = (PieceWidget*) src->data;
	SDL_DestroyTexture(piece->texture);
	free(piece);
	free(src);
}

void handlePieceEvent(Widget* src, SDL_Event* e, void* arg) {
	//clicking on one of the pieces will change the flag:
	//flags_rightclick[0]=rightclick on one of the pieces
	//flags_rightclick[1,2] - indexes of right click
	int* flags_rightclick=(int*)arg;
	int is_valid_value;


	char board_pos[2];
	int loc[2];
	PieceWidget* piece = (PieceWidget*) src->data;
	PieceWidget* tmp_piece;

	switch (e->type) {
		case SDL_MOUSEBUTTONDOWN:
			if ((e->button.x > piece->location.x) && (e->button.y > piece->location.y) &&
				(e->button.x < piece->location.x+piece->location.w) && (e->button.y < piece->location.y+piece->location.h)) {
				if((e->button).button==SDL_BUTTON_LEFT){
					piece->xDiff = e->button.x - piece->location.x;
					piece->yDiff = e->button.y - piece->location.y;
					piece->move = 1;
					piece->show_above=1;

					//stop showing available moves once a dragging been started
					flags_rightclick[0]=0;
				}
				else if((e->button).button==SDL_BUTTON_RIGHT){
					//right click have been made
					flags_rightclick[0]=1;

					//get indexes on the board.
					getPositionOnBoard(piece->location.x, piece->location.y, board_pos);
					flags_rightclick[1]=board_pos[0];
					flags_rightclick[2]=board_pos[1];
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if((e->button).button==SDL_BUTTON_LEFT && (piece->move == 1)){
				getPositionOnBoard(piece->location.x, piece->location.y, board_pos);
				is_valid_value= isValidMoveChessGame(game, piece->r_index, piece->c_index, board_pos[0], board_pos[1]);
				if(is_valid_value==GAME_MEMORY_FAILURE){
					printf("Error: memory allocation failure. Please try again to move piece\n");
				}
				if(is_valid_value==0){// if the move is legal
					setMoveChessGame(game, piece->r_index, piece->c_index, board_pos[0], board_pos[1]);
					if(piecesPosArr[board_pos[0]-'1'][board_pos[1]-'A']!=NULL){
						tmp_piece=piecesPosArr[board_pos[0]-'1'][board_pos[1]-'A']->data;
						tmp_piece->show=0;
						piecesPosArr[board_pos[0]-'1'][board_pos[1]-'A']=NULL;
					}
					piecesPosArr[piece->r_index-'1'][piece->c_index-'A']=NULL;
					piece->r_index=board_pos[0];
					piece->c_index=board_pos[1];
					piecesPosArr[board_pos[0]-'1'][board_pos[1]-'A']=src;

					//a move on the board has been set.
					isGameSaved=false;
				}
				getGuiPositionFromIndex(piece->r_index, piece->c_index, loc);
				piece->location.x=loc[0];
				piece->location.y=loc[1];
			}

			piece->move = 0;
			piece->show_above=0;
			break;
		case SDL_MOUSEMOTION:
			if (piece->move) {
				piece->location.x = e->motion.x - piece->xDiff;
				piece->location.y = e->motion.y - piece->yDiff;
			}
			break;
	}
}

void getPositionOnBoard(int x_pos, int y_pos, char* pos){
	int r_index;
	int c_index;
	c_index=(int)(0.5+((float)x_pos-BOARD_X_UP)/BOARD_SQUARE_WIDTH);
	r_index=7-(int)(0.5+((float)y_pos-BOARD_Y_UP)/BOARD_SQUARE_WIDTH);
	pos[0]='1'+r_index;
	pos[1]='A'+c_index;
}

void getGuiPositionFromIndex(char r_index, char c_index, int* pos){
	int x_loc;
	int y_loc;
	x_loc=BOARD_X_UP+ BOARD_SQUARE_WIDTH*(c_index-'A');
	y_loc=(7-(r_index-'1'))*BOARD_SQUARE_WIDTH+BOARD_Y_UP;
	pos[0]=x_loc;
	pos[1]= y_loc;
}


void drawPiece(Widget* src, SDL_Renderer* render)
{
	PieceWidget* piece = (PieceWidget*) src->data;
	SDL_RenderCopy(render, piece->texture, NULL, &piece->location);
}
