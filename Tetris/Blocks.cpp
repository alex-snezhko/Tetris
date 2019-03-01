#include "stdafx.h"
#include "Blocks.h"
#include "Grid.h"

Block::Block() {}
Block::Block(int x, int y)
{
	xPos = x;
	yPos = y;
}
void Block::changePosition(int x, int y)
{
	xPos = x;
	yPos = y;
	// TODO Update grid position
}

class Shape
{
protected:
	static const int NUM_BLOCKS = 4;
private:
	Block blocks[NUM_BLOCKS];

	// Whether or not this shape can move in a given direction
	bool canMove(Direction d)
	{
		switch (d)
		{
		case left:
			for (int i = 0; i < NUM_BLOCKS; i++)
			{
				// Checks if block at leftmost position of grid, or is touching another block to the left
				if (blocks[i].xPos == 0 || Grid::grid[blocks[i].xPos - 1][blocks[i].yPos])
				{
					return false;
				}
			}
			break;
		case right:
			for (int i = 0; i < NUM_BLOCKS; i++)
			{
				// Checks if block at rightmost position of grid, or is touching another block to the right
				if (blocks[i].xPos == Grid::WIDTH - 1 || Grid::grid[blocks[i].xPos + 1][blocks[i].yPos])
				{
					return false;
				}
			}
			break;
		case down:
			for (int i = 0; i < NUM_BLOCKS; i++)
			{
				// Checks if block at bottom of grid, or is directly above another object
				if (blocks[i].yPos == 0 || Grid::grid[blocks[i].xPos][blocks[i].yPos - 1])
				{
					return false;
				}
			}
			break;
		}

		return true;
	}
	
protected:
	Shape(Block b[NUM_BLOCKS])
	{
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			blocks[i] = b[i];
		}
	}

	virtual Block* pivotToArr(Block pivot) = 0;

public:
	void move(Direction d)
	{
		if (canMove(d))
		{
			switch (d)
			{
			case left:
				for (int i = 0; i < NUM_BLOCKS; i++)
				{
					blocks[i].changePosition(blocks[i].xPos - 1, blocks[i].yPos);
				}
				break;
			case right:
				for (int i = 0; i < NUM_BLOCKS; i++)
				{
					blocks[i].changePosition(blocks[i].xPos + 1, blocks[i].yPos);
				}
				break;
			case down:
				for (int i = 0; i < NUM_BLOCKS; i++)
				{
					blocks[i].changePosition(blocks[i].xPos, blocks[i].yPos - 1);
				}
				break;
			}
		}
	}

	virtual void rotate() = 0;
};

class Line : public Shape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos - 1, pivot.yPos);
		arr[2] = Block(pivot.xPos + 1, pivot.yPos);
		arr[3] = Block(pivot.xPos + 2, pivot.yPos);
		return arr;
		delete[] arr;
	}

public:
	Line(Block pivot) : Shape(pivotToArr(pivot)) {}
};

class Square : public Shape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos + 1, pivot.yPos);
		arr[2] = Block(pivot.xPos + 1, pivot.yPos + 1);
		arr[3] = Block(pivot.xPos, pivot.yPos + 1);
		return arr;
		delete[] arr;
	}

public:
	Square(Block pivot) : Shape(pivotToArr(pivot)) {}
};

class SShape : public Shape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos - 1, pivot.yPos);
		arr[2] = Block(pivot.xPos, pivot.yPos + 1);
		arr[3] = Block(pivot.xPos + 1, pivot.yPos + 1);
		return arr;
		delete[] arr;
	}

public:
	SShape(Block pivot) : Shape(pivotToArr(pivot)) {}
};

class ZShape : public Shape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos + 1, pivot.yPos);
		arr[2] = Block(pivot.xPos, pivot.yPos + 1);
		arr[3] = Block(pivot.xPos - 1, pivot.yPos + 1);
		return arr;
		delete[] arr;
	}

public:
	ZShape(Block pivot) : Shape(pivotToArr(pivot)) {}
};

class TShape : public Shape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos - 1, pivot.yPos);
		arr[2] = Block(pivot.xPos + 1, pivot.yPos);
		arr[3] = Block(pivot.xPos, pivot.yPos + 1);
		return arr;
		delete[] arr;
	}

public:
	TShape(Block pivot) : Shape(pivotToArr(pivot)) {}
};