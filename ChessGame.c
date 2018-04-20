/*
 * ChessGame.c
 *
 *  Created on: Feb 26, 2018
 *      Author: krimolovsky
 */

#include "ChessGame.h"

ChessGame* createChessGame() {
	char white_pieces[8]={'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
	char black_pieces[8]={'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};
	ChessGame* game;
	game=(ChessGame*)calloc(1,sizeof(ChessGame));
	if (game==NULL) {
		return NULL;
	}
	//game settings
	setChessGameSettingsDefault(game);
	game->currentPlayer=DEFAULT_CURRENT_PLAYER;
	game->isWhiteKingThreatened=0;
	game->isBlackKingThreatened=0;

	//creating history
	game->history=createHistoryChess();
	if (game->history==NULL) {
		free(game);
		return NULL;
	}
	//setting the board:
	memcpy(game->gameBoard[0], white_pieces, sizeof(game->gameBoard[0]));
	memcpy(game->gameBoard[7], black_pieces, sizeof(game->gameBoard[7]));
	for (int i=0; i<BOARD_LENGTH; i++) {
		game->gameBoard[1][i]='m';
		game->gameBoard[6][i]='M';
	}
	for (int i=2; i<=5; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			game->gameBoard[i][j]=EMPTY_ENTRY;
		}
	}

	return game;
}

ChessGame* copyChessGame(ChessGame* game) {
	ChessGame* game_copy=NULL;
	ChessHistory* history_copy=NULL;
	if (game==NULL) {
		return NULL;
	}
	game_copy=(ChessGame*)calloc(1, sizeof(ChessGame));
	if (game_copy==NULL) {
		return NULL;
	}
	game_copy->game_mode=game->game_mode;
	game_copy->difficulty=game->difficulty;
	game_copy->user_color=game->user_color;
	game_copy->currentPlayer=game->currentPlayer;
	game_copy->isWhiteKingThreatened=game->isWhiteKingThreatened;
	game_copy->isBlackKingThreatened=game->isBlackKingThreatened;
	history_copy=copyHistoryChess(game->history);
	if (history_copy==NULL) {
		free(game_copy);
		return NULL;
	}
	game_copy->history=history_copy;
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			game_copy->gameBoard[i][j]=game->gameBoard[i][j];
		}
	}

	return game_copy;
}

void setChessGameSettingsDefault(ChessGame* game) {
	if (game==NULL) {
		return;
	}
	game->game_mode=DEFAULT_GAME_MODE;
	game->difficulty=DEFAULT_DIFFICULTY;
	game->user_color=DEFAULT_USER_COLOR;

	return;
}

void destoryChessGame(ChessGame* game) {
	if (game==NULL) {
		return;
	}
	destoryHistoryChess(game->history);
	free(game);
	return;
}

int changeGameMode(ChessGame* game, int new_game_mode) {
	if (new_game_mode!=1 && new_game_mode!=2) {
		return 0;
	}
	game->game_mode=new_game_mode;
	return 1;
}

int changeGameDifficuly(ChessGame* game, int new_difficulty) {
	if (new_difficulty<1 || new_difficulty>5) {
		return 0;
	}
	game->difficulty=new_difficulty;
	return 1;
}

int changeUserColor(ChessGame* game, int new_color) {
	if (new_color!=0 && new_color!=1) {
		return 0;
	}
	game->user_color=new_color;
	return 1;
}

void printBoardChessGame(ChessGame* game) {
	for (int i=BOARD_LENGTH-1; i>=0; i--) {
		printf("%d| ", i+1);
		for (int j=0; j<BOARD_LENGTH; j++) {
			printf("%c ", game->gameBoard[i][j]);
		}
		printf("|\n");
	}
	printf("  -----------------\n");
	printf("   A B C D E F G H\n");
}

//checks if the king of current player is threatened.
int checkIsKingThreatened(ChessGame* game, int currentPlayer) {
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			if (currentPlayer==1 && game->gameBoard[i][j]=='k'){  //currentPlayer is the white player, we found the white king
				if (isThreatend(game, i+'1', j+'A')) {
					return 1;
				}
				return 0; //if not threatened
			}
			else if(currentPlayer==0 && game->gameBoard[i][j]=='K') { //currentPlayer is the black player, we found the black king
				if (isThreatend(game, i+'1', j+'A')) {
					return 1;
				}
				return 0; //if not threatened
			}
		}
	}
	return 0;
}

