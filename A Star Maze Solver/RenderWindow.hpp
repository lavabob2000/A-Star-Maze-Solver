#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using std::cout;
using std::endl;

class RenderWindow {
public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_fileName);
	void clear();
	void render(SDL_Texture* p_texture, int p_x, int p_y);
	void fillRect(int p_x, int p_y, int p_w, int p_h, SDL_Color p_colour);
	void drawRect(int p_x, int p_y, int p_w, int p_h, SDL_Color p_colour);
	void display();
	void cleanUp();

private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
};