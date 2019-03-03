#pragma once
#include "Blocks.h"

class Grid
{
	Grid();

	static void checkForCompletedRow();

	static void shiftDown(int lowestRow, int numRows);

public:
	static const int WIDTH = 10;
	static const int HEIGHT = 20;

	static bool grid[HEIGHT][WIDTH];
};
