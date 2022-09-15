#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "RenderWindow.hpp"
#include "Constants.hpp"
#include "Tile.hpp"


using std::cout;
using std::endl;


float heuristic(Tile* tile, Tile* end) {//different heuristics will give different results by prioritising different paths
	float dist = sqrt(pow(tile->row - end->row, 2)+ pow(tile->col - end->col, 2)); //get distance using pythagoras
	//float dist = abs(end->row - tile->row) + abs(end->col - tile->col);
	return dist;
}



int getBestTile(std::vector<Tile*> openSet) {//gets the tile with the lowest score
	int bestTile = 0;
	for (int i = 0; i < openSet.size(); i++) {
		if (openSet[i]->f < openSet[bestTile]->f) {
			bestTile = i;
		}
	}

	return bestTile;
}


//checks if a tile is in a vector, slower than using a map but itterating through to draw is faster
//minimial difference for small grids but a map could be used for faster performance on larger grids, especially if not being drawn in the same loop

bool inVector(std::vector<Tile*> v, Tile* tile) {
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == tile) {
			return true;
		}
	}
	return false;
}

std::vector<Tile*> getPath(Tile* p_tile) {//retraces the path back to the start
	std::vector<Tile*> path;

	Tile* temp = p_tile;
	while (temp != NULL) {
		path.push_back(temp);
		temp = temp->prev;
	}

	return path;
}

