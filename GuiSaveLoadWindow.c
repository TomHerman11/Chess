/*
 * GuiSaveLoadWindow.c
 *
 *  Created on: Mar 14, 2018
 *      Author: tomhe
 */

#include "GuiSaveLoadWindow.h"

//global variables in this source file
bool flag_exit_save_window=false; //used in presentWindow function,
int currentSaveFiles;
bool clickBackSaveLoad=false;

//functions:
void addPieceToGame(int player, char piece_symbol, char r, char c);
void loadPiecesArray();

/*
 * the function loads a game, based on the path_suffix from arg.
 * if the game loading success, a new game will be updated and also the piece widgets.
 * if not, the game conitnues, as well as the positions of the gui pieces.
 */
void clickSaveButtontoLoad(void* arg) {
	int loadSuccess=1;
	char file_path[50]={'\0'};
	int current_save=*(int*)arg;
	sprintf(file_path, "chessSaves/ChessSave%d", current_save);

	//now loading the game
	game=loadGameAux(game, file_path, 1, &loadSuccess); //will print errors if needed
	//loadGameAux(game, path);
	//printBoardChessGame(game);
	if (loadSuccess) {
		loadPiecesArray();
		//printBoardChessGame(game);
		flag_exit_save_window=true;
		isGameSaved=true;
	}
}

/*
 * used in the function "clickSaveButtontoLoad", for loading a game. hides all pieces across the window. *
 */
void hideAllPieces(){
	PieceWidget* piece;
	for(int i=0; i<4;i++){
		for(int j=0; j<8; j++){
			piece=pieces[i*8+j]->data;
			piece->show=0;
		}
	}
}

/*
 * used in the function "clickSaveButtontoLoad", for loading a game.
 */
void cleanPiecesPosArray(){
	for(int i=0; i<BOARD_LENGTH; i++){
		for(int j=0; j<BOARD_LENGTH; j++){
			piecesPosArr[i][j]=NULL;
		}
	}
}

/*
 * used in the function "clickSaveButtontoLoad", for loading a game.
 */
void loadPiecesArray(){
	int player;
	hideAllPieces();
	cleanPiecesPosArray();
	for(int i=0; i<BOARD_LENGTH; i++){
		for(int j=0; j<BOARD_LENGTH; j++){
			player=checkPieceType(game, i+'1',j+'A');
			if(player!=2){
				addPieceToGame(player, game->gameBoard[i][j], i+'1', j+'A');

			}
		}
	}
}

/*
 * used in the function "clickSaveButtontoLoad", for loading a game.
 * adjusts the gui pieces according to the loaded game.
 */
void addPieceToGame(int player, char piece_symbol, char r, char c){
	int i, j;
	PieceWidget *piece;
	int loc[2];
	getGuiPositionFromIndex(r, c, loc);

	if(player!=0 && player!=1){
		return;
	}
	if (piece_symbol=='m'|| piece_symbol=='M'){
		i= piece_symbol=='m' ? 1:2;
		for(int k=0; k<8; k++){
			piece= pieces[i*8+k]->data;
		//	printf("index: r= %c, c= %c\n", piece->r_index, piece->c_index);
			if(!piece->show){
				piecesPosArr[r-'1'][c-'A']=pieces[i*8+k];
				break;
			}
		}
	}
	else{
		i=player==1? 0:3;
		if (piece_symbol=='q' || piece_symbol=='Q') {
			piece= pieces[i*8+3]->data;
			piecesPosArr[r-'1'][c-'A']=pieces[i*8+3];
		}
		else if(piece_symbol=='k' || piece_symbol=='K'){
			piece= pieces[i*8+4]->data;
			piecesPosArr[r-'1'][c-'A']=pieces[i*8+4];
		}
		else{
			if (piece_symbol=='r' || piece_symbol=='R') {
				j=0;
			}
			else if (piece_symbol=='n' || piece_symbol=='N') {
				j=1;
			}
			else if (piece_symbol=='b' || piece_symbol=='B') {
				j=2;
			}
			piece= pieces[i*8+j]->data;
			if(!piece->show){
				piecesPosArr[r-'1'][c-'A']=pieces[i*8+j];
			}
			else{
				piece= pieces[i*8+7-j]->data;
				if(!piece->show){
					piecesPosArr[r-'1'][c-'A']=pieces[i*8+7-j];
				}
			}
		}
	}
	piece->r_index=r;
	piece->c_index=c;
	piece->location.x=loc[0];
	piece->location.y=loc[1];
	piece->show=1;
}

