#pragma once
#include "Grid.h"
#include "Helpers.h"
#include <exception>

#define LIGHT_BLUE RGB(93, 234, 247)
#define YELLOW RGB(244, 247, 76)
#define ORANGE RGB(234, 170, 60)
#define BLUE RGB(4, 80, 165)
#define GREEN RGB(14, 204, 96)
#define RED RGB(204, 30, 14)
#define PURPLE RGB(185, 14, 204)


struct ShapeAtBottomException : public std::exception {};

struct Block
{
	int xPos;
	int yPos;
	COLORREF color;

	Block();
	Block(int x, int y);
	Block(int x, int y, COLORREF c);

	void moveTo(int x, int y);
};

enum Direction { left = -1, down, right = 1 };

enum RotationPosition { spawn = 0, onceCW = 1, upsideDown = 2, onceCCW = 3 };

class Shape
{
protected:
	// Will point to an array of 4 blocks
	Block* blocks;

	Shape(Block* b);

	// Whether or not the shape can move to the given position
	bool canChangePositions(Block* newPositions);
	virtual Block* pivotToArr(Block pivot) = 0;
	virtual Block* getPositionsAfterRotation(Direction d) = 0;
	virtual Block* getValidPositionAfterWallkick(Block* positions, Direction d) = 0;
	
public:
	RotationPosition rotationPosition;

	~Shape();

	static const int NUM_BLOCKS = 4;

	void changePositions(Block* newPositions);
	Block* getBlocks();
	// Throws exception if block at bottom
	void move(Direction d);
	// Try to rotate shape, wall kick if needed
	void rotate(Direction d);
	virtual Shape* getNew() = 0;
};





class Line : public Shape
{
	Block* pivotToArr(Block pivot) override;
	Block* getPositionsAfterRotation(Direction d) override;
	Block* getValidPositionAfterWallkick(Block* positions, Direction d) override;

public:
	// Light blue
	static const COLORREF COLOR = LIGHT_BLUE;

	Line(Block pivot);

	Shape* getNew() override { return new Line(Block(4, 18)); }
};

class Square : public Shape
{
	Block* pivotToArr(Block pivot) override;
	Block* getPositionsAfterRotation(Direction d) override { return nullptr; }
	Block* getValidPositionAfterWallkick(Block* positions, Direction d) override { return nullptr; }

public:
	static const COLORREF COLOR = YELLOW;

	Square(Block pivot);

	Shape* getNew() override { return new Square(Block(4, 18)); }
};

// Shape that can be placed in a 3x3 grid
class RadialShape : public Shape
{
	Block* getPositionsAfterRotation(Direction d) override;
	Block* getValidPositionAfterWallkick(Block* positions, Direction d) override;

public:
	RadialShape(Block* b);
};

class LShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = ORANGE;

	LShape(Block pivot);

	Shape* getNew() override { return new LShape(Block(4, 18)); }
};

class JShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = BLUE;

	JShape(Block pivot);

	Shape* getNew() override { return new JShape(Block(4, 18)); }
};

class SShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = GREEN;

	SShape(Block pivot);

	Shape* getNew() override { return new SShape(Block(4, 18)); }
};

class ZShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = RED;

	ZShape(Block pivot);

	Shape* getNew() override { return new ZShape(Block(4, 18)); }
};

class TShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = PURPLE;

	TShape(Block pivot);

	Shape* getNew() override { return new TShape(Block(4, 18)); }
};