/*
 * GuiSettingsWindow.c
 *
 *  Created on: Mar 11, 2018
 *      Author: tomhe
 */

#include "GuiSettingsWindow.h"

bool game_1_player=true; //if false then game mode is 2-Player, need not to present the difficulty and user color
bool flag_exit_presentWindow_loop=false;

/*
 * each of the following function changes one of the game settings.
 * also, each function changes the position of the relevant arrow pointing to the last chosen setting
 */

void changeArrowDiffucltyX(void* arg, int difficulty) { //difficulty=1,...,5
	SDL_Rect* rect_arrow=&(((Button*)(((Widget*)arg)->data))->location);
	rect_arrow->x=SETTINGS_RECT_X_BUTTONS+(difficulty-1)*(DIFFICULTYBUTTONLENGTH+DIFFICULTYBUTTONLENGTH);
}

void clickDifficuly1(void* arg) {
	game->difficulty=1;

	//change the position of the arrow to the difficulty 1 button
	changeArrowDiffucltyX(arg, 1);
}

void clickDifficuly2(void* arg) {
	game->difficulty=2;

	//change the position of the arrow to the difficulty 2 button
	changeArrowDiffucltyX(arg, 2);
}

void clickDifficuly3(void* arg) {
	game->difficulty=3;

	//change the position of the arrow to the difficulty 3 button
	changeArrowDiffucltyX(arg, 3);
}

void clickDifficuly4(void* arg) {
	game->difficulty=4;

	//change the position of the arrow to the difficulty 4 button
	changeArrowDiffucltyX(arg, 4);
}

void clickDifficuly5(void* arg) {
	game->difficulty=5;

	//change the position of the arrow to the difficulty 5 button
	changeArrowDiffucltyX(arg, 5);
}

/*
 * change the arrow pointing to the selected option of game mode.
 */
void changeArrowGameModeX(void* arg, int option_number) { //option_number=1 or option_number=2
	SDL_Rect* rect_arrow=&(((Button*)(((Widget*)arg)->data))->location);
	rect_arrow->x=SETTINGS_RECT_X_BUTTONS+(option_number-1)*(GAMEMODEOPTIONWIDTH+SPACEGAMEMODEOPTIONS)+
			(SPACEGAMEMODEOPTIONS-LENGTH_ARROW_CHOSEN_SETTING);
}

void clickGameMode1(void* arg) {
	game->game_mode=1;

	//change the position of the arrow to the first option (1-player)
	changeArrowGameModeX(arg, 1);

	//important for the present window
	game_1_player=true;
}

/*
 * changes the game_mode value in game.
 * also the function changes the position of the arrow in the settings window pointing to the current game_mode
 */
void clickGameMode2(void * arg) {
	game->game_mode=2;

	//change the position of the arrow to the second option (2-player)
	changeArrowGameModeX(arg, 2);

	//important for the present window
	game_1_player=false;
}

/*
 * the functions that change user color changr an argument passed to them, 1 means white 0 means 0.
 * the arg value is used in the presentWindow function
 */
void clickUserColorWhite(void* arg) {
	game->user_color=1;

	//change the flag representing the seleceted color
	*((int*)arg)=1;
}

void clickUserColorBlack(void* arg) {
	game->user_color=0;

	//change the flag representing the seleceted color
	*((int*)arg)=0;
}

void clickBackToMainMenu(void* myENUM) {
	*(CHESS_GUI_WINDOW*)myENUM=GUI_MAIN_WINDOW;
	flag_exit_presentWindow_loop=true;
}

void clickStartGame(void* myENUM) {
	*(CHESS_GUI_WINDOW*)myENUM=GUI_GAME_WINDOW;
	flag_exit_presentWindow_loop=true;

}

/*
 * updates an exisiting array of the game_mode buttons:
 * index 0 - title (no functions)
 * index 1 - 1-player button
 * index 2 - 2-player button
 * index 3 - arrow pointing to the chosen game_mode
 */