/*
 * used is a button of a save slot is clicked. saves to the path according to suffix in arg.
 */
void clickSaveButtontoSave(void* arg) {
	char file_path[50]={'\0'};
	int current_save=*(int*)arg;
	sprintf(file_path, "chessSaves/ChessSave%d", current_save);

	//now saving the game
	saveGameAux(game, file_path);
	flag_exit_save_window=true;
	isGameSaved=true;
}

/*
 * used to exit the save load window.
 */
void clickBackButtonSaveOrLoad(void* arg) {
	if (arg) {}
	flag_exit_save_window=true;
	clickBackSaveLoad=true;
}

/*
 * the function changes all y values of the rectangles of the game slots, if any exist.
 */
void scrollUpSaveLoadWindow(void* arg) {
	if (currentSaveFiles==0) { //nothing to scroll
		return;
	}

	Widget** saveFilesButtons=(Widget**)arg;
	Button* button_first = (Button*) (*saveFilesButtons)->data; //button of first widget
	if (button_first->location.y==SPACEBETWEENSAVESLOTS) { //no further scroll down is available, otherwise it will scroll down too much...
		return;
	}
	//if we reached this point, we want to update the y values of all rectangles of the saveslots!
	for (int i=0; i<currentSaveFiles; i++) {
		((Button*)(*(saveFilesButtons+i))->data)->location.y+=SPACEBETWEENSAVESLOTS;
	}
}

/*
 * the function changes all y values of the rectangles of the game slots, if any exist.
 */
void scrollDownSaveLoadWindow(void* arg) {
	if (currentSaveFiles==0) { //nothing to scroll
			return;
	}

	Widget** saveFilesButtons=(Widget**)arg;
	Button* button_last = (Button*) (*(saveFilesButtons+currentSaveFiles-1))->data; //button of last widget
	if (button_last->location.y<400-(SPACEBETWEENSAVESLOTS+SAVESLOTSIZE)) { //no further scroll up is available, otherwise it will scroll up too much...
		return;
	}
	//if we reached this point, we want to update the y values of all rectangles of the saveslots!
	for (int i=0; i<currentSaveFiles; i++) {
		((Button*)(*(saveFilesButtons+i))->data)->location.y-=SPACEBETWEENSAVESLOTS;
	}
}

/*
 * tries to access all saves available, by that updating which buttons for loading a game will be available.
 * updates the global variable "currentSaveFiles" to know with how many widgets we will have to deal.
 * if an error occurred during making a button, a error will be printed. all other functions will still continue a usual!!
 */
void updateArrayofWidgetLoad(SDL_Renderer* rend ,Widget* saveFilesButtons[]) {
	currentSaveFiles=0;
	char current_file_path[50]={'\0'};
	char current_image_path[50]={'\0'};
	int path_suffix_int=1;

	int rectangleY=SPACEBETWEENSAVESLOTS;

	for (int i=0; i<MAXSAVEFILES; i++) {
		sprintf(current_file_path, "chessSaves/ChessSave%d", path_suffix_int);
		sprintf(current_image_path, "chessPictures/loadsavePhotoSave%d.bmp", path_suffix_int);

		if(access(current_file_path, F_OK)!=-1 ) {
			// file exists!!
			SDL_Rect rect_save = { .x =400-20-SAVESLOTSIZE , .y = rectangleY, .w = SAVESLOTSIZE, .h = SAVESLOTSIZE };
			saveFilesButtons[currentSaveFiles]=createButton
					(rend, current_image_path, rect_save, clickSaveButtontoLoad, NULL, 1, false, 0, 0, 0);

			//error check
			if (saveFilesButtons[currentSaveFiles]==NULL) {
				printf("Error: unable to create save slot number %d although it exists\n", path_suffix_int);
			}
			else {
				//update button info of saveload
				((Button*)saveFilesButtons[currentSaveFiles]->data)->saveloadNumber=path_suffix_int;
				//for next iteration
				rectangleY+=(SAVESLOTSIZE+SPACEBETWEENSAVESLOTS);
				currentSaveFiles++;
			}
		}
		path_suffix_int++;
	}
}

/*
 * making all of the save slots, determined by "MAXSAVEFILES". can be changed to any number. (by uploading the matching pictures.
 * updates the global variable "currentSaveFiles" to know with how many widgets we will have to deal.
 * if an error occurred during making a button, a error will be printed. all other functions will still continue a usual!!
 */