void setMoveChessGame(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y) {
	//insert the move to history
	char details[5]={pre_x, pre_y, new_x, new_y, game->gameBoard[new_x-'1'][new_y-'A']};
	addFirstHistoryChess(game->history, details);

	game->gameBoard[new_x-'1'][new_y-'A']=game->gameBoard[pre_x-'1'][pre_y-'A'];
	game->gameBoard[pre_x-'1'][pre_y-'A']=EMPTY_ENTRY;

	//the move was already checked to be valid.
	//meaning - if this function is called, the king can no longer be threatened.
	if(game->currentPlayer){
		game->isWhiteKingThreatened=0;
	}
	if(!game->currentPlayer){
		game->isBlackKingThreatened=0;
	}

	//change the current player
	game->currentPlayer=1-game->currentPlayer;

	//check if the move threatens the opponent's king (*the opponent is now the current player)
	if (game->currentPlayer==1) {
		game->isWhiteKingThreatened=checkIsKingThreatened(game, game->currentPlayer);
	}
	if (game->currentPlayer==0) {
		game->isBlackKingThreatened=checkIsKingThreatened(game, game->currentPlayer);
	}
}

int isValidMoveChessGame(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y) {
	ChessGame* game_copy=NULL;

	//First error check - invalid position on the board
	if (pre_x<'1' || pre_x>'8' || pre_y<'A' || pre_y>'H') {
			return 1;
		}
	if (new_x<'1' || new_x>'8' || new_y<'A' || new_y>'H') {
		return 1;
	}

	//Second error check - specified position does not contain your piece
	if(game->currentPlayer!=checkPieceType(game, pre_x, pre_y)){
		return 2;
	}

	//Third error check - valid move for specific piece
	if ((pre_x==new_x) && (pre_y==new_y)) { //same spot, not moving at all
		return 3;
	}
	if(game->currentPlayer==checkPieceType(game, new_x, new_y)){//new location contain a piece of the same color
		return 3;
	}
	//now checking which piece is it (and if the move is legal):

	if (!isValidPieceMove(game, pre_x, pre_y, new_x, new_y)) { //the move is not valid due to the rules of the specific piece
		return 3;
	}

	//Fourth error check - does the move threats the king
	//creating a copy of the game -> we will make the move in the copy, and then check if the king of same color is now threatened
	game_copy=copyChessGame(game);
	if (game_copy==NULL) {
		return GAME_MEMORY_FAILURE; //returns MEMORY_FAILURE because of memory allocation failure
	}

	setMoveChessGame(game_copy, pre_x, pre_y, new_x, new_y);

	if(checkIsKingThreatened(game_copy, game->currentPlayer)){
		destoryChessGame(game_copy);
		return 4;
	}
	destoryChessGame(game_copy);
	return 0;
}

//getting valid different positions that contains game pieces of different colors
int isValidPieceMove(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y){
	char piece=game->gameBoard[pre_x-'1'][pre_y-'A'];
	int flag_valid_move_by_piece=1;
	if (piece=='r' || piece=='R') {
		flag_valid_move_by_piece=isValidMoveRook(game, pre_x, pre_y, new_x, new_y);
	}
	else if (piece=='b' || piece=='B') {
		flag_valid_move_by_piece=isValidMoveBishop(game, pre_x, pre_y, new_x, new_y);
	}
	else if (piece=='q' || piece=='Q') {
		flag_valid_move_by_piece=isValidMoveQueen(game, pre_x, pre_y, new_x, new_y);
	}
	else if (piece=='k' || piece=='K') {
		flag_valid_move_by_piece=isValidMoveKing(pre_x, pre_y, new_x, new_y);
	}
	else if (piece=='n' || piece=='N') {
		flag_valid_move_by_piece=isValidMoveKnight(pre_x, pre_y, new_x, new_y);
	}
	else if (piece=='m' || piece=='M') {
		flag_valid_move_by_piece=isValidMovePawn(game, pre_x, pre_y, new_x, new_y);
	}
	return flag_valid_move_by_piece;
}

