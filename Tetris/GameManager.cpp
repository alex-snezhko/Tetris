#include "stdafx.h"
#include "GameManager.h"

int GameManager::score = 0;

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
			Grid::grid[blocks[i].yPos][blocks[i].xPos] = value;
		}
	}
	delete[] blocks;
}

bool GameManager::currentPositionOpen()
{
	Block* blocks = currentShape->getBlocks();
	for (int i = 0; i < Shape::NUM_BLOCKS; i++)
	{
		if (blocks[i].yPos < Grid::HEIGHT && Grid::grid[blocks[i].yPos][blocks[i].xPos])
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
}

void GameManager::initialize()
{
	currentShape = nullptr;
	heldShape = nullptr;
	shapeHeldThisDrop = false;
	for (int i = 0; i < Grid::HEIGHT; i++)
	{
		for (int j = 0; j < Grid::WIDTH; j++)
		{
			Grid::grid[i][j] = false;
		}
	}
	score = 0;

	findNextShape();
}

int GameManager::getScore()
{
	return score;
}

// Parameter will be true if shape being placed is the one being held (except if this is the first piece held this game)
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
		} while (!currentPositionOpen());

		if (attempts == 2)
		{
			endGame();
		}
	}

	setCurrentOnGrid(true);
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
		nextShape = new ReverseLShape(b);
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
			heldShape = currentShape;
			setCurrentOnGrid(false);
			placeShape();
		}
		else
		{
			Shape* temp = heldShape;
			heldShape = currentShape;
			setCurrentOnGrid(false);
			currentShape = temp;
			placeShape(true);
		}
	}
}
