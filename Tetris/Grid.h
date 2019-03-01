#pragma once
#include "Blocks.h"

class Grid
{
public:
	static const int WIDTH = 10;
	static const int HEIGHT = 22;

	static bool grid[WIDTH][HEIGHT];

	static void updateGrid(bool oldPos[], bool newPos[]);

	static void placeBlock(Block b);
	
private:
	Grid();
};
