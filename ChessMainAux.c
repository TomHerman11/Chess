/*
 * ChessMainAux.c
 *
 *  Created on: Feb 28, 2018
 *      Author: krimolovsky
 */

#include "ChessMainAux.h"

ChessGame* ChessMainSettings(ChessGame* game) {
	CHESSCommand command;
	char input[COMMAND_LENGTH];

	printf("Specify game settings or type 'start' to begin a game with the current settings:\n");
	while (1) {
		fgets(input, COMMAND_LENGTH, stdin);
		command=parserSettingsParseLine(input);
		if (command.cmd==CHESS_GAME_MODE) {
			changeGameModeAux(game, command.number);
		}
		if (command.cmd==CHESS_DIFFICULTY) {
			changeGameDifficultyAux(game, command.number);
		}
		if (command.cmd==CHESS_USER_COLOR) {
			changeGameUserColorAux(game,command.number);
		}
		if (command.cmd==CHESS_LOAD) {
			game=loadGameAux(game, command.path, 0, NULL);
		}
		if (command.cmd==CHESS_DEFAULT) {
			printf("All settings reset to default\n");
			setChessGameSettingsDefault(game);
		}
		if (command.cmd==CHESS_PRINT_SETTINGS) {
			printSettingsAux(game);
		}
		if (command.cmd==CHESS_QUIT) {
			quitGameAuxCONSOLE(game);
		}
		if (command.cmd==CHESS_INVALID) {
			printf("ERROR: invalid command\n");
		}
		if (command.cmd==CHESS_START) {
			printf("Starting game...\n");
			break;
		}
	}
	return game;
}

void changeGameModeAux(ChessGame* game, int mode) {
	if (changeGameMode(game, mode)) {
		printf("Game mode is set to %d-player\n", mode);
	}
	else {
		printf("Wrong game mode\n");
	}
}

void changeGameDifficultyAux(ChessGame* game, int difficulty) {
	if (game->game_mode!=1) {
		printf("ERROR: invalid command\n");
		return;
	}
	char* levels[]={"amateur", "easy", "moderate", "hard", "expert"};
	if (changeGameDifficuly(game, difficulty)) {
		printf("Difficulty level is set to %s\n", levels[difficulty-1]);
	}
	else {
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
	}
}

void changeGameUserColorAux(ChessGame* game, int user_color) {
	char *p="white";
	if (game->game_mode!=1) {
		printf("ERROR: invalid command\n");
		return;
	}
	if (!user_color) {
		p="black";
	}
	if (changeUserColor(game, user_color)) {
		printf("User color is set to %s\n", p);
	}
	else {
		printf("Wrong user color. The value should be 0 or 1\n");
	}
}

void printSettingsAux(ChessGame* game) {
	char* levels[]={"amateur", "easy", "moderate", "hard", "expert"};
	char* user_colors[]={"black", "white"};

	printf("SETTINGS:\n");
	printf("GAME_MODE: %d-player\n", game->game_mode);
	if (game->game_mode==1) {
		printf("DIFFICULTY: %s\n", levels[game->difficulty-1]);
		printf("USER_COLOR: %s\n", user_colors[game->user_color]);
	}
}

void quitGameAuxCONSOLE(ChessGame* game) {
	destoryChessGame(game);
	printf("Exiting...\n");
	exit(0);
}

int saveGameAux(ChessGame* game, char* path) {
	FILE* file=NULL;
	char* current_player=game->currentPlayer ? "white" : "black";
	char* user_color=game->user_color ? "white" : "black";
	char* levels[]={"amateur", "easy", "moderate", "hard", "expert"};
	char* game_level=levels[game->difficulty-1];
	file=fopen(path, "w");
	if (file==NULL) {
		printf("File cannot be created or modified\n");
		return 0;
	}
	//ASSUMPTION - the writing will be successful
	fprintf(file, "%s\n",current_player);
	fprintf(file, "SETTINGS:\nGAME_MODE: %d-player\n", game->game_mode);
	fprintf(file, "DIFFICULTY: %s\n", game_level);
	fprintf(file, "USER_COLOR: %s\n", user_color);

	//now saving the board status
	for (int i=BOARD_LENGTH-1; i>=0; i--) {
		fprintf(file, "%d| ", i+1);
		for (int j=0; j<BOARD_LENGTH; j++) {
			fprintf(file, "%c ", game->gameBoard[i][j]);
		}
		fprintf(file, "|\n");
	}
	fprintf(file, "  -----------------\n");
	fprintf(file, "   A B C D E F G H\n");
	fclose(file);
	return 1;
}

