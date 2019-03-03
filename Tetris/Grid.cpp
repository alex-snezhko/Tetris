#include "stdafx.h"
#include "Grid.h"

bool Grid::grid[HEIGHT][WIDTH];

void Grid::checkForCompletedRow()
{
	int lowestRow = Grid::HEIGHT;
	int numRows = 0;

	bool rowCompleted;
	for (int i = 0; i < HEIGHT; i++)
	{
		rowCompleted = true;
		for (int j = 0; j < WIDTH; j++)
		{
			if (!grid[i][j])
			{
				rowCompleted = false;
			}
		}
		if (rowCompleted)
		{
			numRows++;
			if (lowestRow >= i)
			{
				lowestRow = i;
			}
			for (int j = 0; j < WIDTH; j++)
			{
				grid[i][j] = false;
			}
		}
	}

	if (numRows > 0)
	{
		shiftDown(lowestRow, numRows);
	}
}

void Grid::shiftDown(int lowestRow, int numRows)
{
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			if ((lowestRow + numRows + j) >= Grid::HEIGHT)
			{
				grid[lowestRow + j][i] = false;
			}
			else
			{
				grid[lowestRow + j][i] = grid[lowestRow + numRows + j][i];
			}
		}
	}
}
