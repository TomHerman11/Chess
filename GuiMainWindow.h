/*
 * ChessMainWindow.h
 *
 *  Created on: Mar 7, 2018
 *      Author: tomherman
 */

#ifndef CHESSMAINWINDOW_H_
#define CHESSMAINWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>

#include "ChessGame.h"
#include "Button.h"
#include "GuiMainAux.h"

/*
 * showing the main window to the user. with available options:
 * new game -> goes the the SETTINGS WINDOW
 * load game -> loads a new game a starts the game with the settings a board as in the saved game.
 * quit -> quits the game.
 *
 * if an error occurred in making any of the buttons, chessprog terminates since its a key window.
 */
CHESS_GUI_WINDOW initMainWindow(SDL_Renderer* rend);

#endif /* CHESSMAINWINDOW_H_ */