ChessGame* loadGameAux(ChessGame* old_game, char* path, int calledFromGui, int* loadSuccess) {
	ChessGame *game=NULL;
	FILE* file=NULL;
	char buffer_string[LINELENGTHLOADFILE]={'\0'};
	int jump_in_buffer=0;
	char* levels[]={"amateur\n", "easy\n", "moderate\n", "hard\n", "expert\n"};

	file=fopen(path, "r");
	if (file==NULL) {
		printf("Error: File doesn't exist or cannot be opened\n");
		if (calledFromGui) {
			*loadSuccess=0;
		}
		return old_game;
	}

	game=createChessGame();
	if(game==NULL){
		printf("Error: Can't load game due to memory allocation failure\n");
		fclose(file);
		if (calledFromGui) {
			*loadSuccess=0;
		}
		return old_game;
	}
	destoryChessGame(old_game); //old game is not null, checked before calling to function.

	//ASSUMPTION - the reading will be successful
	//receiving current player:
	fgets(buffer_string, LINELENGTHLOADFILE, file);
	if (!strcmp(buffer_string, "black\n")) { //means current player is black now
		game->currentPlayer=0;
	}

	//receiving game mode:
	fgets(buffer_string, LINELENGTHLOADFILE, file); fgets(buffer_string, LINELENGTHLOADFILE, file); //skipping the SETTINGS line
	if (buffer_string[11]=='2') {
		game->game_mode=2;
	}

	//receiving difficulty:
	fgets(buffer_string, LINELENGTHLOADFILE, file);
	for (int i=0; i<5; i++) {
		if (!strcmp(buffer_string+12, levels[i])) {
			game->difficulty=i+1;
		}
	}

	//receiving user color:
	fgets(buffer_string, LINELENGTHLOADFILE, file);
	if (!strcmp(buffer_string+12, "black\n")) { //means current player is black now
		game->user_color=0;
	}

	//receiving board state:
	for (int i=BOARD_LENGTH-1; i>=0; i--) {
		fgets(buffer_string, LINELENGTHLOADFILE, file);
		for (int j=0; j<BOARD_LENGTH; j++) {
			game->gameBoard[i][j]=*(buffer_string+3+jump_in_buffer);
			jump_in_buffer+=2;
		}
		jump_in_buffer=0;
	}

	fclose(file);

	//update the isKingThreatened fields of the game.
	if (game->currentPlayer==1) {
		game->isWhiteKingThreatened=checkIsKingThreatened(game, game->currentPlayer);
	}
	if (game->currentPlayer==0) {
		game->isBlackKingThreatened=checkIsKingThreatened(game, game->currentPlayer);
	}

	return game;
}

ChessGame* inGamePlayerCommandsAux(ChessGame* game){
	static int print_board_inGamePlayer=1;
	CHESSCommand command;
	int result_set_move=0;
	char input[COMMAND_LENGTH];
	char *player=game->currentPlayer ? "white" : "black";
	if (print_board_inGamePlayer) {
		printBoardChessGame(game);
	}

	while (1) {
		printf("Enter your move (%s player):\n", player);
		fgets(input, COMMAND_LENGTH, stdin);
		command=parserGameParseLine(input);
		if (command.cmd==CHESS_MOVE) {
			result_set_move=setMoveAux(game, command.from[0], command.from[1], command.to[0], command.to[1], &print_board_inGamePlayer);
			if (result_set_move==GAME_MEMORY_FAILURE) { //exiting the game, cannot make a user move
				destoryChessGame(game);
				exit(0);
			}
			break;
		}
		if (command.cmd==CHESS_GET_MOVES) {
			getMovesAux(game, command.from[0], command.from[1]);
		}
		if (command.cmd==CHESS_SAVE) {
			if (saveGameAux(game, command.path)) {
				printf("Game saved to: %s\n", command.path);
			}
		}
		if (command.cmd==CHESS_UNDO) {
			if(undoMoveAux(game)){ //undo 2 turns if possible, otherwise undo 1 turn (if no undo available prints it)
				print_board_inGamePlayer=1;
				break;
			}
		}
		if (command.cmd==CHESS_RESET) {
			printf("Restarting...\n");
			print_board_inGamePlayer=1;
			return resetGameAux(game);
		}
		if (command.cmd==CHESS_QUIT) {
			quitGameAuxCONSOLE(game);
		}
		if (command.cmd==CHESS_INVALID) {
			printf("ERROR: invalid command\n");
		}
	}
	return game;
}

