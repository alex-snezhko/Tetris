#include "stdafx.h"
#include "Blocks.h"

Block::Block()
{
	xPos = 0;
	yPos = 0;
	color = RGB(0, 0, 0);
}

Block::Block(int x, int y)
{
	xPos = x;
	yPos = y;
	color = RGB(0, 0, 0);
}

Block::Block(int x, int y, COLORREF c)
{
	xPos = x;
	yPos = y;
	color = c;
}

void Block::moveTo(int x, int y)
{
	xPos = x;
	yPos = y;
}





// Whether or not the shape can move to the given position
bool Shape::canChangePositions(Block* newPositions)
{
	// Temporary variable representing grid without current positions
	bool gridPositions[Grid::HEIGHT][Grid::WIDTH];

	for (int i = 0; i < Grid::HEIGHT; i++)
	{
		for (int j = 0; j < Grid::WIDTH; j++)
		{
			gridPositions[i][j] = Grid::grid[i][j].occupied;
		}
	}

	for (int i = 0; i < Shape::NUM_BLOCKS; i++)
	{
		gridPositions[blocks[i].yPos][blocks[i].xPos] = false;
	}

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		// Checks if new blocks are outside the boundaries of the board or touching another block
		if (newPositions[i].xPos < 0 || newPositions[i].xPos >= Grid::WIDTH || newPositions[i].yPos < 0
			|| newPositions[i].yPos < Grid::HEIGHT && gridPositions[newPositions[i].yPos][newPositions[i].xPos])
		{
			return false;
		}
	}

	return true;
}
	
Shape::Shape(Block* b)
{
	rotationPosition = spawn;
	blocks = new Block[NUM_BLOCKS];
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		blocks[i] = b[i];
	}
	delete[] b;
}

Shape::~Shape()
{
	delete[] blocks;
}

void Shape::changePositions(Block* newPositions)
{
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		if (blocks[i].yPos < Grid::HEIGHT)
		{
			Grid::grid[blocks[i].yPos][blocks[i].xPos].occupied = false;
			Grid::grid[blocks[i].yPos][blocks[i].xPos].color = RGB(255, 255, 255);
		}
	}

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		if (newPositions[i].yPos < Grid::HEIGHT)
		{
			Grid::grid[newPositions[i].yPos][newPositions[i].xPos].occupied = true;
			Grid::grid[newPositions[i].yPos][newPositions[i].xPos].color = blocks[i].color;
		}

		blocks[i].moveTo(newPositions[i].xPos, newPositions[i].yPos);
	}
}

Block* Shape::getBlocks()
{
	Block* arr = new Block[NUM_BLOCKS];
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		arr[i] = blocks[i];
	}
	return arr;
}

void Shape::move(Direction d)
{
	Block* newPositions = new Block[NUM_BLOCKS];

	switch (d)
	{
	case left:
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[i].xPos - 1, blocks[i].yPos);
		}
		if (!canChangePositions(newPositions))
		{
			delete[] newPositions;
			return;
		}
		break;
	case right:
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[i].xPos + 1, blocks[i].yPos);
		}
		if (!canChangePositions(newPositions))
		{
			delete[] newPositions;
			return;
		}
		break;
	case down:
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[i].xPos, blocks[i].yPos - 1);
		}
		if (!canChangePositions(newPositions))
		{
			delete[] newPositions;
			throw ShapeAtBottomException();
		}
		break;
	}
	changePositions(newPositions);
	delete[] newPositions;
}

// Try to rotate shape, wall kick if needed
void Shape::rotate(Direction d)
{
	if (d != left && d != right)
	{
		return;
	}

	Block* newPositions = getPositionsAfterRotation(d);

	// If attempting to rotate a block
	if (newPositions == nullptr)
	{
		delete[] newPositions;
		return;
	}

	// Attempts to rotate
	if (canChangePositions(newPositions))
	{
		changePositions(newPositions);
		delete[] newPositions;
		rotationPosition = (RotationPosition)((rotationPosition + d) % 4);
		return;
	}

	Block* postKickPositions = getValidPositionAfterWallkick(newPositions, d);
	delete[] newPositions;

	if (postKickPositions == nullptr)
	{
		return;
	}

	changePositions(postKickPositions);
	delete[] postKickPositions;
	rotationPosition = (RotationPosition)((rotationPosition + d) % 4);
}






