#include "stdafx.h"
#include "Blocks.h"
#include "Grid.h"

Block::Block()
{
	xPos = 0;
	yPos = 0;
}

Block::Block(int x, int y)
{
	xPos = x;
	yPos = y;
}

void Block::moveTo(int x, int y)
{
	Grid::grid[xPos][yPos] = false;
	Grid::grid[x][y] = true;

	xPos = x;
	yPos = y;
}

class Shape
{
protected:
	static const int NUM_BLOCKS = 4;
	
	// Will point to an array of 4 blocks
	Block* blocks; 

	// Whether or not the shape can move to the given position
	bool canChangePositions(Block* newPositions)
	{
		// Temporary variable representing current positions of grid
		bool gridPositions[Grid::WIDTH][Grid::HEIGHT];
		for (int i = 0; i < Grid::WIDTH; i++)
		{
			for (int j = 0; j < Grid::HEIGHT; j++)
			{
				gridPositions[i][j] = Grid::grid[i][j];
			}
		}

		// Temporarily makes current positions false
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			Grid::grid[blocks[i].xPos][blocks[i].yPos] = false;
		}

		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			// Checks if new blocks are outside the boundaries of the board or touching another block
			if (newPositions[i].xPos < 0 || newPositions[i].xPos >= Grid::WIDTH 
				|| newPositions[i].yPos < 0 || newPositions[i].yPos >= Grid::HEIGHT
				|| Grid::grid[newPositions[i].xPos][newPositions[i].yPos])
			{
				return false;
			}
		}

		// Resets grid positions
		for (int i = 0; i < Grid::WIDTH; i++)
		{
			for (int j = 0; j < Grid::HEIGHT; j++)
			{
				Grid::grid[i][j] = gridPositions[i][j];
			}
		}
		return true;
	}
	
	Shape(Block* b)
	{
		blocks = new Block[NUM_BLOCKS];
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			blocks[i] = b[i];
		}
		delete[] b;
	}

	~Shape()
	{
		delete[] blocks;
	}

	virtual Block* pivotToArr(Block pivot) = 0;

	virtual Block* getPositionsAfterRotation() = 0;

	void changePositions(Block* newPositions)
	{
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			blocks[i].moveTo(newPositions[i].xPos, newPositions[i].yPos);
		}
	}

public:
	static const int COLOR = 0;

	void move(Direction d)
	{
		Block* newPositions = new Block[4];

		switch (d)
		{
		case left:
			for (int i = 0; i < NUM_BLOCKS; i++)
			{
				newPositions[i] = Block(blocks[i].xPos - 1, blocks[i].yPos);
			}
			if (!canChangePositions(newPositions)) return;
			break;
		case right:
			for (int i = 0; i < NUM_BLOCKS; i++)
			{
				newPositions[i] = Block(blocks[i].xPos + 1, blocks[i].yPos);
			}
			if(!canChangePositions(newPositions)) return;
			break;
		case down:
			for (int i = 0; i < NUM_BLOCKS; i++)
			{
				newPositions[i] = Block(blocks[i].xPos, blocks[i].yPos - 1);
			}
			if (!canChangePositions(newPositions))
			{
				delete this;
				return;
			}
			break;
		}
		changePositions(newPositions);
		delete[] newPositions;	
	}

	// Try to rotate shape, wall kick if needed
	virtual void rotate()
	{
		Block* newPositions = getPositionsAfterRotation();

		// Attempts to rotate
		if (canChangePositions(newPositions))
		{
			changePositions(newPositions);
			delete[] newPositions;
			return;
		}

		// Attempting wall kick to the right
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[i].xPos + 1, blocks[i].yPos);
		}
		if (canChangePositions(newPositions))
		{
			changePositions(newPositions);
			delete[] newPositions;
			return;
		}

		// Attempting wall kick to the left
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[i].xPos - 1, blocks[i].yPos);
		}
		if (canChangePositions(newPositions))
		{
			changePositions(newPositions);
			delete[] newPositions;
			return;
		}
	}
	
};





class Line : public Shape
{
	// Used to determine where to put pivot point for rotation; line is flat if even, tall if odd
	int rotationCount;

	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = Block(pivot.xPos - 1, pivot.yPos);
		arr[1] = pivot;
		arr[2] = Block(pivot.xPos + 1, pivot.yPos);
		arr[3] = Block(pivot.xPos + 2, pivot.yPos);
		return arr;
	}

