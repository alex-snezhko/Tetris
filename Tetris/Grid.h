#pragma once
#include "Blocks.h"

class Grid
{
	Grid();
public:
	static const int WIDTH = 10;
	static const int HEIGHT = 20;

	static bool grid[WIDTH][HEIGHT];
};
