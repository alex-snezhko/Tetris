#include "stdafx.h"
#include "Grid.h"

GridTile Grid::grid[HEIGHT][WIDTH];

int Grid::findNumCompletedRows()
{
	int numRows = 0;

	for (int i = 0; i < HEIGHT; i++)
	{
		int blocksInRow = 0;
		for (int j = 0; j < WIDTH; j++)
		{
			if (grid[i][j].occupied)
			{
				blocksInRow++;
			}
		}
		if (blocksInRow == WIDTH)
		{
			numRows++;
		}
	}

	return numRows;
}

// returns null-character-terminated array of rows complete
int* Grid::findCompletedRows()
{
	//std::vector<int> rowsComplete;
	int* rowsComplete = new int[Shape::NUM_BLOCKS + 1];
	unsigned int arrSize = 0;

	for (int i = 0; i < HEIGHT; i++)
	{
		int blocksInRow = 0;
		for (int j = 0; j < WIDTH; j++)
		{
			if (grid[i][j].occupied)
			{
				blocksInRow++;
			}
		}
		if (blocksInRow == WIDTH)
		{
			rowsComplete[arrSize] = i;
			arrSize++;
		}
	}

	return rowsComplete;
}

void Grid::clearRows()
{
	int* rowsComplete = findCompletedRows();

	int numRows = findNumCompletedRows();

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (rowsComplete[i] + 1 < HEIGHT)
			{
				for (int k = rowsComplete[i]; k < HEIGHT - 1; k++)
				{
					grid[k][j].occupied = grid[k + 1][j].occupied;
					grid[k][j].color = grid[k + 1][j].color;
				}
			}
			else
			{
				grid[rowsComplete[i]][j].occupied = false;
				grid[rowsComplete[i]][j].color = RGB(255, 255, 255);
			}
		}

		// Accounts for blocks shifting down
		for (int j = 0; j < numRows; j++)
		{
			rowsComplete[j]--;
		}
	}

	delete[] rowsComplete;
}
