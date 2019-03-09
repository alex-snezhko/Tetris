#pragma once
#include "Blocks.h"

struct GridTile
{
	bool occupied;
	COLORREF color;

	GridTile()
	{
		occupied = false;
		color = RGB(255, 255, 255);
	}
};

class Grid
{
	Grid();

	static void checkForCompletedRow();

	static void shiftDown(int lowestRow, int numRows);

public:
	static const int WIDTH = 10;
	static const int HEIGHT = 20;

	static GridTile grid[HEIGHT][WIDTH];
};
