#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) {
	//initialise window
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << "Window Failed To Initialise. Error: " << SDL_GetError() << endl;
	}

	//initialise renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		cout << "Renderer Failed To Initialise. Error: " << SDL_GetError() << endl;
	}
}


SDL_Texture* RenderWindow::loadTexture(const char* p_fileName) {
	SDL_Texture* texture = NULL;
	//load texture and test to see if it loaded
	texture = IMG_LoadTexture(renderer, p_fileName);
	if (texture == NULL) {
		cout << "Failed To Load Texture. Error: " << SDL_GetError() << endl;
	}

	return texture;

}

void RenderWindow::clear() {
	//clear the screen and set the background colour to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(SDL_Texture* p_texture, int p_x, int p_y) {
	//get source (which part of the file is used, in this case it is the whole image so the whole texture is used)
	SDL_Rect src;
	src.x = 0;
	src.y = 0;

	SDL_QueryTexture(p_texture, NULL, NULL, &src.w, &src.h);//get size of texture 

	//set destination
	SDL_Rect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = src.w;
	dst.h = src.h;

	//copy texture to the screen
	SDL_RenderCopy(renderer, p_texture, &src, &dst);

}

void RenderWindow::fillRect(int p_x, int p_y, int p_w, int p_h, SDL_Color p_colour) {
	//create SDL_Rect with pos and size
	SDL_Rect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = p_w;
	dst.h = p_h;

	//set colour and draw the rectangle to the screen
	SDL_SetRenderDrawColor(renderer, p_colour.r, p_colour.g, p_colour.b, p_colour.a);//set the draw colour
	SDL_RenderFillRect(renderer, &dst); //draw the rect
	
	
}

void RenderWindow::drawRect(int p_x, int p_y, int p_w, int p_h, SDL_Color p_colour) { //draw a hollow rectangle
	//create SDL_Rect with pos and size
	SDL_Rect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = p_w;
	dst.h = p_h;

	//set colour and draw the rectangle to the screen
	SDL_SetRenderDrawColor(renderer, p_colour.r, p_colour.g, p_colour.b, p_colour.a);//set the draw colour
	SDL_RenderDrawRect(renderer, &dst); //draw the rect


}

void RenderWindow::display() {//update screen
	SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}