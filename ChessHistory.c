/*
 * ChessHistory.c
 *
 *  Created on: Feb 25, 2018
 *      Author: krimolovsky
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ChessHistory.h"

ChessHistory* createHistoryChess(void) {
	ChessHistory* history_add;

	history_add=(ChessHistory*) calloc(1,sizeof(ChessHistory));
	if (history_add==NULL) {
		return NULL;
	}
	history_add->actualSize=0;
	history_add->maxSize=HISTORY_SIZE;
	return history_add;
}

ChessHistory* copyHistoryChess(ChessHistory* history) {
	ChessHistory* history_copy=NULL;
	if (history==NULL) {
		return NULL;
	}
	history_copy=(ChessHistory*) calloc(1,sizeof(ChessHistory));
	if (history_copy==NULL) {
		return NULL;
	}
	history_copy->actualSize=history->actualSize;
	history_copy->maxSize=history->maxSize;
	for (int i=0; i<HISTORY_SIZE; i++) {
		for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
			history_copy->elements[i][j]=history->elements[i][j];
		}
	}
	return history_copy;
}

void destoryHistoryChess(ChessHistory* history) {
	if (history==NULL) {
		return;
	}
	free(history);
}

void addFirstHistoryChess(ChessHistory* history, char* details) {
	if (history==NULL) {
		return;
	}

	//forwarding all memory elements, one step right
	if (history->actualSize<history->maxSize) { //this means the list is not full
		//now forwarding
		for (int i=history->actualSize-1; i>=0; i--) {
			for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
				history->elements[i+1][j]=history->elements[i][j];
			}
		}
		history->actualSize+=1;
	}

	else if (history->actualSize==history->maxSize) { //in case the list is full, we still want to delete the last element.
		for (int i=(history->actualSize)-2; i>=0; i--) {
			for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
				history->elements[i+1][j]=history->elements[i][j];
			}
		}
	}

	//now inserting the new details to the first element
	for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
		history->elements[0][j]=details[j];
	}
}

void deleteFirstHistoryChess(ChessHistory* history) {
	if (history==NULL) {
		return;
	}

	//now updating the elements
	for (int i=0; i<history->actualSize-1; i++) {
		for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
			history->elements[i][j]=history->elements[i+1][j];
		}
	}
	//anyway we want to put nulls in the last element
	for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
		history->elements[history->actualSize-1][j]='\0';
	}
	history->actualSize-=1;
}

void getFirstHistoryChess(ChessHistory* history, char *details) {
	for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
		details[j]=history->elements[0][j];
	}
}

char* getFirstHistoryChess2(ChessHistory* history) {
	char* details=(char *)calloc(NUMBER_OF_DETAILS_FOR_UNDO, sizeof(char));
	for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
		details[j]=history->elements[0][j];
	}
	return details;
}


bool isEmptyHistoryChess(ChessHistory* history) {
	if (history->actualSize==0) {
		return true;
	}
	return false;
}

void PrintHistoryElements(ChessHistory* history) {
	for (int i=0; i<HISTORY_SIZE; i++) {
		for (int j=0; j<NUMBER_OF_DETAILS_FOR_UNDO; j++) {
			printf("%c, ", history->elements[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