int checkPieceType(ChessGame* game, char x, char y) {
	int result=2;
	char white_pieces[6]={'r', 'n', 'b', 'k', 'q', 'm'};
	char black_pieces[6]={'R', 'N', 'B', 'K', 'Q', 'M'};

	if (strchr(white_pieces,game->gameBoard[x-'1'][y-'A'])!=NULL) {
		result=1;
	}
	else if (strchr(black_pieces,game->gameBoard[x-'1'][y-'A'])!=NULL) {
		result=0;
	}
	return result;
}

int isValidMoveRook(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y) {
	//ASSUMPTION: (pre_x,pre_y)!=(new_x,new_y) - was checked before entering the function

	int higher_coor;
	int lower_coor;
	if (pre_x!=new_x && pre_y!=new_y) {
		return 0; //not legal rook move
	}
	if (pre_x==new_x) {
		if (pre_y>new_y){
			higher_coor=pre_y-'A';
			lower_coor=new_y-'A';
		}
		else{
			higher_coor=new_y-'A';
			lower_coor=pre_y-'A';
		}
		for(int i=lower_coor+1;i<higher_coor;i++){
			if(game->gameBoard[new_x-'1'][i]!=EMPTY_ENTRY){
				return 0; //the way to the goal is not clear...
			}
		}
	}
	if (pre_y==new_y) {
		if (pre_x>new_x){
			higher_coor=pre_x-'1';
			lower_coor=new_x-'1';
		}
		else{
			higher_coor=new_x-'1';
			lower_coor=pre_x-'1';
		}
		for(int i=lower_coor+1;i<higher_coor;i++){
			if(game->gameBoard[i][new_y-'A']!=EMPTY_ENTRY){
				return 0; //the way to the goal is not clear...
			}
		}
	}
	return 1;
}

int isValidMoveBishop(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y) {
	//ASSUMPTION: (pre_x,pre_y)!=(new_x,new_y) - was checked before entering the function
	int distance;

	//check if the move is diagonal:
	if (abs(pre_x-new_x)!=abs(pre_y-new_y)) {
		return 0;
	}
	distance=abs(pre_x-new_x);
	// 4 ways of moving - checking if the way is 'clear':
	//moving up-right
	if (new_x>pre_x && new_y>pre_y) {
		for (int i=1; i<distance; i++) {
			if (game->gameBoard[pre_x-'1'+i][pre_y-'A'+i]!=EMPTY_ENTRY) {
				return 0;
			}
		}
		return 1;
	}
	//moving up-left
	if (new_x>pre_x && new_y<pre_y) {
		for (int i=1; i<distance; i++) {
			if (game->gameBoard[pre_x-'1'+i][pre_y-'A'-i]!=EMPTY_ENTRY) {
				return 0;
			}
		}
		return 1;
	}
	//moving down-right
	if (new_x<pre_x && new_y>pre_y) {
		for (int i=1; i<distance; i++) {
			if (game->gameBoard[pre_x-'1'-i][pre_y-'A'+i]!=EMPTY_ENTRY) {
				return 0;
			}
		}
		return 1;
	}
	//moving down-left
	if (new_x<pre_x && new_y<pre_y) {
		for (int i=1; i<distance; i++) {
			if (game->gameBoard[pre_x-'1'-i][pre_y-'A'-i]!=EMPTY_ENTRY) {
				return 0;
			}
		}
		return 1;
	}
	return 1;
}

int isValidMoveQueen(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y) {
	//ASSUMPTION: (pre_x,pre_y)!=(new_x,new_y) - was checked before entering the function
	if(isValidMoveRook(game, pre_x, pre_y, new_x, new_y)){
		return 1;
	}
	if(isValidMoveBishop(game, pre_x, pre_y, new_x, new_y)){
		return 1;
	}
	return 0;
}

