#include "Button.h"

Widget* createButton(
	SDL_Renderer* renderer,
	const char* image,
	SDL_Rect location,
	void (*action)(void*),
	void (*action2)(void*),
	int isMouseButtonUp,
	bool is_transparent,
	int red,
	int green,
	int blue)
{

	// allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	if (res == NULL) {
		//printf("%s ### widget malloc\n", image);
		return NULL;
	}

	Button* data = (Button*) malloc(sizeof(Button));
	if (data == NULL) {
		free(res);
		//printf("%s ### button malloc\n", image);
		return NULL;
	}

	// we use the surface as a temp var
	SDL_Surface* surface = SDL_LoadBMP(image);
	if (surface == NULL) {
		free(res);
		free(data);
	//	printf("%s ### surf\n", image);
		return NULL;
	}

	// set a specific color (white: 255,255,255) to transparent
	if (is_transparent) {
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, red, green, blue));
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(surface);
		printf("%s ### texture\n", image);
		return NULL ;
	}

	// surface is not needed after texture is created
	SDL_FreeSurface(surface);

	// store button & widget details
	data->texture = texture;
	data->location = location;
	data->action = action;
	data->action2=action2;
	res->destroy = destroyButton;
	res->draw = drawButton;
	res->data = data;
	if (isMouseButtonUp==1) res->handleEvent=handleButtonEvent;
	if (isMouseButtonUp==2) res->handleEvent=handleButtonEventWheel;
	return res;
}

void destroyButton(Widget* src)
{
	Button* button = (Button*) src->data;
	SDL_DestroyTexture(button->texture);
	free(button);
	free(src);
}

void handleButtonEvent(Widget* src, SDL_Event* e, void* arg)
{
	if (e->type == SDL_MOUSEBUTTONUP) {
		Button* button = (Button*) src->data;
		SDL_Point point = { .x = e->button.x, .y = e->button.y };

		if (SDL_PointInRect(&point, &button->location)) {
			button->action(arg);
		}
	}
}

void handleButtonEventWheel(Widget* src, SDL_Event* e, void* arg)
{
	if (e->type == 	SDL_MOUSEWHEEL) {
		Button* button = (Button*) src->data;
		if (e->wheel.y==1) { //scroll up
			button->action(arg);
		}
		else if (e->wheel.y==-1) { //scroll down
			button->action2(arg);
		}
	}
}


void drawButton(Widget* src, SDL_Renderer* render)
{
	Button* button = (Button*) src->data;
	SDL_RenderCopy(render, button->texture, NULL, &button->location);
}
