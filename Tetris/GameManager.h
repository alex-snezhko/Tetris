#pragma once
#include "Grid.h"

class GameManager
{
	GameManager();

	static int score;

	static Shape* currentShape;

	static Shape* nextShape;

	static Shape* heldShape;

	// Whether or not a piece has been held already this drop
	static bool shapeHeldThisDrop;

	static void setCurrentOnGrid(bool value);

	static bool currentPositionOpen();

	static void endGame();
	
public:
	static void initialize();

	static int getScore();

	// Parameter will be true if shape being placed is the one being held (except if this is the first piece held this game)
	static void placeShape(bool held = false);

	// Offset value determines whether to shift new pieces up (if default starting position is occupied)
	static void findNextShape(int offset = 0);

	static void holdShape();
};