public:
	Line(Block pivot) : Shape(pivotToArr(pivot)) { rotationCount = 0; }

	Block* getPositionsAfterRotation() override
	{
		Block* newPositions = new Block[NUM_BLOCKS];
		
		switch (rotationCount)
		{
		case 0:
			for (int i = 0; i < 4; i++)
			{
				newPositions[i] = Block(blocks[1].xPos + 1, blocks[1].yPos + 1 - i);
			}
			break;
		case 1:
			for (int i = 0; i < 4; i++)
			{
				newPositions[i] = Block(blocks[1].xPos + 1 - i, blocks[1].yPos - 1);
			}
			break;
		case 2:
			for (int i = 0; i < 4; i++)
			{
				newPositions[i] = Block(blocks[1].xPos - 1, blocks[1].yPos - 1 + i);
			}
			break;
		case 3:
			for (int i = 0; i < 4; i++)
			{
				newPositions[i] = Block(blocks[1].xPos - 1 - i, blocks[1].yPos + 1);
			}
			break;
		default:
			rotationCount = 0;
			break;
		}
	}
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
	}

public:
	Square(Block pivot) : Shape(pivotToArr(pivot)) {}
	void rotate() override {}
};


// Shape that can be placed in a 3x3 grid
class RadialShape : public Shape
{
public:
	RadialShape(Block* b) : Shape(b) {}

	Block* getPositionsAfterRotation() override
	{
		Block* newPositions = new Block[NUM_BLOCKS];
		newPositions[0] = blocks[0];
		for (int i = 1; i < NUM_BLOCKS; i++)
		{
			switch (blocks[i].xPos - blocks[0].xPos)
			{
			case 1:
				switch (blocks[i].yPos - blocks[0].yPos)
				{
				case 1:
					newPositions[i] = Block(blocks[0].xPos + 1, blocks[0].yPos - 1);
					break;
				case 0:
					newPositions[i] = Block(blocks[0].xPos, blocks[0].yPos - 1);
					break;
				case -1:
					newPositions[i] = Block(blocks[0].xPos - 1, blocks[0].yPos - 1);
					break;
				}
			case 0:
				switch (blocks[i].yPos - blocks[0].yPos)
				{
				case 1:
					newPositions[i] = Block(blocks[0].xPos + 1, blocks[0].yPos);
					break;
				case -1:
					newPositions[i] = Block(blocks[0].xPos - 1, blocks[0].yPos);
					break;
				}
			case -1:
				switch (blocks[i].yPos - blocks[0].yPos)
				{
				case 1:
					newPositions[i] = Block(blocks[0].xPos + 1, blocks[0].yPos + 1);
					break;
				case 0:
					newPositions[i] = Block(blocks[0].xPos, blocks[0].yPos + 1);
					break;
				case -1:
					newPositions[i] = Block(blocks[0].xPos - 1, blocks[0].yPos + 1);
					break;
				}
				
			}
		}
		return newPositions;
	}
};



class LShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos - 1, pivot.yPos);
		arr[2] = Block(pivot.xPos + 1, pivot.yPos);
		arr[3] = Block(pivot.xPos + 1, pivot.yPos + 1);
		return arr;
	}
};



class ReverseLShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos - 1, pivot.yPos);
		arr[2] = Block(pivot.xPos - 1, pivot.yPos + 1);
		arr[3] = Block(pivot.xPos + 1, pivot.yPos);
		return arr;
	}
};



class SShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos - 1, pivot.yPos);
		arr[2] = Block(pivot.xPos, pivot.yPos + 1);
		arr[3] = Block(pivot.xPos + 1, pivot.yPos + 1);
		return arr;
	}

public:
	SShape(Block pivot) : RadialShape(pivotToArr(pivot)) {}
};



class ZShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos + 1, pivot.yPos);
		arr[2] = Block(pivot.xPos, pivot.yPos + 1);
		arr[3] = Block(pivot.xPos - 1, pivot.yPos + 1);
		return arr;
	}

public:
	ZShape(Block pivot) : RadialShape(pivotToArr(pivot)) {}
};



class TShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override
	{
		Block* arr = new Block[NUM_BLOCKS];
		arr[0] = pivot;
		arr[1] = Block(pivot.xPos, pivot.yPos + 1);
		arr[2] = Block(pivot.xPos - 1, pivot.yPos);
		arr[3] = Block(pivot.xPos + 1, pivot.yPos);
		return arr;
	}

public:
	TShape(Block pivot) : RadialShape(pivotToArr(pivot)) {}
};