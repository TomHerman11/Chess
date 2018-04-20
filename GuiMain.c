/*
 * GuiMain.c
 *
 *  Created on: Mar 11, 2018
 *      Author: tomhe
 */

#include "GuiMain.h"

//global variables from GuiGlobalArgs
ChessGame* game;
Widget *piecesPosArr[BOARD_LENGTH][BOARD_LENGTH];
Widget* pieces[4*BOARD_LENGTH];
bool isGameSaved;

int GuiMain() {
	char* error;

	//extern Widget* piecesPosArr[BOARD_LENGTH][BOARD_LENGTH];

	// initialize SDL2 for video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	// create an SDL window
	SDL_Window* main_window = createWindow("Chessprog" ,800, 600);
	if (main_window==NULL) {
		error="Error: unable to create window. Exiting...\n";
		errorMessageBox(error);
		printf("%s", error);

		SDL_Quit();
		return 1;
	}

	// create a renderer for the window
	SDL_Renderer* rend = createRenderer(main_window);
	if (rend==NULL) {
		error="Error: unable to create window. Exiting...\n";
		errorMessageBox(error);
		printf("%s", error);

		SDL_DestroyWindow(main_window);
		SDL_Quit();
		return 1;
	}

	//creating the game
	game=createChessGame();
	if (game==NULL) {
		error="Error: unable to create new game. Exiting...\n";
		errorMessageBox(error);
		printf("%s", error);

		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(main_window);
		SDL_Quit();
		return 1;
	}
	//used for showing messageBox "do you want to save the game...."
	isGameSaved=true;

	//creating piece widgets
	if (!createWidgets(rend)) { //error in creating widgets of board pieces
		destroyPieces();
		error="Error: unable to create game pieces. Exiting...\n";
		errorMessageBox(error);
		printf("%s", error);

		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(main_window);
		SDL_Quit();
		return 1;
	}

	setDefaultPiecesPositions();

	//running the game
	runGuiMain(main_window);

	destoryChessGame(game);
	destroyPieces();
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(main_window);
	SDL_Quit();
	return 0;
}

void runGuiMain(SDL_Window* main_window) {
	SDL_Renderer* rend=SDL_GetRenderer(main_window);
	CHESS_GUI_WINDOW current_window=GUI_MAIN_WINDOW;
	CHESS_GUI_WINDOW pre_window=GUI_MAIN_WINDOW; //used if back was clicked in load window, from main OR(!) game
	int messageBoxResult=0;

	while (1) {
		if (current_window==GUI_MAIN_WINDOW) {
			pre_window=current_window;
			current_window=initMainWindow(rend);
		}

		if (current_window==GUI_SETTINGS_WINDOW) {
			current_window=settingsWindow(rend);
		}

		if (current_window==GUI_GAME_WINDOW) {
			pre_window=current_window;
			current_window=GameWindow(main_window);

			//show messagebox, if user wants to save the game, in case the game is not saved
			if (isGameSaved==false && (current_window==GUI_MAIN_WINDOW || current_window==GUI_QUIT)) {
				SDL_HideWindow(main_window);
				messageBoxResult=messageBoxBeforeExitingGame();
				if(messageBoxResult==1) { //"yes" was chosen
					saveWindow();

				}
				//messageBoxResult==2 -> "cancel" was chosen
				//other possibility - "yes" was chosen but back button was pressed in the save window
				if (messageBoxResult==2 ||(messageBoxResult==1 && isGameSaved==false)) {
					current_window=GUI_GAME_WINDOW;
					SDL_ShowWindow(main_window);
					continue;
				}
			}
			if(current_window==GUI_MAIN_WINDOW){
				if(!restartGuiGame(rend)){	//restart function sets isGameSaved to true;
					errorMessageBox("Error: failure returning to main menu- returning to current game. You may try again.");
					printf("Error: failure returning to main menu- returning to current game. You may try again.");
					current_window=GUI_GAME_WINDOW;
					SDL_ShowWindow(main_window);
					continue;

				}
				changeWindowName(main_window, "Chessprog");
				SDL_ShowWindow(main_window);
			}
		}

		if (current_window==GUI_SAVE_GAME) {
			SDL_HideWindow(main_window);
			current_window=saveWindow();
			SDL_ShowWindow(main_window);

			if (isGameSaved) { //a save was made
				//change the main window name, since the game was saved
				changeWindowName(main_window, "Chessprog");
			}

		}
		if (current_window==GUI_LOAD_GAME) {
			SDL_HideWindow(main_window);
			current_window=loadWindow(pre_window);
			SDL_ShowWindow(main_window);

			if (isGameSaved) { //a load was made - the game is now "saved".
				//change the main window name, since the game was saved
				changeWindowName(main_window, "Chessprog");
			}

		}
		if (current_window==GUI_QUIT) {
			break;
		}
	}
}

