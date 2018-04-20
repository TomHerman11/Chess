#ifndef BUTTON_H_
#define BUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Widget.h"

typedef struct button_t Button;
struct button_t {
	SDL_Renderer* render;
	SDL_Texture* texture;
	SDL_Rect location;
	void (*action)(void*);
	void (*action2)(void*);
	int saveloadNumber; //used to "remember" to what file each button of save/load is connected. used in GuiSaveLoadWindow
};

Widget* createButton(
		SDL_Renderer* renderer,
		const char* image,
		SDL_Rect location,
		void (*action)(void*),
		void (*action2)(void*), //second action function used for mousewheel movement
		int isMouseButtonUp,
		bool is_transparent,
		int red,
		int green,
		int blue);
		//isMouseButtonUp==1 means that handleEvent will use the mousebuttuonup function.
		//isMouseButtonUp==2 means that handleEvent will use the mousewheel function.

void destroyButton(Widget* src);
void handleButtonEvent(Widget* src, SDL_Event* event, void* arg);
void handleButtonEventWheel(Widget* src, SDL_Event* e, void* arg);
void drawButton(Widget*, SDL_Renderer*);

#endif /* BUTTON_H_ */
