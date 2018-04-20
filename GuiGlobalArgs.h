/*
 * GuiGlobalArgs.h
 *
 *  Created on: Mar 14, 2018
 *      Author: tomhe
 */

#ifndef GUIGLOBALARGS_H_
#define GUIGLOBALARGS_H_

#include "Widget.h"

typedef enum {
	GUI_MAIN_WINDOW,
	GUI_SETTINGS_WINDOW,
	GUI_GAME_WINDOW,
	GUI_SAVE_GAME,
	GUI_LOAD_GAME,
	GUI_QUIT,
	GUI_STAY,
} CHESS_GUI_WINDOW;

extern ChessGame* game;

extern Widget *piecesPosArr[BOARD_LENGTH][BOARD_LENGTH];
extern Widget* pieces[4*BOARD_LENGTH];

extern bool isGameSaved;

#endif /* GUIGLOBALARGS_H_ */
