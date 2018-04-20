/*
 * Parser.c
 *
 *  Created on: Feb 25, 2018
 *      Author: krimolovsky
 */
#include "ChessParser.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool parserIsInt(const char* str) {
	if(str==NULL){
		return false;
	}
	int length=strlen(str);
	if ((str[0]<48 || str[0]>57)&& str[0]!='-'){
		return false;
	}
	for (int i=1; i<length-1; i++) { //checks first if the string is integer/long
		if (str[i]<48 || str[i]>57) {
			return false;
		}
	}
	if (atoi(str)!=0) { //checks if the string is *integer* and not 0
		return true;
	}

	for (int i=0; i<length-1; i++) { //atoi returns 0 if the number is 0, thus we want to return true if the number is pure 0.
		if (str[i]!=48) {
			return false;
		}
	}
	return true;
}

CHESS_COMMAND parserSettingsParseLineHelper(char* arg1) {
	CHESS_COMMAND enum_command;
	if (!strcmp(arg1, "game_mode")) {
		enum_command=CHESS_GAME_MODE;
	}
	else if (!strcmp(arg1, "difficulty")) {
		enum_command=CHESS_DIFFICULTY;
	}
	else if (!strcmp(arg1, "user_color")) {
		enum_command=CHESS_USER_COLOR;
	}
	else if (!strcmp(arg1, "load")) {
		enum_command=CHESS_LOAD;
	}
	else if (!strcmp(arg1, "default")) {
		enum_command=CHESS_DEFAULT;
	}
	else if (!strcmp(arg1, "print_settings")) {
		enum_command=CHESS_PRINT_SETTINGS;
	}
	else if (!strcmp(arg1, "quit")) {
		enum_command=CHESS_QUIT;
	}
	else if (!strcmp(arg1, "start")) {
		enum_command=CHESS_START;
	}
	else {
		enum_command=CHESS_INVALID;
	}
	return enum_command;
}

CHESSCommand parserSettingsParseLine(const char* str) {
	char copy_str[COMMAND_LENGTH]={'\0'};
	const char delim[]=" \t\r\n";
	char* arg1=NULL;
	char* arg2=NULL;
	CHESS_COMMAND enum_command;
	CHESSCommand command;

	//copying the input string, since str is const.
	strcpy(copy_str, str);

	//getting the arguments from the command line
	arg1=strtok(copy_str, delim);
	arg2=strtok(NULL, delim);

	//if no command was written
	if(arg1==NULL) {
			command.cmd=CHESS_INVALID;
			return command;
	}

	//check type of command || not valid command
	enum_command=parserSettingsParseLineHelper(arg1);

	//initializing the command to be returned
	command.cmd=enum_command;

	if (command.cmd==CHESS_GAME_MODE || command.cmd==CHESS_DIFFICULTY || command.cmd==CHESS_USER_COLOR) {
		if (parserIsInt(arg2)) {
			command.number=atoi(arg2);
		}
		else {
			command.number=-1;
		}
	}
	else if (command.cmd==CHESS_LOAD) {
		if (arg2==NULL) {
			command.cmd=CHESS_INVALID;
		}
		else {
			strcpy(command.path, arg2);
		}
	}
	return command;
}

CHESS_COMMAND parserGameParseLineHelper(char* arg1) {
	CHESS_COMMAND enum_command;
	if (!strcmp(arg1, "move")) {
		enum_command=CHESS_MOVE;
	}
	else if (!strcmp(arg1, "get_moves")) {
		enum_command=CHESS_GET_MOVES;
	}
	else if (!strcmp(arg1, "save")) {
		enum_command=CHESS_SAVE;
	}
	else if (!strcmp(arg1, "undo")) {
		enum_command=CHESS_UNDO;
	}
	else if (!strcmp(arg1, "reset")) {
		enum_command=CHESS_RESET;
	}
	else if (!strcmp(arg1, "quit")) {
		enum_command=CHESS_QUIT;
	}
	else {
		enum_command=CHESS_INVALID;
	}
	return enum_command;
}

bool parserCoordinates(char *arg2) {
	//check first if the coordinates in the right length
	if (arg2==NULL) {
		return false;
	}
	if (strlen(arg2)==5) {
		if (arg2[0]=='<' && arg2[2]==',' && arg2[4]=='>') {
			return true;
		}
	}
	return false;
}

