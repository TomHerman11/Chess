/*
 * ChessMainWindow.c
 *
 *  Created on: Mar 7, 2018
 *      Author: tomherman
 */

#include "GuiMainWindow.h"

void clickQuitButton(void* arg) {
	*(CHESS_GUI_WINDOW*)arg=GUI_QUIT;
}

void clickNewGameButton(void* arg) {
	*(CHESS_GUI_WINDOW*)arg=GUI_SETTINGS_WINDOW;
}

void clickLoadGameButton(void* arg) {
	*(CHESS_GUI_WINDOW*)arg=GUI_LOAD_GAME;
}

/*
 * if there's a problem in making any of the buttons, an error is printed and chessprog will be terminated (not from this function!)
 */
int createMainWindowUtilButtons(SDL_Renderer* rend, Widget* utilButtons[]) {
	int is_create_successed=1;
	char* error;
	SDL_Rect rect_background = { .x = 0, .y = 0, .w = 800, .h = 600 };
	SDL_Rect rect_new_game = { .x = 200, .y = 150, .w = 400, .h = 100 };
	SDL_Rect rect_load_game = { .x = 200, .y = 300, .w = 400, .h = 100 };
	SDL_Rect rect_quit = { .x = 200, .y = 450, .w = 400, .h = 100 };

	utilButtons[0]=createButton(rend, "chessPictures/mainBackground.bmp",
			rect_background, NULL, NULL, 1, false, 0, 0, 0);

	utilButtons[1]=createButton(rend, "chessPictures/mainNewGame.bmp",
			rect_new_game, clickNewGameButton, NULL, 1, false, 0, 0, 0);

	utilButtons[2]=createButton(rend, "chessPictures/mainLoadGame.bmp",
				rect_load_game, clickLoadGameButton, NULL, 1, false, 0, 0, 0);

	utilButtons[3]=createButton(rend, "chessPictures/mainQuit.bmp",
			rect_quit,clickQuitButton, NULL, 1, false, 0, 0, 0);

	if (utilButtons[1]==NULL || utilButtons[3]==NULL) { //if newgame or quit buttons are null, we will no continue!
		error="Error: error in creating critical buttons in Main Window. Exiting...\n";
		errorMessageBox(error);
		printf("%s" ,error);
		is_create_successed=0;

	}
	return is_create_successed;
}

CHESS_GUI_WINDOW initMainWindow(SDL_Renderer* rend) {
	int success_in_creating_buttons=1;
	int done = 0;

	CHESS_GUI_WINDOW result=GUI_STAY;
	Widget* utilButtons[4];
	success_in_creating_buttons=createMainWindowUtilButtons(rend, utilButtons);

	if (success_in_creating_buttons==0) { //not able to perform while in main window
		done=1;
		result=GUI_QUIT;
	}
	// event handling loop

	SDL_Event e;
	while (!done) {
		while (SDL_PollEvent(&e)) {
			for (int i=1; i<4; i++) { //new game, load and quit buttons
				if (utilButtons[i]!=NULL) {
					utilButtons[i]->handleEvent(utilButtons[i], &e, &result);
				}
			}

			switch (e.type) {
				case SDL_QUIT:
					done = 1;
					result=GUI_QUIT;
					break;
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						done = 1;
						result=GUI_QUIT;
					}
					break;
			}
		}

		if (result==GUI_LOAD_GAME|| result==GUI_SETTINGS_WINDOW || result==GUI_QUIT) {
			break;
		}

		// clear window to color red (r,g,b,a)
		SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);
		SDL_RenderClear(rend);

		// draw our shiny buttons
		for (int i=0; i<4; i++) {
			if (utilButtons[i]!=NULL) {
				utilButtons[i]->draw(utilButtons[i], rend);
			}
		}

		// present changes to user
		SDL_RenderPresent(rend);

		// small delay
		SDL_Delay(10);
	}

	// free everything and finish
	for (int i=0; i<4; i++) {
		destroyWidget(utilButtons[i]);
	}

	return result;
}
