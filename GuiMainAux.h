/*
* GuiMainAux.h
 *
 *  Created on: Mar 11, 2018
 *      Author: tomhe
 */

#ifndef GUIMAINAUX_H_
#define GUIMAINAUX_H_

#include <SDL.h>
#include <SDL_video.h>
#include "PieceWidget.h"


SDL_Window* createWindow(const char* window_name, int width, int height);

void changeWindowName(SDL_Window* window, const char* new_window_name);

SDL_Renderer* createRenderer(SDL_Window* window);

/*
 * presents a messageBox for the user, before exiting the game to main window or quit.
 * the options are yes, no, cancel.
 * cancel (or BACK in save window) returns to the game window, and the game can continue as nothing happened.
 */

int messageBoxBeforeExitingGame();

/*
 * we use this function whenever there's a error.
 * only OK option will be available.
 */
void errorMessageBox(char* error);

void setDefaultPiecesPositions();

int createWidgets(SDL_Renderer* rend);

void destroyPieces();

#endif /* GUIMAINAUX_H_ */
