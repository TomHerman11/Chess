/*
 * main.c
 *
 *  Created on: Mar 19, 2018
 *      Author: tomhe
 */

#include "ConsoleMain.h"
#include "GuiMain.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc==1) { //no special arguments, default play is set.
		consoleMain();
	}

	else if (argc==2 && !strcmp(argv[1], "-c")) {
		consoleMain();
	}

	else if (argc==2 && !strcmp(argv[1], "-g")) {
		GuiMain();
	}

	return 0;
}
