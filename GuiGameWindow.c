/*
 * GuiGameWindow.c
 *
 *  Created on: Mar 12, 2018
 *      Author: tomhe
 */

#include "GuiGameWindow.h"

/*
 * Called when Save button is clicked, and changes the value of arg to GUI_SAVE_GAME.
 */
void clickSaveGame(void* arg) {
	*(CHESS_GUI_WINDOW*)arg=GUI_SAVE_GAME;
}

/*
 * Called when Load button is clicked, and changes the value of arg to GUI_LOAD_GAME.
 */
void clickLoadGame(void* arg) {
	*(CHESS_GUI_WINDOW*)arg=GUI_LOAD_GAME;
}

/*
 * Called when Main Menu button is clicked, and changes the value of arg to GUI_MAIN_WINDOW.
 */
void clickMainMenu(void* arg) {
	*(CHESS_GUI_WINDOW*)arg=GUI_MAIN_WINDOW;
}

/*
 * Called when Quit button is clicked, and changes the value of arg to GUI_QUIT.
 */
void clickQuit(void* arg) {
	*(CHESS_GUI_WINDOW*)arg=GUI_QUIT;
}

/*
 * Called when Undo button is clicked.
 * Check if there is a move to undo, and changes the value of arg to 1 if there is.
 */
void clickUndo(void* arg) {
	if(isEmptyHistoryChess(game->history)==0){// if history is not empty
		*(int*)arg=1;
	}
}

/*
 * Called when Restart button is clicked, and changes the value of arg to 1.
 */
void clickRestart(void* arg){
	*(int*)arg=1;
}


int restartGuiGame(SDL_Renderer* rend) {
	ChessGame* new_game;
	new_game=createChessGame();
	if (new_game==NULL) {
		return 0;
	}
	new_game->difficulty=game->difficulty;
	new_game->game_mode=game->game_mode;
	new_game->user_color=game->user_color;
	destoryChessGame(game);
	game=new_game;
	isGameSaved=true;

	destroyPieces();
	createWidgets(rend);
	setDefaultPiecesPositions();
	return 1;
}
/*
 * The function receives an array of pointers to Widget, and creates the buttons, the background and the board for the game .
 * In case an error occurred during the creation of a "vital" button, the function opens an error window and returns 0.
 * Otherwise- return 1.
 * If an error occurred during the creation of a "non-vital" error, opens a window error to notify about it.
 */