int createGameModeButtons(SDL_Renderer* rend, Widget* gameModeButtons[]) {
	int is_create_successed=1;

	SDL_Rect rect_game_mode_title = { .x = SETTINGS_RECT_X_BUTTONS, .y = 20+30, .w = 400, .h = 100 };

	SDL_Rect rect_1_player = { .x = SETTINGS_RECT_X_BUTTONS+SPACEGAMEMODEOPTIONS,
			.y = 120+30, .w = GAMEMODEOPTIONWIDTH, .h = GAMEMODEOPTIONHEIGHT };

	SDL_Rect rect_2_player = { .x = SETTINGS_RECT_X_BUTTONS+GAMEMODEOPTIONWIDTH+2*SPACEGAMEMODEOPTIONS,
			.y = 120+30, .w = GAMEMODEOPTIONWIDTH, .h = GAMEMODEOPTIONHEIGHT };

	SDL_Rect rect_arrow_game_mode = { .x = SETTINGS_RECT_X_BUTTONS,
			.y = 120+30, .w = LENGTH_ARROW_CHOSEN_SETTING, .h = LENGTH_ARROW_CHOSEN_SETTING };

	gameModeButtons[0]=createButton(rend, "chessPictures/settingsGameModeTitle.bmp",
			rect_game_mode_title, NULL, NULL, 1, true, 109, 84, 50);
	gameModeButtons[1]=createButton(rend, "chessPictures/settings1Player.bmp",
			rect_1_player, clickGameMode1, NULL, 1, false, 0, 0, 0);
	gameModeButtons[2]=createButton(rend, "chessPictures/settings2Player.bmp",
			rect_2_player, clickGameMode2, NULL, 1, false, 0, 0, 0);
	gameModeButtons[3]=createButton(rend, "chessPictures/settingsArrow.bmp",
			rect_arrow_game_mode, NULL, NULL, 1, true, 0, 0, 0);

	for (int i=0; i<4; i++) {
		if (gameModeButtons[i]==NULL) {
			is_create_successed=0;
		}
	}
	return is_create_successed;
}

