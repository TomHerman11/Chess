/*
 * GuiSettingsWindow.h
 *
 *  Created on: Mar 11, 2018
 *      Author: tomhe
 */

#ifndef GUISETTINGSWINDOW_H_
#define GUISETTINGSWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include "ChessGame.h"
#include "Button.h"
#include "GuiMainAux.h"

#define SETTINGS_RECT_X_BUTTONS 200

#define SPACEGAMEMODEOPTIONS 65
#define GAMEMODEOPTIONWIDTH 130
#define GAMEMODEOPTIONHEIGHT 40

#define DIFFICULTYBUTTONLENGTH 40
#define SPACEDIFFICULTYBUTTONS 40

#define USERCOLORBUTTONLENGTH 63

#define LENGTH_ARROW_CHOSEN_SETTING 40

/*
 * updating the current rendrer to present all of the selected game options.
 * from this window the game options can be changed.
 * if two players mode is selected, difficulty and user color are not available.
 *
 * back -> goes to the MAINWINDOW.
 * start -> start the game with the selected options, and the board is in an intial state.
 *
 * if an error occurred in creating of any button, the rendrer will be updated to the MAIN WINDOW.
 * the user can try as much as he can to open the settings game. chessprog will not be terminated.
 */
CHESS_GUI_WINDOW settingsWindow(SDL_Renderer* rend);

#endif /* GUISETTINGSWINDOW_H_ */