Block* Line::pivotToArr(Block pivot)
{
	Block* arr = new Block[NUM_BLOCKS];
	arr[0] = Block(pivot.xPos - 1, pivot.yPos, COLOR);
	arr[1] = Block(pivot.xPos, pivot.yPos, COLOR);
	arr[2] = Block(pivot.xPos + 1, pivot.yPos, COLOR);
	arr[3] = Block(pivot.xPos + 2, pivot.yPos, COLOR);
	return arr;
}

Line::Line(Block pivot) : Shape(pivotToArr(pivot)) {}

Block* Line::getPositionsAfterRotation(Direction d)
{
	Block* newPositions = new Block[NUM_BLOCKS];

	switch (rotationPosition)
	{
	case spawn:
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[1].xPos + 1, blocks[1].yPos + 1 - i, COLOR);
		}
		break;
	case onceCW:
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[1].xPos + 1 - i, blocks[1].yPos - 1, COLOR);
		}
		break;
	case upsideDown:
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[1].xPos - 1, blocks[1].yPos - 1 + i, COLOR);
		}
		break;
	case onceCCW:
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = Block(blocks[1].xPos - 1 + i, blocks[1].yPos + 1, COLOR);
		}
		break;
	}

	return newPositions;
}

// Algorithm to match Tetris SRS standard
Block* Line::getValidPositionAfterWallkick(Block* positions, Direction d)
{
	Block* newPositions = new Block[NUM_BLOCKS];
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		newPositions[i] = positions[i];
	}

	// Used for the purpose of calculations
	int xMod;
	int yMod;
	RotationPosition rotPos = rotationPosition;
	if (d == left)
	{
		rotPos = (RotationPosition)((rotationPosition - 1) % 4);
		if (rotPos == -1)
		{
			rotPos = (RotationPosition)3;
		}
	}

	switch (rotPos)
	{
	case spawn:
		xMod = -1;
		yMod = -1;
		break;
	case onceCW:
		xMod = -1;
		yMod = 1;
		break;
	case upsideDown:
		xMod = 1;
		yMod = 1;
		break;
	case onceCCW:
		xMod = 1;
		yMod = -1;
		break;
	}

	// Position deltas
	int dX;
	int dY;

	const int NUM_TESTS = 4;
	for (int i = 0; i < NUM_TESTS; i++)
	{
		dX = 1;
		dY = 2;

		if ((rotPos + i) % 2 == 0)
		{
			int temp = dX;
			dX = dY;
			dY = temp;
		}
		if (i % 2 == 1)
		{
			dX = -dX;
			dY = -dY;
		}

		dX = dX * xMod * d;
		dY = dY * yMod * d;

		switch (i)
		{
		case 0:
		case 1:
			dY = 0;
			break;
		}

		for (int j = 0; j < NUM_BLOCKS; j++)
		{
			newPositions[j].moveTo(newPositions[j].xPos + dX, newPositions[j].yPos + dY);
		}
		if (canChangePositions(newPositions))
		{
			return newPositions;
		}

		// Resets back to original position if wallkick attempt fails
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = positions[i];
		}
	}

	delete[] newPositions;
	return nullptr;
}





Block* Square::pivotToArr(Block pivot)
{
	Block* arr = new Block[NUM_BLOCKS];
	arr[0] = Block(pivot.xPos, pivot.yPos, COLOR);
	arr[1] = Block(pivot.xPos + 1, pivot.yPos, COLOR);
	arr[2] = Block(pivot.xPos + 1, pivot.yPos + 1, COLOR);
	arr[3] = Block(pivot.xPos, pivot.yPos + 1, COLOR);
	return arr;
}

Square::Square(Block pivot) : Shape(pivotToArr(pivot)) {}





RadialShape::RadialShape(Block* b) : Shape(b) {}

Block* RadialShape::getPositionsAfterRotation(Direction d)
{
	Block* newPositions = new Block[NUM_BLOCKS];
	newPositions[0] = blocks[0];
	for (int i = 1; i < NUM_BLOCKS; i++)
	{
		int x = blocks[0].xPos + ((blocks[i].yPos - blocks[0].yPos) * d);
		int y = blocks[0].yPos - ((blocks[i].xPos - blocks[0].xPos) * d);
		newPositions[i] = Block(x, y);
	}
	return newPositions;
}