int createGameOptionsButtons(SDL_Renderer* rend ,Widget* gameUtilButtons[]) {
	char* error;

	SDL_Rect rect_background = { .x = 0 , .y = 0, .w = 800 , .h = 600  };
	SDL_Rect rect_restart_game = { .x = 580 , .y = 120, .w = 200, .h = 60 };
	SDL_Rect rect_save_game = { .x = 580, .y = 200, .w = 200, .h = 60 };
	SDL_Rect rect_load_game = { .x = 580, .y = 280, .w = 200, .h = 60 };
	SDL_Rect rect_undo = { .x = 580, .y = 360, .w = 200, .h = 60 };
	SDL_Rect rect_main_menu = { .x = 580, .y = 440, .w = 200, .h = 60 };
	SDL_Rect rect_quit = { .x = 580, .y = 520, .w = 200, .h = 60 };
	SDL_Rect rect_board_game = { .x = 20 , .y = 54, .w =524 , .h =524  };

	gameUtilButtons[0]=createButton(rend, "chessPictures/mainBackground.bmp", rect_background, NULL, NULL, 1, false, 0, 0, 0);
	gameUtilButtons[1]=createButton(rend, "chessPictures/Restart.bmp", rect_restart_game, clickRestart, NULL, 1, false, 0, 0, 0);
	gameUtilButtons[2]=createButton(rend, "chessPictures/save button.bmp", rect_save_game, clickSaveGame, NULL, 1, false, 0, 0, 0);
	gameUtilButtons[3]=createButton(rend, "chessPictures/Load button.bmp", rect_load_game, clickLoadGame, NULL, 1,false, 0, 0, 0);
	gameUtilButtons[4]=createButton(rend, "chessPictures/undo button.bmp", rect_undo, clickUndo, NULL, 1,false, 0, 0, 0);
	gameUtilButtons[5]=createButton(rend, "chessPictures/main menu button.bmp", rect_main_menu, clickMainMenu, NULL, 1,false, 0, 0, 0);
	gameUtilButtons[6]=createButton(rend, "chessPictures/2nd quit button.bmp", rect_quit,clickQuit, NULL, 1,false, 0, 0, 0);
	gameUtilButtons[7]=createButton(rend, "chessPictures/board.bmp", rect_board_game,NULL, NULL, 1,false, 0, 0, 0);

	if (gameUtilButtons[5]==NULL ||
		gameUtilButtons[6]==NULL ||
		gameUtilButtons[7]==NULL  ) //returns 0 if main menu, quit, or board buttons were not created! o/w returns 1.
	{
		error="Error: error in creating 'vital' buttons in game window. Returning to main menu...\n";
		errorMessageBox(error);
		printf("%s" ,error);
		return 0;
	}
	if (gameUtilButtons[0]==NULL ||
		gameUtilButtons[1]==NULL ||
		gameUtilButtons[2]==NULL ||
		gameUtilButtons[3]==NULL ||
		gameUtilButtons[4]==NULL  )
	{
		error="Error: error in creating buttons in Game Window. You may continue playing although not all functions will be available"
				", or go back to main menu and try again.\n";
		errorMessageBox(error);
		printf("%s" ,error);
	}

	return 1;
}

/*
 * Draw the pieces on the board
 */
void drawWindowGamePieces(SDL_Renderer* rend) {
	PieceWidget *curr_piece=NULL;
	Widget *above_piece=NULL;

	//draw pieces
	for(int i=0; i<4;i++){
		for(int j=0; j<8; j++){
			curr_piece=(PieceWidget*)pieces[i*8+j]->data;
			if(curr_piece->show){ //draw only if the piece is still on the board (wasn't taken)
				if(curr_piece->show_above){ // if the piece is being moved on the board. will be drawn last
					above_piece=pieces[i*8+j];
				}
				else{
					pieces[i*8+j]->draw(pieces[i*8+j], rend);
				}
			}
		}
	}
	if(above_piece!=NULL){
		above_piece->draw(above_piece, rend);
		above_piece=NULL;
	}
}

/*
 * Handles the events of all the pieces that still participate in the game
 */
void handleEventGameWindowPieces(SDL_Event* e ,int* flags_rightclick) {
	PieceWidget *curr_piece=NULL;

	for(int i=0; i<4;i++){
		for(int j=0; j<8; j++){
			curr_piece=(PieceWidget*)pieces[i*8+j]->data;
			if(curr_piece->show){
				pieces[i*8+j]->handleEvent(pieces[i*8+j], e, flags_rightclick);
			}
		}
	}
}

/*
 * Creates the buttons that will be shown in case of a win/draw.
 * If an error occurred, shows an error message.
 */
void createGameStatusButtons(SDL_Renderer* rend ,Widget* gameStatusButtons[]){
	char* error;
	SDL_Rect rects[5];

	char * const status_pics[]={"chessPictures/youWin.bmp", "chessPictures/computerWins.bmp",
			"chessPictures/whiteWins.bmp", "chessPictures/blackWins.bmp", "chessPictures/draw.bmp"};

	for(int i=0; i<5; i++){
		rects[i] = (SDL_Rect){ .x = BOARD_X_UP , .y = BOARD_Y_UP+BOARD_SQUARE_WIDTH*2, .w = BOARD_SQUARE_WIDTH*8, .h = BOARD_SQUARE_WIDTH*4 };
		gameStatusButtons[i]= createButton(rend, status_pics[i], rects[i], NULL, NULL, 1, false, 0, 0, 0);
	}

	if (gameStatusButtons[0]==NULL ||
		gameStatusButtons[1]==NULL ||
		gameStatusButtons[2]==NULL ||
		gameStatusButtons[3]==NULL ||
		gameStatusButtons[4]==NULL  )
	{
		error="Error: error in creating buttons in Game Window. You may continue playing although not all functions will be available"
				", or go back to main menu and try again.\n";
		errorMessageBox(error);
		printf("%s" ,error);
	}

}