int main(int argc, char* argv[]) {
	srand(time(NULL)); //set random seed

	//initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "SDL Failed To Initialise. Error: " << SDL_GetError() << endl;
	}
	if (!(IMG_Init(IMG_INIT_PNG))) {
		cout << "SDL Image Failed To Initialise. Error; " << SDL_GetError() << endl;
	}


	RenderWindow window("A* Maze Solver", SCREENWIDTH, SCREENHEIGHT);

	SDL_Event event;


	std::vector<std::vector<Tile>> tiles;//create 2D vector of tiles
	tiles.resize(GRIDROWS);
	for (int i = 0; i < GRIDROWS; i++) {//rows
		tiles[i].resize(GRIDCOLS);//resize vector
		for (int j = 0; j < GRIDCOLS; j++){//cols
			tiles[i][j] = Tile(i, j);
		}
	}

	//set start and end points and ensure that they are not walls
	Tile* start = &tiles[0][0];
	Tile* end = &tiles[GRIDROWS-1][GRIDCOLS-1];
	start->isWall = false;
	end->isWall = false;

	std::vector<Tile*> openSet;
	openSet.push_back(start);
	start->g = 0;
	start->f = heuristic(start, end);

	bool finished = true;
	std::vector<Tile*> path;
	
	//variables for limiting frame rate
	Uint64 frameStart;
	int frameTime;
	const int frameDelay = 16;
	
	//output controls
	cout << "Controls:\n  Left CLick: Place Wall\n  Right Click: Remove Wall\n  Space: Start\n  ESC: Clear Path\n  Backspace: Randomise Walls" << endl;


	bool running = true;
	while (running) {
		frameStart = SDL_GetTicks64();
		while (SDL_PollEvent(&event)) {//event handler
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) { //start algorithm
					finished = false;
					//reset the state of the board
					for (int i = 0; i < GRIDROWS; i++) {//rows
						for (int j = 0; j < GRIDCOLS; j++) {//cols
							tiles[i][j].g = INFINITY;
							tiles[i][j].f = INFINITY;
							tiles[i][j].h = INFINITY;
							tiles[i][j].prev = NULL;
							
						}
					}
					start->g = 0;
					//reset open set
					openSet.clear();
					openSet.push_back(start);
					path.clear();//clear path
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {//clear path and open set so the board looks clear
					openSet.clear();
					path.clear();
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && finished) {//ransomise the walls
					openSet.clear();
					path.clear();

					for (int i = 0; i < tiles.size(); i++) {
						for (int j = 0; j < tiles[0].size(); j++) {
							if (std::rand() % 100 < WALLCHANCE) {
								tiles[i][j].isWall = true;
							}
							else {
								tiles[i][j].isWall = false;
							}
						}
					}
					//make sure start and end are not walls
					start->isWall = false;
					end->isWall = false;
				}
				break;
			}
		}

		if (finished) {//allow editing of the map with left and right click if path is found
			int mX, mY;
			Uint32 mouseState = SDL_GetMouseState(&mX, &mY);
			if (mouseState == 1) { //left click to add walls
				mX /= TILEWIDTH;
				mY /= TILEHEIGHT;
				if (mX < GRIDCOLS && mY < GRIDROWS) {
					tiles[(int)mY][(int)mX].isWall = true;
				}
			}
			else if (mouseState == 4) { //right click to remove walls
				mX /= TILEWIDTH;
				mY /= TILEHEIGHT;
				if (mX < GRIDCOLS && mY < GRIDROWS) {
					tiles[(int)mY][(int)mX].isWall = false;
				}
			}
			
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		//process algorithm
		//if statement used instead of while loop so each step of the algorithm can be displayed visually
		if (!finished) {
			if (openSet.size() > 0) {
				int current = getBestTile(openSet);//get the tile with the lowest f score
				Tile* currentTile = openSet[current];
				//check if current tile is the end goal
				if (currentTile == end) {
					cout << "Path Found" << endl;
					finished = true;
					//path = getPath(currentTile);

				}
				path = getPath(currentTile);
				//remove current item from open set and get neighbours
				openSet.erase(openSet.begin() + current);


				int i = currentTile->row;
				int j = currentTile->col;

				std::vector<Tile*> neighbours;
				//get all neighbours
				if (i > 0) {
					neighbours.push_back(&tiles[i - 1][j]);
				}
				if (i < tiles.size() - 1) {
					neighbours.push_back(&tiles[i + 1][j]);

				}
				if (j > 0) {
					neighbours.push_back(&tiles[i][j - 1]);
				}
				if (j < tiles[0].size() - 1) {
					neighbours.push_back(&tiles[i][j + 1]);
				}
				//option for diagonals, comment out to remove. also recommended to use phythagoras heuristic over manhattan for diagonals
				//also checks to see if the diagonal is blocked by walls, so it cannot move between a diagonal gap
				if (i > 0 && j > 0) {//up left
					if (!(tiles[i - 1][j].isWall && tiles[i][j - 1].isWall)) {//cehck if diagonals are walls
						neighbours.push_back(&tiles[i - 1][j - 1]);
					}
				}
				if (i < tiles.size()-1 && j > 0) { // down left
					if (!(tiles[i + 1][j].isWall && tiles[i][j - 1].isWall)) {
						neighbours.push_back(&tiles[i + 1][j - 1]);
					}
				}
				if (i > 0 && j < tiles[0].size() - 1) { //up right
					if (!(tiles[i - 1][j].isWall && tiles[i][j + 1].isWall)) {
						neighbours.push_back(&tiles[i - 1][j + 1]);
					}
				}
				if (i < tiles.size() - 1 && j < tiles[0].size()-1) { //down right
					if (!(tiles[i + 1][j].isWall && tiles[i][j + 1].isWall)) {
						neighbours.push_back(&tiles[i + 1][j + 1]);
					}
				}



				for (int i = 0; i < neighbours.size(); i++) {//itterate through each neighbour
					float tempGScore = currentTile->g + 1;//current tile score + distance to next tile(always 1 in this case)

					//set new g score and f score if the new g score is lower, as it means we got here faster, new tiles have infinity for score
					//so this will always show as getting there faster
					if (tempGScore < neighbours[i]->g && !neighbours[i]->isWall) {
						neighbours[i]->g = tempGScore;
						neighbours[i]->f = tempGScore + heuristic(neighbours[i], end);
						neighbours[i]->prev = currentTile; // add path

						//add neighbour to open set if it is not already in it
						if (!inVector(openSet, neighbours[i])) {
							openSet.push_back(neighbours[i]);
						}
					}
				}
			}
			else {
				finished = true; //algorithm is finished if there is nothing in open set
				cout << "No Path" << endl;
			}
		}

		//-------------------------------------------------------------------------------------------------------------------------------------
		

		//comment out this draw code if you want to see how fast it runs without having to render every step
		//or change if statemnt to while loop for algorithm
		

		//clear screen
		window.clear();


		//draw to screen
		
		//draw tiles
		for (int i = 0; i < GRIDROWS; i++) {//rows
			for (int j = 0; j < GRIDCOLS; j++) {//cols
				if (tiles[i][j].isWall) {
					window.fillRect(tiles[i][j].col * TILEWIDTH, tiles[i][j].row * TILEHEIGHT, TILEWIDTH, TILEHEIGHT, SDL_Color{ 0, 0, 0, 255 });
				}
				else {
					window.drawRect(tiles[i][j].col * TILEWIDTH, tiles[i][j].row * TILEHEIGHT, TILEWIDTH, TILEHEIGHT, SDL_Color{ 0, 0, 0, 255 });
				}
			}
		}


		//draw open set
		for (int i = 0; i < openSet.size(); i++) {
			window.fillRect(openSet[i]->col * TILEWIDTH, openSet[i]->row * TILEHEIGHT, TILEWIDTH, TILEHEIGHT, SDL_Color{0,255,0,255});
		}

		//draw start and end points
		window.fillRect(start->col * TILEWIDTH, start->row * TILEHEIGHT, TILEWIDTH, TILEHEIGHT, SDL_Color{ 255, 0, 255, 255 });
		window.fillRect(end->col * TILEWIDTH, end->row * TILEHEIGHT, TILEWIDTH, TILEHEIGHT, SDL_Color{ 255, 255, 0, 255 });

		//draw path
		for (int i = 0; i < path.size(); i++) {
			window.fillRect(path[i]->col * TILEWIDTH+1, path[i]->row * TILEHEIGHT+1, TILEWIDTH-2, TILEHEIGHT-2, SDL_Color{ 0,0,255,255 });
		}

		

		//update screen
		window.display();


		//FPS
		//can be commented out to allow algorithm to run at full speed, add back to limit speed for viewing purposes
		/*
		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay) {
			SDL_Delay(frameDelay - frameTime);
		}
		*/
		
	}

	//destroy window and quit SDL
	window.cleanUp();
	SDL_Quit();

	return 0;
}