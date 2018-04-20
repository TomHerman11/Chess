/*
 * GuiMainAux.c
 *
 *  Created on: Mar 11, 2018
 *      Author: tomhe
 */

#include "GuiMainAux.h"

//extern ChessGame *game;
//extern Widget piecesPosArr[BOARD_LENGTH][BOARD_LENGTH];

SDL_Window* createWindow(const char* window_name, int width, int height) {
	SDL_Window* window = SDL_CreateWindow(
		//"Chessprog",
		window_name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN);

	//make sure window was created successfully - in the calling function
	return window;
}

void changeWindowName(SDL_Window* window, const char* new_window_name) {
	SDL_SetWindowTitle(window, new_window_name);
}

SDL_Renderer* createRenderer(SDL_Window* window) {
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (rend!=NULL) { // ensure renderer supports transparency
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	}

	//make sure renderer was created successfully - in the calling function
	return rend;
}

int messageBoxBeforeExitingGame() {
    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, 0, "no" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
    };
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
        	{120, 91, 58},
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
        	{255, 255, 255},
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
        	{255, 255, 255},
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
        	{61, 39, 9},
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
        	{34, 177, 76}
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Chessprog - Save The Game?", /* .title */
        "Do you want to save the current Chess game?", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("ERROR: error displaying message box");
        return -1;
    }
    return buttonid;
}

void errorMessageBox(char* error) {
    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, 0, "OK" }, };

    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Chessprog - Error!", /* .title */
        error, /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("ERROR: error displaying message box");
    }
}


int createWidgets(SDL_Renderer* rend){
	char * const w_pics[]={"chessPictures/w rook.bmp", "chessPictures/w knight.bmp", "chessPictures/w bishop.bmp",
			"chessPictures/w queen.bmp", "chessPictures/w king.bmp", "chessPictures/w bishop.bmp",
			"chessPictures/w knight.bmp", "chessPictures/w rook.bmp"};
	char * const b_pics[]={"chessPictures/b rook.bmp", "chessPictures/b knight.bmp", "chessPictures/b bishop.bmp",
			"chessPictures/b queen.bmp", "chessPictures/b king.bmp", "chessPictures/b bishop.bmp",
			"chessPictures/b knight.bmp", "chessPictures/b rook.bmp"};
	SDL_Rect rects[4][8];

	for(int i=0;i<8;i++){
		rects[0][i] = (SDL_Rect){ .x = BOARD_X_UP+ BOARD_SQUARE_WIDTH*i ,
			.y = BOARD_Y_UP+BOARD_SQUARE_WIDTH*7, .w = BOARD_SQUARE_WIDTH, .h = BOARD_SQUARE_WIDTH };

		pieces[i]=createPieceWidget(rend, w_pics[i], rects[0][i], NULL,
				game->gameBoard[0][i],'1',i+'A', true, 148, 227, 254);

		if (pieces[i]==NULL) {
			return 0; //meaning there is an error in creating the pieces
		}
	}

	for(int i=0;i<8;i++){
		rects[3][i] = (SDL_Rect){ .x = BOARD_X_UP+ BOARD_SQUARE_WIDTH*i ,
			.y = BOARD_Y_UP+BOARD_SQUARE_WIDTH*0, .w = BOARD_SQUARE_WIDTH, .h = BOARD_SQUARE_WIDTH };

		pieces[3*8+i]=createPieceWidget(rend, b_pics[i], rects[3][i], NULL,
				game->gameBoard[7][i],'8',i+'A', true, 148, 227, 254);

		if (pieces[3*8+i]==NULL) {
			return 0; //meaning there is an error in creating the pieces
		}
	}

	for(int i=0;i<8;i++){
		rects[1][i] = (SDL_Rect){ .x = BOARD_X_UP+ BOARD_SQUARE_WIDTH*i ,
			.y = BOARD_Y_UP+BOARD_SQUARE_WIDTH*6, .w = BOARD_SQUARE_WIDTH, .h = BOARD_SQUARE_WIDTH };

		pieces[1*8+i]=createPieceWidget(rend, "chessPictures/w pawn.bmp", rects[1][i], NULL,
				game->gameBoard[1][i],'2',i+'A', true, 148, 227, 254);

		if (pieces[1*8+i]==NULL) {
			return 0; //meaning there is an error in creating the pieces
		}
	}

	for(int i=0;i<8;i++){
		rects[2][i] = (SDL_Rect){ .x = BOARD_X_UP+ BOARD_SQUARE_WIDTH*i ,
			.y = BOARD_Y_UP+BOARD_SQUARE_WIDTH*1, .w = BOARD_SQUARE_WIDTH, .h = BOARD_SQUARE_WIDTH };

		pieces[2*8+i]=createPieceWidget(rend, "chessPictures/b pawn.bmp", rects[2][i], NULL,
				game->gameBoard[6][i],'7',i+'A', true, 148, 227, 254);

		if (pieces[2*8+i]==NULL) {
			return 0; //meaning there is an error in creating the pieces
		}
	}
	return 1; //creation is a success!!
}

void destroyPieces(){
	for(int i=0; i<4;i++){
		for(int j=0; j<8; j++){
			destroyWidget(pieces[i*8+j]); //from Widget C file, checks if a widget pointer is not null
		}
	}
}

void setDefaultPiecesPositions(){
	PieceWidget *curr_piece=NULL;

	//printf("ok\n");

	for(int i=0; i<BOARD_LENGTH; i++){
		for(int j=0; j<BOARD_LENGTH; j++){
			piecesPosArr[i][j]=NULL;
		}
	}

	for(int i=0; i<4;i++){
		for(int j=0; j<8; j++){
			curr_piece=pieces[i*8+j]->data;
			piecesPosArr[curr_piece->r_index-'1'][curr_piece->c_index-'A']=pieces[i*8+j];
		}
	}
}