CHESSCommand parserGameParseLine(const char* str) {
	char copy_str[COMMAND_LENGTH]={'\0'};
	const char delim[]=" \t\r\n";
	char* arg1=NULL;
	char* arg2=NULL;
	char* arg3=NULL;
	char* arg4=NULL;
	CHESS_COMMAND enum_command;
	CHESSCommand command;

	//copying the input string, since str is const.
	strcpy(copy_str, str);

	//getting the arguments from the command line
	arg1=strtok(copy_str, delim);
	arg2=strtok(NULL, delim);
	arg3=strtok(NULL, delim);
	arg4=strtok(NULL, delim);

	//if no command was written
	if(arg1==NULL) {
			command.cmd=CHESS_INVALID;
			return command;
	}

	//check type of command || not valid command
	enum_command=parserGameParseLineHelper(arg1);

	//initializing the command to be returned
	command.cmd=enum_command;

	if (command.cmd==CHESS_GET_MOVES) {
		if (parserCoordinates(arg2)) {
			command.from[0]=arg2[1];
			command.from[1]=arg2[3];
		}
		else { //input not like the pattern <x,y>
			command.cmd=CHESS_INVALID;
		}
	}
	else if (command.cmd==CHESS_SAVE) {
		if (arg2==NULL) {
			command.cmd=CHESS_INVALID;
		}
		else {
			strcpy(command.path, arg2);
		}
	}

	else if (command.cmd==CHESS_MOVE) {
		if (arg3==NULL) {
			command.cmd=CHESS_INVALID;
		}
		else if (!strcmp(arg3, "to") && parserCoordinates(arg2) && parserCoordinates(arg4)) { //parserCoordinates will return false if arg2 or arg4==NULL
				command.from[0]=arg2[1];
				command.from[1]=arg2[3];
				command.to[0]=arg4[1];
				command.to[1]=arg4[3];
		}
		else { //input not like the pattern move <x,y> to <w,z>
			command.cmd=CHESS_INVALID;
		}
	}
	return command;
}

void printCHESSCOMMAND(CHESSCommand command) {
	if (command.cmd==CHESS_GAME_MODE) {
		printf("chess game mode\n");
	}
	if (command.cmd==CHESS_DIFFICULTY) {
			printf("chess difficulty\n");
	}
	if (command.cmd==CHESS_USER_COLOR) {
			printf("chess user color\n");
	}
	if (command.cmd==CHESS_LOAD) {
			printf("chess load\n");
	}
	if (command.cmd==CHESS_DEFAULT) {
			printf("chess default\n");
	}
	if (command.cmd==CHESS_PRINT_SETTINGS) {
			printf("chess print settings\n");
	}
	if (command.cmd==CHESS_QUIT) {
			printf("chess quit\n");
	}
	if (command.cmd==CHESS_START) {
			printf("chess start\n");
	}
	if (command.cmd==CHESS_MOVE) {
			printf("chess move\n");
	}
	if (command.cmd==CHESS_GET_MOVES) {
			printf("chess get_moves\n");
	}
	if (command.cmd==CHESS_SAVE) {
			printf("chess save\n");
	}
	if (command.cmd==CHESS_UNDO) {
			printf("chess undo\n");
	}
	if (command.cmd==CHESS_RESET) {
			printf("chess reset\n");
	}
	if (command.cmd==CHESS_INVALID) {
			printf("chess invalid\n");
	}
	if (command.cmd==CHESS_LOAD|| command.cmd==CHESS_SAVE){
		printf("path: %s\n", command.path);
	 }
	if (command.cmd==CHESS_GET_MOVES|| command.cmd==CHESS_MOVE) {
		printf("from: <%c,%c>\n", command.from[0],command.from[1]);
	}
	if (command.cmd==CHESS_MOVE){
		printf("to: <%c,%c>\n", command.to[0],command.to[1]);
	}
	if (command.cmd==CHESS_GAME_MODE || command.cmd==CHESS_DIFFICULTY || command.cmd==CHESS_USER_COLOR) {
		printf("number: %d\n", command.number);
	}

}
