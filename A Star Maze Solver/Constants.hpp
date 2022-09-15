#pragma once
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
//global constants, change the number of rows and columns as well as wall chance for different results

const int SCREENWIDTH = 500;
const int SCREENHEIGHT = 500;

const int GRIDROWS = 50;
const int GRIDCOLS = 50;

const int TILEWIDTH = SCREENWIDTH / GRIDROWS;
const int TILEHEIGHT = SCREENHEIGHT / GRIDCOLS;

const int WALLCHANCE = 30;

#endif