void updateArrayofWidgetSave(SDL_Renderer* rend ,Widget* saveFilesButtons[]) {
	currentSaveFiles=0;
	int rectangleY=SPACEBETWEENSAVESLOTS;
	char image_path[50]={'\0'};
	int path_suffix_int=1;

	//creating as much buttons as possible as defined at first in "GuiGlobalArgs.h"
	for (int i=0; i<MAXSAVEFILES; i++) {
		//update the picture path:
		sprintf(image_path, "chessPictures/loadsavePhotoSave%d.bmp", path_suffix_int);

		//create the widget
		SDL_Rect rect_save = { .x =400-20-SAVESLOTSIZE , .y = rectangleY, .w = SAVESLOTSIZE, .h = SAVESLOTSIZE };
		saveFilesButtons[i]=createButton
				(rend, image_path, rect_save, clickSaveButtontoSave, NULL, 1, false, 0, 0, 0);

		//error check
		if (saveFilesButtons[i]==NULL) {
			printf("Error: unable to create save slot number %d\n", path_suffix_int);
		}
		else {
			//update the button info of the save button
			((Button*)saveFilesButtons[i]->data)->saveloadNumber=path_suffix_int;

			//update next rectangle
			rectangleY+=(SAVESLOTSIZE+SPACEBETWEENSAVESLOTS);
			currentSaveFiles++;
		}
		//update next save slot in potential
		path_suffix_int++;
	}
}

/*
 * util_buttons is a array created in loadWindow or saveWindow.
 * it has a fix length according to the number of util buttons in the loadWindow or saveWindow function.
 * the util buttons are:
 * scrollable button that will change the y value of the save slots (are contained in the array save_buttons).
 * back button to last window.
 * if (fromSaveWindow==true) then the function is called from saveWindow - for loading the right background. same for load.
 *
 * if there's a problem with the back button, the function returns 0 (by that closing the load/save window).
 * even if there's a problem with the scroll or background, we continue as usual.
 */
int createUtilButtonsSaveOrLoad(Widget* util_buttons[], SDL_Renderer* rend, bool fromSaveWindow) {
	int create_vital_buttons_success=1;

	//setting the background.
	SDL_Rect rect_background = { .x = 0 , .y = 0, .w = 400, .h = 400 };

	if (fromSaveWindow) { //upload SAVE background
	util_buttons[0]=createButton(rend, "chessPictures/loadsaveSAVEBackground.bmp", rect_background,
				NULL, NULL, 1, false, 0, 0, 0);
	}
	else { //upload LOAD background
		util_buttons[0]=createButton(rend, "chessPictures/loadsaveLOADBackground.bmp", rect_background,
						NULL, NULL, 1, false, 0, 0, 0);
	}

	//taking care of scrolling button, will be at util_buttons[1]
	SDL_Rect rect_scroll_button = { .x = 0 , .y = 0, .w = 400, .h = 400 };
		util_buttons[1]=createButton(rend, "chessPictures/loadsaveScroll.bmp",
				rect_scroll_button, scrollUpSaveLoadWindow, scrollDownSaveLoadWindow, 2, true, 255, 255, 255);

	if (util_buttons[1]==NULL) {
		printf("Error: unable to use scrolling in save or load window\n");
	}

	//taking care of back button
	SDL_Rect rect_back_button = { .x = 20 , .y = 10, .w = 80, .h = 40 };
	util_buttons[2]=createButton(rend, "chessPictures/loadsaveBack.bmp", rect_back_button,
			clickBackButtonSaveOrLoad, NULL, 1, true, 0, 0, 0);
	if (util_buttons[2]==NULL) {
		printf("Error: unable to create save or load window\n");
		create_vital_buttons_success=0;
	}
	return create_vital_buttons_success;
}

/*
 * destroy scroll button, back button and mouse wheel message
 */
void destoryUtilButtonsSaveOrLoad(Widget** util_buttons) {
	for (int i=0; i<3; i++) {
		destroyWidget(util_buttons[i]);
	}
}

/*
 * the functions waiting for user inputs (through keyboard or mouse clicks/wheel. acts accordingly.
 */