int setMoveAux(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y, int* print_board_inGamePlayer) {
	int is_valid_result=isValidMoveChessGame(game, pre_x, pre_y, new_x, new_y);
	if (is_valid_result==GAME_MEMORY_FAILURE) {
		printf("Error: Memory allocation failure. Cannot set move in the game. Exiting... \n");
		return GAME_MEMORY_FAILURE;
	}

	if (is_valid_result==0) {
		setMoveChessGame(game, pre_x, pre_y, new_x, new_y);
		*print_board_inGamePlayer=1;
		return 1;
	}
	if (is_valid_result==1){
		printf("Invalid position on the board\n");
		*print_board_inGamePlayer=0;
		return 0;
	}
	if (is_valid_result==2) {
		printf("The specified position does not contain your piece\n");
		*print_board_inGamePlayer=0;
		return 0;
	}
	if (is_valid_result==3) {
		printf("Illegal move\n");
		*print_board_inGamePlayer=0;
		return 0;
	}
	if (is_valid_result==4) {
		if ((game->currentPlayer && game->isWhiteKingThreatened) ||
				(!game->currentPlayer && game->isBlackKingThreatened)) {
			printf("Illegal move: king is still threatened\n");
			*print_board_inGamePlayer=0;
			return 0;
		}
		printf("Illegal move: king will be threatened\n");
		*print_board_inGamePlayer=0;
		return 0;
	}
	return 1;
}

void getMovesAux(ChessGame* game, char x, char y) {
	int get_moves_result=getMoves(game, x, y);
	if (get_moves_result==GAME_GETMOVES_MEMORY_FAILURE) {
		printf("Error: Memory allocation failure. Could not continue calculation of get moves command\n");
		return;
	}
	if (get_moves_result==1) {
		printf("Invalid position on the board\n");
		return;
	}
	if (get_moves_result==2) {
		printf("The specified position does not contain a player piece\n");
		return;
	}
}

int undoMoveAux(ChessGame* game) {
	int undo_move_result=undoMoveChessGame(game,1); //first undo
	if (undo_move_result==1) {
		printf("Empty history, no move to undo\n");
		return 0;
	}
	//trying to do the second undo. in case there is only 1 undo available, we will return 1
	if (undoMoveChessGame(game,1)==1) { //meaning second undo is NOT possible
		if(game->game_mode==1 && game->currentPlayer!=game->user_color){ //now computer turn, need to print after undo
			printBoardChessGame(game);
		}
	}
	return 1;
}

ChessGame* resetGameAux(ChessGame* game){
	ChessGame* new_game;
	new_game=createChessGame();
	if (new_game==NULL) {
		printf("Error: could not reset the game. Please continue playing with the current game\n");
		return game;
	}
	new_game->difficulty=game->difficulty;
	new_game->game_mode=game->game_mode;
	new_game->user_color=game->user_color;
	destoryChessGame(game);
	return ChessMainSettings(new_game);
}

int isGameOverAux(ChessGame *game) {
	int check_winner_result=checkWinnerOrDrawChessGame(game);
	if (check_winner_result==GAME_MEMORY_FAILURE) {
		printf("Error: Memory allocation failure. Exiting... \n");
		return 1;
	}
	if (check_winner_result==0) {
		printf("Checkmate! black player wins the game\n");
	}
	if (check_winner_result==1) {
		printf("Checkmate! white player wins the game\n");
	}
	if (check_winner_result==3) {
		printf("The game ends in a draw\n");
	}
	if (check_winner_result==2) {
		return 0; //no winner nor draw, game continues
	}
	return 1;
}

void checkThreatenedAux(ChessGame* game) {
	if (game->currentPlayer && game->isWhiteKingThreatened) {
		printf("Check: white king is threatened\n");
	}
	if (!game->currentPlayer && game->isBlackKingThreatened) {
		printf("Check: black king is threatened\n");
	}
}

int computerMoveChess(ChessGame *game) {
	char details[4];
	char* piece_name=NULL;
	char piece;
	int error_check=getBestMoveChessMinimax(game, details);
	if (error_check==0) {
		return 0;
	}

	piece=game->gameBoard[details[0]-'1'][details[1]-'A'];
	if (piece=='m' || piece=='M') {
		piece_name="pawn";
	}
	if (piece=='n' || piece=='N') {
		piece_name="knight";
	}
	if (piece=='b' || piece=='B') {
		piece_name="bishop";
	}
	if (piece=='r' || piece=='R') {
		piece_name="rook";
	}
	if (piece=='q' || piece=='Q') {
		piece_name="queen";
	}
	if (piece=='k' || piece=='K') {
		piece_name="king";
	}
	printf("Computer: move %s at <%c,%c> to <%c,%c>\n", piece_name, details[0], details[1], details[2], details[3]);
	setMoveChessGame(game, details[0], details[1], details[2], details[3]);
	return 1;
}
