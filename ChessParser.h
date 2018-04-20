/*
 * Parser.h
 *
 *  Created on: Feb 25, 2018
 *      Author: krimolovsky
 */

#ifndef CHESSPARSER_H_
#define CHESSPARSER_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COMMAND_LENGTH 2000

//a type used to represent a command
typedef enum {
	CHESS_GAME_MODE,
	CHESS_DIFFICULTY,
	CHESS_USER_COLOR,
	CHESS_LOAD,
	CHESS_DEFAULT,
	CHESS_PRINT_SETTINGS,
	CHESS_QUIT,
	CHESS_START,
	CHESS_MOVE,
	CHESS_GET_MOVES,
	CHESS_SAVE,
	CHESS_UNDO,
	CHESS_RESET,
	CHESS_INVALID,
} CHESS_COMMAND;


//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	CHESS_COMMAND cmd;
	char path[COMMAND_LENGTH];
	char to[2];
	char from[2];
	int number;
} CHESSCommand;


/*
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool parserIsInt(const char* str);

/*
 * this function receives a string and by comparing the string meaning, it returns the appropriate enum_command value.
 *
 * @returns
 * the enum_command that cmd is it's name
 * if the string is not a valid name of enum_command, CHESS_INVALID
 */
CHESS_COMMAND spParserPraseLineHelper(char *cmd);

/*
 *this function is called from the settings window in the game.
 *@returns
 *the command that was received from the user.
 *if the command is invalid, then CHESSCommand is INVALID.
 */
CHESSCommand parserSettingsParseLine(const char* str);

/*
 * this function receives a string and by comparing the string meaning, it returns the appropriate enum_command value.
 *
 * @returns
 * the enum_command that cmd is it's name
 * if the string is not a valid name of enum_command, CHESS_INVALID
 */
CHESS_COMMAND parserGameParseLineHelper(char* arg1);

/*
 *this function is called from the game window in the game.
 *@returns
 *the command that was received from the user.
 *if the command is invalid, then CHESSCommand is INVALID.
 */
CHESSCommand parserGameParseLine(const char* str);

/*
 * checks if arg2 is like the pattern <x,y>, for x and y KOLSHEHEM
 */
bool parserCoordinates(char *arg2);

void printCHESSCOMMAND(CHESSCommand command);

#endif /* CHESSPARSER_H_ */