int createDifficultyButtons(SDL_Renderer* rend, Widget* gameDifficultyButtons[]) {
	int is_create_successed=1;

	SDL_Rect rect_difficulty_title = { .x = SETTINGS_RECT_X_BUTTONS, .y = 190+25, .w = 400, .h = 100 };

	SDL_Rect rect_difficulty1 = { .x = SETTINGS_RECT_X_BUTTONS+SPACEDIFFICULTYBUTTONS+
			0*(DIFFICULTYBUTTONLENGTH+DIFFICULTYBUTTONLENGTH),
			.y = 290+25, .w = DIFFICULTYBUTTONLENGTH, .h = DIFFICULTYBUTTONLENGTH };

	SDL_Rect rect_difficulty2 = { .x = SETTINGS_RECT_X_BUTTONS+SPACEDIFFICULTYBUTTONS+
			1*(DIFFICULTYBUTTONLENGTH+DIFFICULTYBUTTONLENGTH),
			.y = 290+25, .w = DIFFICULTYBUTTONLENGTH, .h = DIFFICULTYBUTTONLENGTH };

	SDL_Rect rect_difficulty3 = { .x = SETTINGS_RECT_X_BUTTONS+SPACEDIFFICULTYBUTTONS+
			2*(DIFFICULTYBUTTONLENGTH+DIFFICULTYBUTTONLENGTH),
			.y = 290+25, .w = DIFFICULTYBUTTONLENGTH, .h = DIFFICULTYBUTTONLENGTH };

	SDL_Rect rect_difficulty4 = { .x = SETTINGS_RECT_X_BUTTONS+SPACEDIFFICULTYBUTTONS+
			3*(DIFFICULTYBUTTONLENGTH+DIFFICULTYBUTTONLENGTH),
			.y = 290+25, .w = DIFFICULTYBUTTONLENGTH, .h = DIFFICULTYBUTTONLENGTH };

	SDL_Rect rect_difficulty5 = { .x = SETTINGS_RECT_X_BUTTONS+SPACEDIFFICULTYBUTTONS+
			4*(DIFFICULTYBUTTONLENGTH+DIFFICULTYBUTTONLENGTH),
			.y = 290+25, .w = DIFFICULTYBUTTONLENGTH, .h = DIFFICULTYBUTTONLENGTH };

	SDL_Rect rect_arrow_difficulty = { .x = SETTINGS_RECT_X_BUTTONS,
			.y = 290+25, .w = LENGTH_ARROW_CHOSEN_SETTING, .h = LENGTH_ARROW_CHOSEN_SETTING };


	gameDifficultyButtons[0]=createButton(rend, "chessPictures/settingsDifficultyTitle.bmp",
			rect_difficulty_title, NULL, NULL, 1, true, 109, 84, 50);

	gameDifficultyButtons[1]=createButton(rend, "chessPictures/settingsDifficulty1.bmp",
			rect_difficulty1, clickDifficuly1, NULL, 1, false, 0, 0, 0);

	gameDifficultyButtons[2]=createButton(rend, "chessPictures/settingsDifficulty2.bmp",
			rect_difficulty2, clickDifficuly2, NULL, 1, false, 0, 0, 0);

	gameDifficultyButtons[3]=createButton(rend, "chessPictures/settingsDifficulty3.bmp",
			rect_difficulty3, clickDifficuly3, NULL, 1, false, 0, 0, 0);

	gameDifficultyButtons[4]=createButton(rend, "chessPictures/settingsDifficulty4.bmp",
			rect_difficulty4, clickDifficuly4, NULL, 1, false, 0, 0, 0);

	gameDifficultyButtons[5]=createButton(rend, "chessPictures/settingsDifficulty5.bmp",
			rect_difficulty5, clickDifficuly5, NULL, 1, false, 0, 0, 0);

	gameDifficultyButtons[6]=createButton(rend, "chessPictures/settingsArrow.bmp", //the only arrow exists
			rect_arrow_difficulty, NULL, NULL, 1, true, 0, 0, 0);

	for (int i=0; i<7; i++) {
		if (gameDifficultyButtons[i]==NULL) {
			is_create_successed=0;
		}
	}
	return is_create_successed;
}

int createUserColorButtons(SDL_Renderer* rend, Widget* gameUserColorButtons[]) {
	int is_create_successed=1;

	SDL_Rect rect_user_color_title = { .x = SETTINGS_RECT_X_BUTTONS, .y = 360+10, .w = 400, .h = 90 };

	SDL_Rect rect_white_player = { .x = SETTINGS_RECT_X_BUTTONS+260, .y = 360+20,
			.w = USERCOLORBUTTONLENGTH, .h = USERCOLORBUTTONLENGTH };

	SDL_Rect rect_black_player = { .x = SETTINGS_RECT_X_BUTTONS+260,
			.y = 360+20, .w = USERCOLORBUTTONLENGTH, .h = USERCOLORBUTTONLENGTH };

	gameUserColorButtons[0]=createButton(rend, "chessPictures/settingsUserColorTitle.bmp",
			rect_user_color_title, NULL, NULL, 1, true, 109, 84, 50);

	//by our design - click on white king the color and widget will change to black and other way
	//that's why the functions are opposite
	gameUserColorButtons[1]=createButton(rend, "chessPictures/settingsUserColorWhite.bmp",
			rect_white_player, clickUserColorBlack, NULL, 1, true, 148, 227, 254);
	gameUserColorButtons[2]=createButton(rend, "chessPictures/settingsUserColorBlack.bmp",
			rect_black_player, clickUserColorWhite, NULL, 1, true, 148, 227, 254);

	for (int i=0; i<3; i++) {
		if (gameUserColorButtons[i]==NULL) {
			is_create_successed=0;
		}
	}
	return is_create_successed;
}

