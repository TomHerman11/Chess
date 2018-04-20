/*
 * GuiSaveLoadWindow.h
 *
 *  Created on: Mar 14, 2018
 *      Author: tomhe
 */

#ifndef GUISAVEWINDOW_H_
#define GUISAVEWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include <unistd.h>

#include "ChessGame.h"
#include "Button.h"
#include "GuiMainAux.h"
#include "ChessMainAux.h"
#include "PieceWidget.h"

#define MAXSAVEFILES 5
#define SAVESLOTSIZE 100
#define SPACEBETWEENSAVESLOTS 20

/*
 * used for the load window. goes through all available save slots and creats the window accordingly.
 * prints error if error occurred in making the window or the renderer or the BACK button.
 * in any other case, enabling to load a game instead of the current game.
 */
CHESS_GUI_WINDOW loadWindow(CHESS_GUI_WINDOW pre_window);

/*
 * used for the save window. enabling to save the game on any save slot from 1 to "MAXSAVEFILES".
 * prints error if error occurred in making the window or the renderer or the BACK button.
 * in any other case, enabling to save the game in any save slot that was selected.
 */
CHESS_GUI_WINDOW saveWindow();

#endif /* GUISAVEWINDOW_H_ */