/*
 * Creates the buttons that warn in case of CHECK.
 * If an error occurred, shows an error message.
 */
void createCheckButtons(SDL_Renderer* rend ,Widget* gameCheckButtons[]){
	char* error;
	SDL_Rect rects[4];

	char * const check_pics[]={"chessPictures/computeristhreatened.bmp",
			"chessPictures/playeristhreatened.bmp",
			"chessPictures/whiteisthreatened.bmp",
			"chessPictures/blackisthreatened.bmp"};

	for(int i=0; i<4; i++){
		rects[i] = (SDL_Rect){ .x = 580 , .y = 10, .w = 200, .h = 100 };
		gameCheckButtons[i]= createButton(rend, check_pics[i], rects[i], NULL, NULL, 1, false, 0, 0, 0);
	}

	if (gameCheckButtons[0]==NULL ||
		gameCheckButtons[1]==NULL ||
		gameCheckButtons[2]==NULL ||
		gameCheckButtons[3]==NULL  )
	{
		error="Error: error in creating buttons in Game Window. You may continue playing although not all functions will be available"
				", or go back to main menu and try again.\n";
		errorMessageBox(error);
		printf("%s" ,error);
	}
}

/*
 * Draw the fitting check warning according to the game's state.
 */
void drawCheckButtons(SDL_Renderer* rend, Widget* gameCheckButtons[]){
	switch(game->game_mode){
	case 1: //game mode 1
		if(game->isWhiteKingThreatened){
			if(game->user_color==1){ //the player is threatened
				if (gameCheckButtons[1]!=NULL) {
					gameCheckButtons[1]->draw(gameCheckButtons[1], rend);
				}
			}
			else{ //the computer is threatened
				if (gameCheckButtons[0]!=NULL) {
					gameCheckButtons[0]->draw(gameCheckButtons[0], rend);
				}
			}
		}
		else if(game->isBlackKingThreatened){
			if(game->user_color==0){ //the player is threatened
				if (gameCheckButtons[1]!=NULL) {
					gameCheckButtons[1]->draw(gameCheckButtons[1], rend);
				}
			}
			else{ //the computer is threatened
				if (gameCheckButtons[0]!=NULL) {
					gameCheckButtons[0]->draw(gameCheckButtons[0], rend);
				}
			}
		}
		break;
	case 2: //game mode 2
		if(game->isWhiteKingThreatened){
			if (gameCheckButtons[2]!=NULL) {
				gameCheckButtons[2]->draw(gameCheckButtons[2], rend);
			}
		}
		else if(game->isBlackKingThreatened){
			if (gameCheckButtons[3]!=NULL) {
				gameCheckButtons[3]->draw(gameCheckButtons[3], rend);
			}
		}
		break;
	}
}

/*
 * Presents a message with the game result at the end of the game
 */
void presentStatus(SDL_Renderer* rend, Widget* gameStatusButtons[]){
	int check_winner_result=checkWinnerOrDrawChessGame(game);


	if(check_winner_result==3){ //a draw
		if (gameStatusButtons[4]!=NULL) {
			gameStatusButtons[4]->draw(gameStatusButtons[4], rend);
		}
		return;
	}
	if(game->game_mode==2){
		if(check_winner_result==1){ //white player wins
			if (gameStatusButtons[2]!=NULL) {
				gameStatusButtons[2]->draw(gameStatusButtons[2], rend);
			}
		}
		else if(check_winner_result==0){ //white player wins
			if (gameStatusButtons[3]!=NULL) {
				gameStatusButtons[3]->draw(gameStatusButtons[3], rend);
			}
		}
	}

	if(game->game_mode==1){
		if(check_winner_result== game->user_color){ //the player wins
			if (gameStatusButtons[0]!=NULL) {
				gameStatusButtons[0]->draw(gameStatusButtons[0], rend);
			}
		}
		else if(check_winner_result== 1-game->user_color){ //the computer wins
			if (gameStatusButtons[1]!=NULL) {
				gameStatusButtons[1]->draw(gameStatusButtons[1], rend);
			}
		}
	}
}