int createSettingsBackStartBG(SDL_Renderer* rend, Widget* start_back_BG[]) {
	int is_create_successed=1;

	SDL_Rect rect_window_background = { .x = 0, .y = 0, .w = 800, .h = 600 };

	SDL_Rect rect_BG = { .x = SETTINGS_RECT_X_BUTTONS, .y = 50, .w = 400, .h = 500};

	SDL_Rect rect_start = { .x = SETTINGS_RECT_X_BUTTONS+150+80, .y = 470, .w = 150, .h = 70};

	SDL_Rect rect_back = { .x = SETTINGS_RECT_X_BUTTONS+10, .y = 470, .w = 150, .h = 70};

	start_back_BG[0]=createButton(rend, "chessPictures/mainBackground.bmp",
			rect_window_background, NULL, NULL, 1, false, 0, 0, 0);

	start_back_BG[1]=createButton(rend, "chessPictures/settingsBackground.bmp",
			rect_BG, NULL, NULL, 1, false, 0, 0, 0);

	start_back_BG[2]=createButton(rend, "chessPictures/settingsStart.bmp",
			rect_start, clickStartGame, NULL, 1, true, 148, 227, 254);

	start_back_BG[3]=createButton(rend, "chessPictures/settingsBackArrow.bmp",
			rect_back, clickBackToMainMenu, NULL, 1, true, 0, 0, 0);

	for (int i=0; i<4; i++) {
		if (start_back_BG[i]==NULL) {
			is_create_successed=0;
		}
	}
	return is_create_successed;
}

void setInitialArrowsSettings(Widget* game_mode_arrow, Widget* difficulty_arrow) {
	//set for game_mode
	if (game->game_mode==1) clickGameMode1(game_mode_arrow);
	else if (game->game_mode==2) clickGameMode2(game_mode_arrow);

	//set for diffuclty
	if (game->difficulty==1) clickDifficuly1(difficulty_arrow);
	else if (game->difficulty==2) clickDifficuly2(difficulty_arrow);
	else if (game->difficulty==3) clickDifficuly3(difficulty_arrow);
	else if (game->difficulty==4) clickDifficuly4(difficulty_arrow);
	else if (game->difficulty==5) clickDifficuly5(difficulty_arrow);

}

/*
 * used to handle all choices that have been selected by the user. presents the selceted options.
 * also choosing to start the game or to go back to the MAIN WINDOW.
 */
CHESS_GUI_WINDOW presentWindowSettings(	SDL_Renderer* rend, Widget* start_back_BG[], Widget* gameModeButtons[],
										Widget* gameDifficultyButtons[], Widget* gameUserColorButtons[])
{
	CHESS_GUI_WINDOW result=GUI_MAIN_WINDOW;
	int game_user_color=game->user_color; //1 for white, 0 for black, changed through buttons on matching widgets
	SDL_Event e;
	while (!flag_exit_presentWindow_loop) {
		while (SDL_PollEvent(&e)) {
			start_back_BG[2]->handleEvent(start_back_BG[2], &e, &result); // start button
			start_back_BG[3]->handleEvent(start_back_BG[3], &e, &result); // back button

			//handle event for game mode buttons, gameModeButtons[3] is the widget of the arrow
			gameModeButtons[1]->handleEvent(gameModeButtons[1], &e, gameModeButtons[3]); // 1-player button
			gameModeButtons[2]->handleEvent(gameModeButtons[2], &e, gameModeButtons[3]); //2-player button

			if (game_1_player) { //need to handle user color and difficulty
				for (int i=1; i<6; i++) {
				gameDifficultyButtons[i]->handleEvent(gameDifficultyButtons[i], &e, gameDifficultyButtons[6]);
				//gameDifficultyButtons[6] is the arrow that need to be moved
				}
				if (game_user_color==1) {
					gameUserColorButtons[1]->handleEvent(gameUserColorButtons[1], &e, &game_user_color); //user color white
				}
				else { //user color is 0, means black
					gameUserColorButtons[2]->handleEvent(gameUserColorButtons[2], &e, &game_user_color); //user color black
				}
			}

			switch (e.type) {
				case SDL_QUIT:
					flag_exit_presentWindow_loop=true;
					result=GUI_QUIT;
					break;
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						flag_exit_presentWindow_loop=true;
						result=GUI_QUIT;
					}
					break;
			}
		}

	// clear window to color red (r,g,b,a)
		SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);
		SDL_RenderClear(rend);

		// draw our shiny buttons
		for (int i=0; i<4; i++) {
			start_back_BG[i]->draw(start_back_BG[i], rend);
		}
		for (int i=0; i<4; i++) {
			gameModeButtons[i]->draw(gameModeButtons[i], rend);
		}

		//draw difficulty and user color is needed
		if (game_1_player) {
			//draw difficulties
			for (int i=0; i<7; i++) {
				gameDifficultyButtons[i]->draw(gameDifficultyButtons[i], rend);
				//gameDifficultyButtons[6] is the arrow that need to be moved
			}
			//draw user color title
			gameUserColorButtons[0]->draw(gameUserColorButtons[0], rend);

			//draw matching piece matching the user color
			if (game_user_color==1) {
				gameUserColorButtons[1]->draw(gameUserColorButtons[1], rend);
			}
			else { //user color is 0, means black
				gameUserColorButtons[2]->draw(gameUserColorButtons[2], rend);
			}
		}

		// present changes to user
		SDL_RenderPresent(rend);

		// small delay
		SDL_Delay(10);
	}

	flag_exit_presentWindow_loop=false;
	return result;
}

