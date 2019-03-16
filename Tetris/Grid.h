#pragma once
#include "Blocks.h"
#include <vector>

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

public:
	static const int WIDTH = 10;
	static const int HEIGHT = 20;

	static GridTile grid[HEIGHT][WIDTH];

	static int findNumCompletedRows();
	static int* findCompletedRows();
	static void clearRows();
};
