#pragma once
#include <iostream>
#include <math.h>

#include "Constants.hpp"


class Tile {
public:
	Tile(int p_row, int p_col);
	Tile();

	int row;
	int col;
	float f; //total cost
	float g; //distance from start
	float h; //heuristic
	Tile* prev; //path
	bool isWall = false;

private:

};