CHESS_GUI_WINDOW settingsWindow(SDL_Renderer* rend) {
	int create_startbackBG_buttons=1;
	int create_gamemode_buttons=1;
	int create_difficulty_buttons=1;
	int create_usercolor_buttons=1;
	CHESS_GUI_WINDOW window_result=GUI_MAIN_WINDOW;
	char* error;

	/*ChessGame* new_game;
	new_game=createChessGame();
	if (new_game==NULL) {
		//DO SOMETHING
	}
	destoryChessGame(game);
	game=new_game;
	isGameSaved=true;//CHECK IF NEEDED
	*/

	//set game settings to default
	setChessGameSettingsDefault(game);

	//create background, and start and back buttons
	Widget* start_back_BG[4];
	create_startbackBG_buttons=createSettingsBackStartBG(rend, start_back_BG);

	//create game mode buttons
	Widget* gameModeButtons[4];
	create_gamemode_buttons=createGameModeButtons(rend, gameModeButtons);

	//create difficulty buttons
	Widget* gameDifficultyButtons[7];
	create_difficulty_buttons=createDifficultyButtons(rend, gameDifficultyButtons);

	//create user_color buttons - one for title, two for king in color white and black
	Widget* gameUserColorButtons[3];
	create_usercolor_buttons=createUserColorButtons(rend, gameUserColorButtons);

	//if there is an error in creating the buttons:
	if (!(create_startbackBG_buttons && create_gamemode_buttons &&
			create_difficulty_buttons && create_usercolor_buttons)) //if one the creations failed (=0). we return to main window
	{
		error="Error: error in creating settings buttons, cannot open settings window.\n";
		errorMessageBox(error);
		printf("%s" ,error);
	}
	else { //everything is fine, we may continue

		//set the arrows point the current game settings
		//these are the pointers to the arrows widgets
		setInitialArrowsSettings(gameModeButtons[3], gameDifficultyButtons[6]);

		//preset the window and wait for setting decisions
		window_result=presentWindowSettings(rend, start_back_BG, gameModeButtons, gameDifficultyButtons, gameUserColorButtons);
	}

	//destory the widgets that been created
	for (int i=0; i<4; i++) {
		destroyWidget(start_back_BG[i]);
	}
	for (int i=0; i<4; i++) {
		destroyWidget(gameModeButtons[i]);
	}
	for (int i=0; i<7; i++) {
		destroyWidget(gameDifficultyButtons[i]);
	}
	for (int i=0; i<3; i++) {
		destroyWidget(gameUserColorButtons[i]);
	}

	return window_result;
}
