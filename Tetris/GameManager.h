#pragma once
#include "Blocks.h"
#include <time.h>
#include <string>

class GameManager
{
	GameManager();

	static int score;
	static int level;
	static int rowsCompletedThisLevel;
	

	static Shape* currentShape;
	static Shape* nextShape;
	static Shape* heldShape;

	// Whether or not a piece has been held already this drop
	static bool shapeHeldThisDrop;

	static void setCurrentOnGrid(bool value);
	static bool currentPositionOpen();
	static void endGame();
	
public:
	static bool atBottom;

	static void initialize();
	static int getScore();
	static int getLevel();
	static void clearRows();
	// Parameter will be true if shape being placed is the one being held (except if this is the first piece held this game)
	static void placeShape(bool held = false);
	static bool move(Direction d);
	static void rotate(Direction d);
	static bool fallToBottom();
	static Block* getDropPreview();
	// Offset value determines whether to shift new pieces up (if default starting position is occupied)
	static void findNextShape(int offset = 0);
	static void holdShape();
	static Shape* getCurrentShape();
	static Shape* getNextShape();
	static Shape* getHeldShape();
};

