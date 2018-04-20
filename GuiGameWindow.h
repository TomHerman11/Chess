/*
 * GuiGameWindow.h
 *
 *  Created on: Mar 12, 2018
 *      Author: tomhe
 */

#ifndef GUIGAMEWINDOW_H_
#define GUIGAMEWINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include <math.h>

#include "ChessGame.h"
#include "Button.h"
#include "GuiMainAux.h"
#include "PieceWidget.h"
#include "ChessMinimax.h"
#include "highlightBoard.h"

/*
 * restarts the game, and sets the default parameters to the Widgets on board.
 * the game changes isGameSaved to true, since the game is restarted.
 */
int restartGuiGame(SDL_Renderer* rend);


/*
 * ERRORS: if one of the "vital" buttons,the game, or the minimax encounters a problem, WE RETURN TO THE MAIN WINDOW.
 * the "vital" buttons:
 * - main window button
 * - quit button
 * - the board
 */
CHESS_GUI_WINDOW GameWindow(SDL_Window* main_window);


#endif /* GUIGAMEWINDOW_H_ */
