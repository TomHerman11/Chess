/*
 * GuiMain.h
 *
 *  Created on: Mar 19, 2018
 *      Author: tomhe
 */

#ifndef GUIMAIN_H_
#define GUIMAIN_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "GuiMainAux.h"
#include "GuiMainWindow.h"
#include "GuiSettingsWindow.h"
#include "GuiGameWindow.h"
#include "GuiSaveLoadWindow.h"

int GuiMain();

/*
 * this function runs the game, goes between the different windows available to the user.
 * the functions ends when quit is clicked, or in cases of severe memory failure.
 */
void runGuiMain(SDL_Window* main_window);

#endif /* GUIMAIN_H_ */
