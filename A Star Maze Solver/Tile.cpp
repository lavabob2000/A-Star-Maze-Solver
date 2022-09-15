#include "Tile.hpp"

Tile::Tile(int p_row, int p_col):row(p_row), col(p_col) {
	
	f = 100;
	g = 100;
	h = 100;
	prev = NULL;
	if (std::rand()%100 < WALLCHANCE) {
		isWall = true;
		
	}
}

Tile::Tile() {
	//default constructor function
	row = 0;
	col = 0;
	f = 100;
	g = 100;
	h = 100;
	prev = NULL;
	if (std::rand()%100 < WALLCHANCE) {
		isWall = true;
	}
}