int isValidMoveKnight(char pre_x, char pre_y, char new_x, char new_y) {
	//ASSUMPTION: (pre_x,pre_y)!=(new_x,new_y) - was checked before entering the function
	//moreover - check if the new location does not contain a piece of the same color was checked.

	int distance_x=abs(pre_x-new_x);
	int distance_y=abs(pre_y-new_y);
	if ((distance_x==1) && (distance_y==2)) {
		return 1;
	}
	if ((distance_x==2) && (distance_y==1)) {
		return 1;
	}
	return 0;
}

int isValidMoveKing(char pre_x, char pre_y, char new_x, char new_y) {
	int distance_x=abs(pre_x-new_x);
	int distance_y=abs(pre_y-new_y);
	if(distance_x==0 && distance_y==1) {
		return 1;
	}
	if(distance_x==1 && distance_y==0) {
		return 1;
	}
	if(distance_x==1 && distance_y==1) {
		return 1;
	}
	return 0;
}

int isValidMovePawn(ChessGame* game, char pre_x, char pre_y, char new_x, char new_y) {
	//ASSUMPTION - Board(pre_x,pre_y) contains a piece of the same color as the current player

	if (game->currentPlayer==1) { //pawn is white
		//checking if pawn is moving diagonally
		if (new_x-pre_x==1 && abs(new_y-pre_y)==1) {
			if (checkPieceType(game, new_x, new_y)==0) { //target square contains a black piece
				return 1;
			}
			return 0; //moving diagonally to an empty space is not legal for pawn
		}
		//checking if pawn is moving forward
		if (pre_y!=new_y) { //not moving forward
			return 0;
		}
		if (new_x-pre_x==1 && checkPieceType(game, new_x, new_y)==2) { //moving forward by 1 step
			return 1;
		}

		if (pre_x=='2' && new_x-pre_x==2) { //pawn can move by 2 steps
			if(checkPieceType(game, new_x-1, new_y)==2 && checkPieceType(game, new_x, new_y)==2){
				return 1;
			}
		}
		return 0;
	}

	if (game->currentPlayer==0) { //pawn is black
		//checking if pawn is moving diagonally
		if (pre_x-new_x==1 && abs(new_y-pre_y)==1) {
			if (checkPieceType(game, new_x, new_y)==1) { //target square contains a white piece
				return 1;
			}
			return 0; //moving diagonally to an empty space is not legal for pawn
		}
		//checking if pawn is moving forward
		if (pre_y!=new_y) { //not moving forward
			return 0;
		}
		if (pre_x-new_x==1 && checkPieceType(game, new_x, new_y)==2) { //moving forward by 1 step
			return 1;
		}

		if (pre_x=='7' && pre_x-new_x==2) { //pawn can move by 2 steps
			if(checkPieceType(game, new_x+1, new_y)==2 && checkPieceType(game, new_x, new_y)==2){
				return 1;
			}
		}
		return 0;
	}
	return 0;
}

int isThreatend(ChessGame* game, char x, char y) {
	int position_color=checkPieceType(game, x, y);
	if (position_color==2) return 0; //this square is empty, no threats at all!

	//checking all positions to see if Board(x,y) is threatened
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			if (1-checkPieceType(game, i+'1', j+'A')==position_color){ //this square contains piece from a different color
				//checking if this piece can move to the square (x,y) - if it does then (x,y) IS THREATENED!
				if (isValidPieceMove(game,i+'1', j+'A', x, y)==1) {
					return 1;
				}
			}
		}
	}
	return 0;
}

//void getMoves(ChessGame* game, char x, char y) { }