/*
 * Called from guiComputerMoveChess and GuiUndoMove.
 * Moves the piece in position <from_r, from_c> to <to_r, to_c> on the board.
 */
void guiPieceMove(char from_r, char from_c, char to_r, char to_c){
	int loc[2];
		PieceWidget *piece;
		piece=piecesPosArr[from_r-'1'][from_c-'A']->data;
		piece->r_index=to_r;
		piece->c_index=to_c;
		getGuiPositionFromIndex(piece->r_index, piece->c_index, loc);
		piece->location.x= loc[0];
		piece->location.y= loc[1];
		if(piecesPosArr[to_r-'1'][to_c-'A']!=NULL){
			piece=piecesPosArr[to_r-'1'][to_c-'A']->data;
			piece->show=0;
		}
		piecesPosArr[to_r-'1'][to_c-'A']=piecesPosArr[from_r-'1'][from_c-'A'];
		piecesPosArr[from_r-'1'][from_c-'A']=NULL;
}

/*
 * Called from GuiUndoMove.
 * If there a piece was returned to the game when a move was undone,
 * then the function returns the piece to the gui board.
 */
void returnDeadPieceToBoard(Widget *pieces[], char piece_symbol, char r, char c){
	int player, i, j;
		PieceWidget *piece;
		player=checkPieceType(game, r,c);
		if(player==2){ //if no piece was returned to the game
			return;
		}
		if (piece_symbol=='m'|| piece_symbol=='M'){
			i= piece_symbol=='m' ? 1:2; //search for the pawns in the the right place in the array
			for(int k=0; k<8; k++){
				piece= pieces[i*8+k]->data; // white pawns are in indexes 8-15. black pawns are in 16-23.
				if(piece->r_index==r && piece->c_index==c){
					piece->show=1;
					piecesPosArr[r-'1'][c-'A']=pieces[i*8+k];
					return;
				}
			}
		}
		i=player==1? 0:3; //search for the pawns in the the right place in the array
		//the white pieces (other than pawns) are in indexes 0-7. the black are in indexes 24-31.

		if (piece_symbol=='q' || piece_symbol=='Q') {
			piece= pieces[i*8+3]->data; //there is only one queen for the player
			piece->show=1;
			piecesPosArr[r-'1'][c-'A']=pieces[i*8+3];
			return;

		}

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
		if(piece->r_index==r && piece->c_index==c){ //check the first position of the piece
			piece->show=1;
			piecesPosArr[r-'1'][c-'A']=pieces[i*8+j];
			return;
		}
		piece= pieces[i*8+7-j]->data;
		if(piece->r_index==r && piece->c_index==c){ //check the second position of the piece
			piece->show=1;
			piecesPosArr[r-'1'][c-'A']=pieces[i*8+7-j];

		}
}

/*
 * Undo the last two moves if possible.
 * Undo the last move if there is only one move to undo.
 */
void GuiUndoMove(Widget *pieces[]) {
	char last_move[5];
	getFirstHistoryChess(game->history, last_move);
	if (undoMoveChessGame(game,0)==0) {//first undo is possible
		guiPieceMove(last_move[2], last_move[3], last_move[0], last_move[1]);
		returnDeadPieceToBoard(pieces, last_move[4], last_move[2], last_move[3]);
	}

	//trying to do the second undo
	getFirstHistoryChess(game->history, last_move);
	if (undoMoveChessGame(game,0)==0) { //second undo is possible
		guiPieceMove(last_move[2], last_move[3], last_move[0], last_move[1]);
		returnDeadPieceToBoard(pieces, last_move[4], last_move[2], last_move[3]);
	}
}

