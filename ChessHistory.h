/*
 * ChessHistory.h
 *
 *  Created on: Feb 25, 2018
 *      Author: krimolovsky
 */

#ifndef CHESSHISTORY_H_
#define CHESSHISTORY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define HISTORY_SIZE 6
#define NUMBER_OF_DETAILS_FOR_UNDO 5
/*
* {x,y,i,j,symbol}
* <x,y> - first two chars for location before move
* <i,j> - next 2 chars for location after move,
* symbol - last char for what symbol was before move
*/


typedef struct chess_history_t {
	char elements[HISTORY_SIZE][NUMBER_OF_DETAILS_FOR_UNDO];
	int actualSize;
	int maxSize;
} ChessHistory;

/*
 * creates a history list of size HISTORY_SIZE, each element of size NUMBER_OF_DETAILS_FOR_UNDO.
 */
ChessHistory* createHistoryChess(void);

/*
 * copies a history, if history is null then returns null.
 */
ChessHistory* copyHistoryChess(ChessHistory* history);

/*
 * deletes history chess list, releasing resources
 */
void destoryHistoryChess(ChessHistory* history);

/*
 * inserts info to the first elements, EVEN WHEN THE LIST IS FULL (by deleting the last element).
 */
void addFirstHistoryChess(ChessHistory* history, char* details);

/*
 * deletes the first elements, inserts nulls to last element
 */
void deleteFirstHistoryChess(ChessHistory* history);

/*
 * returns the first element (meaning a list of all 5 details).
 * this is the last move details made in the game (from, to where, the symbol was before the move).
 */
void getFirstHistoryChess(ChessHistory* history, char* details);

/*
 * checks if the list is empty, for further use if player want to do undo over 3 times in a row.
 */
bool isEmptyHistoryChess(ChessHistory* history);

void PrintHistoryElements(ChessHistory* history);







#endif /* CHESSHISTORY_H_ */
