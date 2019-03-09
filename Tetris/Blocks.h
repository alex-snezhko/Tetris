#pragma once
#include "Grid.h"

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

enum Direction { left, right, down };

class Shape
{
protected:
	// Will point to an array of 4 blocks
	Block* blocks;

	Shape(Block* b);
	~Shape();

	// Whether or not the shape can move to the given position
	bool canChangePositions(Block* newPositions);
	virtual Block* pivotToArr(Block pivot) = 0;
	virtual Block* getPositionsAfterRotation() = 0;
	void changePositions(Block* newPositions);

public:
	static const int NUM_BLOCKS = 4;
	static const char COLOR = 0;

	Block* getBlocks();
	void move(Direction d);
	// Try to rotate shape, wall kick if needed
	virtual void rotate();
};

class Line : public Shape
{
	// Used to determine where to put pivot point for rotation; line is flat if even, tall if odd
	int rotationCount;

	Block* pivotToArr(Block pivot) override;
	Block* getPositionsAfterRotation() override;

public:
	// Light blue
	static const COLORREF COLOR = RGB(93, 234, 247);

	Line(Block pivot);
};

class Square : public Shape
{
	Block* pivotToArr(Block pivot) override;
	Block* getPositionsAfterRotation() override;

public:
	static const COLORREF COLOR = RGB(244, 247, 76);

	Square(Block pivot);

	void rotate() override;
};

// Shape that can be placed in a 3x3 grid
class RadialShape : public Shape
{
public:
	RadialShape(Block* b);

	Block* getPositionsAfterRotation() override;
};

class LShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = RGB(234, 170, 60);

	LShape(Block pivot);
};

class ReverseLShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = RGB(4, 80, 165);

	ReverseLShape(Block pivot);
};

class SShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = RGB(14, 204, 96);

	SShape(Block pivot);
};

class ZShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = RGB(204, 30, 14);

	ZShape(Block pivot);
};

class TShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	static const COLORREF COLOR = RGB(185, 14, 204);

	TShape(Block pivot);
};