// Algorithm to match Tetris SRS standard
Block* RadialShape::getValidPositionAfterWallkick(Block* positions, Direction d)
{
	Block* newPositions = new Block[NUM_BLOCKS];
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		newPositions[i] = positions[i];
	}

	// Used for the purpose of calculations
	int xMod;
	int yMod;
	RotationPosition rotPos = rotationPosition;
	if (d == left)
	{
		rotPos = (RotationPosition)((rotationPosition - 1) % 4);
		if (rotPos == -1)
		{
			rotPos = (RotationPosition)3;
		}
	}

	switch (rotPos)
	{
	case spawn:
		xMod = -1;
		yMod = -1;
		break;
	case onceCW:
		xMod = 1;
		yMod = 1;
		break;
	case upsideDown:
		xMod = 1;
		yMod = -1;
		break;
	case onceCCW:
		xMod = -1;
		yMod = 1;
		break;
	}

	// Position deltas
	int dX;
	int dY;

	const int NUM_TESTS = 4;
	for (int i = 0; i < NUM_TESTS; i++)
	{
		switch (i)
		{
		case 0:
			dX = 1 * xMod * d;
			dY = 0;
			break;
		case 1:
			dX = 1 * xMod * d;
			dY = -1 * yMod * d;
			break;
		case 2:
			dX = 0;
			dY = 2 * yMod * d;
			break;
		case 3:
			dX = 1 * xMod * d;
			dY = 2 * yMod * d;
			break;
		}

		for (int j = 0; j < NUM_BLOCKS; j++)
		{
			newPositions[j].moveTo(newPositions[j].xPos + dX, newPositions[j].yPos + dY);
		}
		if (canChangePositions(newPositions))
		{
			return newPositions;
		}

		// Resets back to original position if wallkick attempt fails
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			newPositions[i] = positions[i];
		}
	}

	delete[] newPositions;
	return nullptr;
}





Block* LShape::pivotToArr(Block pivot)
{
	Block* arr = new Block[NUM_BLOCKS];
	arr[0] = Block(pivot.xPos, pivot.yPos, COLOR);
	arr[1] = Block(pivot.xPos - 1, pivot.yPos, COLOR);
	arr[2] = Block(pivot.xPos + 1, pivot.yPos, COLOR);
	arr[3] = Block(pivot.xPos + 1, pivot.yPos + 1, COLOR);
	return arr;
}

LShape::LShape(Block pivot) : RadialShape(pivotToArr(pivot)) {}





Block* JShape::pivotToArr(Block pivot)
{
	Block* arr = new Block[NUM_BLOCKS];
	arr[0] = Block(pivot.xPos, pivot.yPos, COLOR);
	arr[1] = Block(pivot.xPos - 1, pivot.yPos, COLOR);
	arr[2] = Block(pivot.xPos - 1, pivot.yPos + 1, COLOR);
	arr[3] = Block(pivot.xPos + 1, pivot.yPos, COLOR);
	return arr;
}

JShape::JShape(Block pivot) : RadialShape(pivotToArr(pivot)) {}





Block* SShape::pivotToArr(Block pivot)
{
	Block* arr = new Block[NUM_BLOCKS];
	arr[0] = Block(pivot.xPos, pivot.yPos, COLOR);
	arr[1] = Block(pivot.xPos - 1, pivot.yPos, COLOR);
	arr[2] = Block(pivot.xPos, pivot.yPos + 1, COLOR);
	arr[3] = Block(pivot.xPos + 1, pivot.yPos + 1, COLOR);
	return arr;
}

SShape::SShape(Block pivot) : RadialShape(pivotToArr(pivot)) {}





Block* ZShape::pivotToArr(Block pivot)
{
	Block* arr = new Block[NUM_BLOCKS];
	arr[0] = Block(pivot.xPos, pivot.yPos, COLOR);
	arr[1] = Block(pivot.xPos + 1, pivot.yPos, COLOR);
	arr[2] = Block(pivot.xPos, pivot.yPos + 1, COLOR);
	arr[3] = Block(pivot.xPos - 1, pivot.yPos + 1, COLOR);
	return arr;
}

ZShape::ZShape(Block pivot) : RadialShape(pivotToArr(pivot)) {}





Block* TShape::pivotToArr(Block pivot)
{
	Block* arr = new Block[NUM_BLOCKS];
	arr[0] = Block(pivot.xPos, pivot.yPos, COLOR);
	arr[1] = Block(pivot.xPos, pivot.yPos + 1, COLOR);
	arr[2] = Block(pivot.xPos - 1, pivot.yPos, COLOR);
	arr[3] = Block(pivot.xPos + 1, pivot.yPos, COLOR);
	return arr;
}

TShape::TShape(Block pivot) : RadialShape(pivotToArr(pivot)) {}