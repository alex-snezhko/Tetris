#include "stdafx.h"
#include "GameManager.h"

int GameManager::score = 0;
int GameManager::level = 1;
int GameManager::rowsCompletedThisLevel = 0;
bool GameManager::atBottom = false;

Shape* GameManager::currentShape = nullptr;
Shape* GameManager::nextShape = nullptr;
Shape* GameManager::heldShape = nullptr;

bool GameManager::shapeHeldThisDrop = false;

void GameManager::setCurrentOnGrid(bool value)
{
	Block* blocks = currentShape->getBlocks();
	for (int i = 0; i < Shape::NUM_BLOCKS; i++)
	{
		// True if placed block has been offset
		if (blocks[i].yPos < Grid::HEIGHT)
		{
			Grid::grid[blocks[i].yPos][blocks[i].xPos].occupied = value;
			if (value)
			{
				Grid::grid[blocks[i].yPos][blocks[i].xPos].color = blocks[i].color;
			}
			else
			{
				Grid::grid[blocks[i].yPos][blocks[i].xPos].color = RGB(255, 255, 255);
			}
		}
	}
	delete[] blocks;
}

bool GameManager::currentPositionOpen()
{
	Block* blocks = currentShape->getBlocks();
	for (int i = 0; i < Shape::NUM_BLOCKS; i++)
	{
		if (blocks[i].yPos < Grid::HEIGHT && Grid::grid[blocks[i].yPos][blocks[i].xPos].occupied)
		{
			delete[] blocks;
			return false;
		}
	}
	delete[] blocks;
	return true;
}

void GameManager::endGame()
{
	initialize();
}

void GameManager::initialize()
{
	findNextShape();

	heldShape = nullptr;

	for (int i = 0; i < Grid::HEIGHT; i++)
	{
		for (int j = 0; j < Grid::WIDTH; j++)
		{
			Grid::grid[i][j].occupied = false;
			Grid::grid[i][j].color = RGB(255, 255, 255);
		}
	}
	score = 0;
	level = 1;
	rowsCompletedThisLevel = 0;

	placeShape();
}

int GameManager::getScore()
{
	return score;
}

int GameManager::getLevel()
{
	return level;
}

void GameManager::clearRows()
{
	int numRows = Grid::findNumCompletedRows();

	Grid::clearRows();
	
	int baseScore = 0;

	switch (numRows)
	{
	case 1:
		baseScore = 40;
		break;
	case 2:
		baseScore = 100;
		break;
	case 3:
		baseScore = 300;
		break;
	case 4:
		baseScore = 1200;
		break;
	}

	score += baseScore * level;
	rowsCompletedThisLevel += numRows;

	if (rowsCompletedThisLevel >= 10)
	{
		level++;
		rowsCompletedThisLevel = 0;
	}
}

// Parameter will be true if shape being placed is the one being held (except if this is the first piece held this game)
// makes next shape current shape and finds next shape
void GameManager::placeShape(bool held)
{
	shapeHeldThisDrop = false;

	if (!held)
	{
		currentShape = nextShape;

		// Try to place block higher if current position occupied (close to losing)
		int attempts = 0;
		do 
		{
			findNextShape(attempts);
			attempts++;
			if (attempts == 2)
			{
				break;
			}
		} while (!currentPositionOpen());

		if (attempts == 2)
		{
			endGame();
		}
	}

	setCurrentOnGrid(true);
}

// Returns true if rows to be cleared
bool GameManager::move(Direction d)
{
	try
	{
		currentShape->move(d);
	}
	catch (ShapeAtBottomException&)
	{
		delete currentShape;
		clearRows();
		placeShape();

		atBottom = true;
		return Grid::findNumCompletedRows() > 0;
	}
	return false;
}

void GameManager::rotate(Direction d)
{
	currentShape->rotate(d);
}

// Returns true if rows to be cleared
bool GameManager::fallToBottom()
{
	while (true)
	{
		try
		{
			currentShape->move(down);
		}
		catch (ShapeAtBottomException&)
		{
			delete currentShape;
			clearRows();
			placeShape();

			atBottom = true;
			return Grid::findNumCompletedRows() > 0;
		}
	}
	return false;
}

Block* GameManager::getDropPreview()
{
	Block* initPos = currentShape->getBlocks();
	Block* newPos = nullptr;

	while (true)
	{
		try
		{
			currentShape->move(down);
		}
		catch (ShapeAtBottomException&)
		{
			newPos = currentShape->getBlocks();
			currentShape->changePositions(initPos);
			delete[] initPos;
			break;
		}
	}

	return newPos;
}

// Offset value determines whether to shift new pieces up (if default starting position is occupied)
void GameManager::findNextShape(int offset)
{
	Block b(4, 18 + offset);
	
	int r = rand();
	switch (r % 7)
	{
	case 0:
		nextShape = new Line(b);
		break;
	case 1:
		nextShape = new Square(b);
		break;
	case 2:
		nextShape = new LShape(b);
		break;
	case 3:
		nextShape = new JShape(b);
		break;
	case 4:
		nextShape = new SShape(b);
		break;
	case 5:
		nextShape = new ZShape(b);
		break;
	case 6:
		nextShape = new TShape(b);
		break;
	}
}

void GameManager::holdShape()
{
	if (!shapeHeldThisDrop)
	{
		if (heldShape == nullptr)
		{
			heldShape = currentShape->getNew();			
			setCurrentOnGrid(false);
			delete currentShape;
			placeShape();
		}
		else
		{
			Shape* temp = heldShape;
			heldShape = currentShape->getNew();
			setCurrentOnGrid(false);
			delete currentShape;
			currentShape = temp;
			placeShape(true);
		}

		shapeHeldThisDrop = true;
		while (heldShape->rotationPosition != spawn)
		{
			heldShape->rotate(right);
		}
		
	}
}

Shape* GameManager::getCurrentShape()
{
	return currentShape;
}

Shape* GameManager::getNextShape()
{
	return nextShape;
}

Shape* GameManager::getHeldShape()
{
	return heldShape;
}