void presentWindowSaveOrLoad(SDL_Renderer* rend ,Widget* saveFilesButtons[], Widget* util_buttons[]) {
							//, CHESS_GUI_WINDOW pre_curr_next_windows[]) {
	SDL_Event e;
	while (!flag_exit_save_window) {
		while (SDL_PollEvent(&e)) {
			//handleEvent for scroll and back
			if (util_buttons[1]!=NULL) {//can handle save or load window without scrolling. not without Back!
				util_buttons[1]->handleEvent(util_buttons[1], &e, saveFilesButtons); //scroll button
			}
			util_buttons[2]->handleEvent(util_buttons[2], &e, NULL); //back button

			//handleEvent for save slots
			for (int i=0; i<currentSaveFiles; i++) {
				if (saveFilesButtons[i]!=NULL) {
					saveFilesButtons[i]->handleEvent(saveFilesButtons[i], &e, &((Button*)saveFilesButtons[i]->data)->saveloadNumber);
				}
			}


			switch (e.type) {
				case SDL_WINDOWEVENT:
					if(e.window.event== SDL_WINDOWEVENT_CLOSE){
						flag_exit_save_window = 1;
						clickBackSaveLoad=true;
					}
					break;
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						flag_exit_save_window = 1;
						clickBackSaveLoad=true;
					}
					break;
			}
		}

		// clear window to color red (r,g,b,a)
		SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);
		SDL_RenderClear(rend);

		// draw our shiny buttons
		for (int i=0; i<3; i++) {
			if (util_buttons[i]!=NULL) {
				util_buttons[i]->draw(util_buttons[i], rend);
			}
		}

		for (int i=0; i<currentSaveFiles; i++) {
			if (saveFilesButtons[i]!=NULL) {
				saveFilesButtons[i]->draw(saveFilesButtons[i], rend);
			}
		}

		// present changes to user
		SDL_RenderPresent(rend);

		// small delay
		SDL_Delay(10);
	}
	flag_exit_save_window=false;
	return;
}

CHESS_GUI_WINDOW loadWindow(CHESS_GUI_WINDOW pre_window) {
	// create a SDL window
	SDL_Window* window = createWindow("Chessprog - Load Game", 400, 400);

	// create a renderer for the window
	SDL_Renderer* rend = createRenderer(window);

	//create an array of the buttons for each slot, updating the array by using another function
	//count how many save files already exists
	Widget* saveFilesButtons[MAXSAVEFILES];
	updateArrayofWidgetLoad(rend, saveFilesButtons);

	//create background button, scroll button, back button
	Widget* util_buttons[3];
	createUtilButtonsSaveOrLoad(util_buttons, rend, false); //false since the function is called from load and not save.

	//project the renderer on the screen
	presentWindowSaveOrLoad(rend, saveFilesButtons, util_buttons);

	//function is done, need to free resources
	for (int i=0; i<currentSaveFiles; i++) {
		destroyWidget(saveFilesButtons[i]);
	}

	destoryUtilButtonsSaveOrLoad(util_buttons);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	flag_exit_save_window=false;

	if (pre_window==GUI_GAME_WINDOW) { //back or loading game will get us back to game window anyway.
		return GUI_GAME_WINDOW;
	}
	else if (pre_window==GUI_MAIN_WINDOW) {
		if (clickBackSaveLoad) {
			//for next call to load function.
			clickBackSaveLoad=false;

			//get back to pre window
			return pre_window;
		}
		else { //loading a game
			return GUI_GAME_WINDOW;
		}
	}
	return pre_window;
}

CHESS_GUI_WINDOW saveWindow() {
	char* error;
	int create_utils_success=1;

	// create an SDL window
	SDL_Window* window = createWindow("Chessprog - Save Game", 400, 400);
	if (window==NULL) {
		error="Error: unable to create save window\n";
		errorMessageBox(error);
		printf("%s", error);

		return GUI_GAME_WINDOW;
	}

	// create a renderer for the window
	SDL_Renderer* rend = createRenderer(window);
	if (rend==NULL) {
		error="Error: unable to create save window\n";
		errorMessageBox(error);
		printf("%s", error);

		SDL_DestroyWindow(window);
		return GUI_GAME_WINDOW;
	}

	//create an array of the buttons for each slot, updating the array by using another function
	Widget* saveFilesButtons[MAXSAVEFILES];
	updateArrayofWidgetSave(rend, saveFilesButtons); //the function also prints errors if needed

	//create background button, scroll button, back button
	Widget* util_buttons[3];
	//we set true in the next function since the function is called from save and not load.
	create_utils_success=createUtilButtonsSaveOrLoad(util_buttons, rend, true); //the function also prints errors if needed

	if (create_utils_success) {
		//project the renderer on the screen
		presentWindowSaveOrLoad(rend, saveFilesButtons, util_buttons);
	}

	//function is done, need to free resources
	for (int i=0; i<currentSaveFiles; i++) {
		destroyWidget(saveFilesButtons[i]);
	}

	destoryUtilButtonsSaveOrLoad(util_buttons);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	flag_exit_save_window=false;
	clickBackSaveLoad=false;
	return GUI_GAME_WINDOW;
}