/*
 * Performs the computer's move.
 * If an error occurred- return 0. Otherwise- return 1.
 */
int guiComputerMoveChess() {
	char details[4];
	if(getBestMoveChessMinimax(game, details)){// no errors occurred
		setMoveChessGame(game, details[0], details[1], details[2], details[3]);
		guiPieceMove(details[0], details[1], details[2], details[3]);
		return 1;
	}
	else { //an error occurred in getBestMoveChessMinimax!! we return to main window...
		return 0;
	}
	return 1;
}


/*
 * If the game is over- return 1.
 * If a memory allocation occurred- return -1.
 * Otherwise- return 0.
 */
int isGameOverAuxGui() {
	int check_winner_result=checkWinnerOrDrawChessGame(game);
	if(check_winner_result==GAME_MEMORY_FAILURE){
		return -1;
	}
	if(check_winner_result==2){ //the result is 2 if the game isn't over
		return 0;
	}
	return 1;
}


/*
 * Draw the game's buttons, the background and the board for the game .
 */
void gameWindowDrawUtilButtons(SDL_Renderer* rend, Widget* gameUtilButtons[], int game_over_flag) {
	for (int i=0; i<8; i++) {// gameUtilButtons includes 8 Widgets to draw
		if(i<1 || i>3 || (!game_over_flag)){ //if the game is over, the buttons in indexes 1-3 aren't available and therefore aren't drawn
			if (gameUtilButtons[i]!=NULL) {
				gameUtilButtons[i]->draw(gameUtilButtons[i], rend);
			}
		}
	}
}

/*
 * Highlight available moves for a piece when it's right-clicked.
 */
void gameWindowHandleDrawHighlights(SDL_Renderer* rend, Highlight* array_of_highlights[BOARD_LENGTH][BOARD_LENGTH],
									int flags_rightclick[], int game_over_flag)
{
	int highlight_success=1;
	if (flags_rightclick[0] && (!game_over_flag)) { //a right click has been made on one of the pieces and game isn't over
		highlight_success=highlightGetMoves(game, flags_rightclick[1], flags_rightclick[2], array_of_highlights);
		if (highlight_success) {
			drawHighlightArray(rend, array_of_highlights);
		}
		else {
			flags_rightclick[0]=0;
		}
	}
	else { //don't show highlights
		makeHighlightInvisible(array_of_highlights);
	}
}