int getMoves(ChessGame* game, char x, char y) {
	int piece_type=0;
	int result_is_valid=0;
	ChessGame *game_copy=NULL;
	ChessGame *game_copy2=NULL;
	if (x<'1' || x>'8' || y<'A' || y>'H') {
		return 1; //invalid position on the board
	}
	piece_type=checkPieceType(game, x, y);
	if(piece_type==2){
		return 2;//position doesn't contain a player piece
	}
	game_copy=copyChessGame(game);
	if(game_copy==NULL){
		return GAME_GETMOVES_MEMORY_FAILURE; //memory allocation failure
	}
	(game_copy->currentPlayer)=piece_type; //player in game_copy has the same color as the piece
	for(int i=0; i<BOARD_LENGTH; i++){
		for(int j=0; j<BOARD_LENGTH;j++){
			result_is_valid=isValidMoveChessGame(game_copy, x,y, i+'1',j+'A');
			if (result_is_valid==GAME_MEMORY_FAILURE) {
				destoryChessGame(game_copy);
				return GAME_GETMOVES_MEMORY_FAILURE;
			}
			if (!result_is_valid){
				printf("<%c,%c>", i+'1',j+'A');

				//checking if the piece is threatened
				game_copy2=copyChessGame(game_copy);
				if (game_copy2==NULL) {
					destoryChessGame(game_copy);
					return GAME_GETMOVES_MEMORY_FAILURE;
				}
				setMoveChessGame(game_copy2, x,y, i+'1',j+'A');
				if(isThreatend(game_copy2, i+'1',j+'A')){
					printf("*");
				}
				destoryChessGame(game_copy2);

				//checking if the piece captures a rival's piece in the new position
				//position can't hold a piece of the same color- checked in isValidChessGame
				if(game_copy->gameBoard[i][j]!=EMPTY_ENTRY) {
					printf("^");
				}
				printf("\n");
			}
		}
	}
	destoryChessGame(game_copy);
	return 0;//no errors.
}

int undoMoveChessGame(ChessGame* game, int consoleModeFlag) {
	char details[5];
	char pre_x;
	char pre_y;
	char new_x;
	char new_y;
	char symbol;

	//if there are no moves to undo:
	if (isEmptyHistoryChess(game->history)) {
		return 1;
	}

	//details=getFirstHistoryChess(game->history);
	getFirstHistoryChess(game->history, details);
	pre_x=details[0];
	pre_y=details[1];
	new_x=details[2];
	new_y=details[3];
	symbol=details[4];

	if(consoleModeFlag){
		if (!game->currentPlayer) { //need to undo the move of the white player
			printf("Undo move for white player: <%c,%c> -> <%c,%c>\n", new_x, new_y, pre_x, pre_y);
		}
		else {
			printf("Undo move for black player: <%c,%c> -> <%c,%c>\n", new_x, new_y, pre_x, pre_y);
		}
	}
	game->gameBoard[pre_x-'1'][pre_y-'A']=game->gameBoard[new_x-'1'][new_y-'A'];
	game->gameBoard[new_x-'1'][new_y-'A']=symbol;
	deleteFirstHistoryChess(game->history); //deleting the move from history
	game->currentPlayer=1-game->currentPlayer; //changing the player

	//updating the isKingThreatened
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			if (game->gameBoard[i][j]=='k'){  //current player OF GAME was white, we found the white king
				game->isWhiteKingThreatened=isThreatend(game, i+'1', j+'A');
			}
			if(game->gameBoard[i][j]=='K') { //current player OF GAME was black, we found the black king
				game->isBlackKingThreatened=isThreatend(game, i+'1', j+'A');
			}
		}
	}
	return 0; //meaning no errors
}

int checkWinnerOrDrawChessGame(ChessGame* game) {
	int result_is_valid=0;
	for (int i=0; i<BOARD_LENGTH; i++) {
		for (int j=0; j<BOARD_LENGTH; j++) {
			if(checkPieceType(game, i+'1', j+'A')==game->currentPlayer){
				//now we need to check if there are any available moves for the current player
				for(int k=0; k<BOARD_LENGTH; k++){
						for(int l=0; l<BOARD_LENGTH;l++){
							result_is_valid=isValidMoveChessGame(game, i+'1',j+'A', k+'1',l+'A');
							if (result_is_valid==GAME_MEMORY_FAILURE) {
								return GAME_MEMORY_FAILURE;
							}
							if (!result_is_valid) {
								return 2;
							}
						}
					}
				}
			}
		}
	if ((game->currentPlayer==1 && game->isWhiteKingThreatened)) { //white player's turn, no moves available, white kings is threatened
		return 0; //black wins
	}
	if ((game->currentPlayer==0 && game->isBlackKingThreatened)) { //black player's turn, no moves available, black kings is threatened
		return 1; //white wins
	}
	//if we reached this point, no moves available, no king is threatened - there's a draw!
	return 3;
}