CHESS_GUI_WINDOW GameWindow(SDL_Window* main_window){
	int isButtonsCreateSuccessed=0;
	SDL_Renderer* rend=SDL_GetRenderer(main_window);

	//gameUtilButtons, by indexes:	0 - background 1 - restart, 2 - save, 3 - load, 4 - undo, 5 - main menu, 6 - quit, 7 - gameBoard
	Widget* gameUtilButtons[8];
	Widget* gameStatusButtons[5];
	Widget* gameCheckButtons[4];
	isButtonsCreateSuccessed=createGameOptionsButtons(rend, gameUtilButtons);
	if (!isButtonsCreateSuccessed) {
		//returning to main menu
		for (int i=0; i<8; i++) {
			destroyWidget(gameUtilButtons[i]);
		}
		return GUI_MAIN_WINDOW;
	}

	//continue creating util buttons: (not "vital" buttons)
	createGameStatusButtons(rend ,gameStatusButtons);
	createCheckButtons(rend, gameCheckButtons);

	// event handling loop
	int done = 0;
	int undo_flag=0, restart_flag=0;
	int game_over_flag=0;

	int flags_rightclick[3]={0};
	Highlight* array_of_highlights[BOARD_LENGTH][BOARD_LENGTH];
	createArrayofHighlights(rend, array_of_highlights);

	SDL_Event e;
	CHESS_GUI_WINDOW result=GUI_STAY;
	while (!done) {
		if(game->game_mode==1 && game->currentPlayer!=game->user_color && (!game_over_flag)){//if computer's turn
			if (!guiComputerMoveChess()) { //there's an error in computing the computer's move. returning to main window!!
				errorMessageBox("Error: can't compute computer's turn. Returning to Main Menu...\n");
				printf("Error: can't compute computer's turn. Returning to Main Menu...\n");
				done=1;
				result=GUI_MAIN_WINDOW;
				isGameSaved=1;
			}
			isGameSaved=0;
		}
		else {
			while (SDL_PollEvent(&e)) {
				if(!game_over_flag){
					gameUtilButtons[1]->handleEvent(gameUtilButtons[1], &e, &restart_flag); //restart button
					gameUtilButtons[2]->handleEvent(gameUtilButtons[2], &e, &result); //save button
					gameUtilButtons[3]->handleEvent(gameUtilButtons[3], &e, &result); //load button
					handleEventGameWindowPieces(&e, flags_rightclick);
				}

				gameUtilButtons[4]->handleEvent(gameUtilButtons[4], &e, &undo_flag); //undo button
				gameUtilButtons[5]->handleEvent(gameUtilButtons[5], &e, &result); //main menu button
				gameUtilButtons[6]->handleEvent(gameUtilButtons[6], &e, &result); //quit button

				switch (e.type) {
					case SDL_QUIT:
						done=1;
						result=GUI_QUIT;
						break;
					case SDL_KEYDOWN:
						if (e.key.keysym.sym == SDLK_ESCAPE){
							done=1;
							result=GUI_QUIT;
						}
						break;
				}
			}
		}
		//check if the game is over
		game_over_flag= isGameOverAuxGui();
		if (game_over_flag==1) { //we don't want to show the message box
			isGameSaved=1;
		}
		else if(game_over_flag==-1){
			isGameSaved=1;
			result=GUI_MAIN_WINDOW;
			errorMessageBox("Error: memory allocation error. Returning to Main Menu...\n");
			printf("Error: memory allocation error. Returning to Main Menu...\n");
		}

		if (result==GUI_SAVE_GAME || result==GUI_LOAD_GAME || result==GUI_MAIN_WINDOW || result==GUI_QUIT) {
			done=1;
			flags_rightclick[0]=0;
		}
		if(undo_flag){
			GuiUndoMove(pieces);
			undo_flag=0;
			flags_rightclick[0]=0;
		}
		if(restart_flag){
			changeWindowName(main_window, "Chessprog");
			if(!restartGuiGame(rend)){
				errorMessageBox("Error: can't restart game. You may try again.");
				printf("Error: can't restart game. You may try again.");
			}
			flags_rightclick[0]=0;
			restart_flag=0;
		}

		// clear window to color red (r,g,b,a)
		SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);
		SDL_RenderClear(rend);

		//drawing!!!
		gameWindowDrawUtilButtons(rend, gameUtilButtons, game_over_flag); //daw util buttons
		drawWindowGamePieces(rend); //all pieces are not null, o/w this window would not be created.
		gameWindowHandleDrawHighlights(rend, array_of_highlights, flags_rightclick, game_over_flag);

		if(game_over_flag){
			presentStatus(rend, gameStatusButtons);
		}
		else{
			//notify of check if occurred
			drawCheckButtons(rend, gameCheckButtons);
		}

		//if a move has been made on the game, we want to indicate that to the user
		if (isGameSaved==false) {
			changeWindowName(main_window, "Chessprog - Not Saved");
		}

		// present changes to user
		SDL_RenderPresent(rend);

		// small delay
		SDL_Delay(10);
	}
	//destroying
	for (int i=0; i<8; i++) {
		destroyWidget(gameUtilButtons[i]);
	}
	for (int i=0; i<5; i++) {
		destroyWidget(gameStatusButtons[i]);
	}
	for (int i=0; i<4; i++) {
		destroyWidget(gameCheckButtons[i]);
	}

	destoryArrayofHightlights(array_of_highlights);
	return result;
}
