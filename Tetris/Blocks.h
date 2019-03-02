#pragma once
#include "Grid.h"

struct Block
{
	int xPos;
	int yPos;

	Block();
	Block(int x, int y);

	void moveTo(int x, int y);
};

enum Direction { left, right, down };

class Shape
{
protected:
	// Will point to an array of 4 blocks
	Block* blocks;

	// Whether or not the shape can move to the given position
	bool canChangePositions(Block* newPositions);

	Shape(Block* b);

	~Shape();

	virtual Block* pivotToArr(Block pivot) = 0;

	virtual Block* getPositionsAfterRotation() = 0;

	void changePositions(Block* newPositions);

public:
	static const int NUM_BLOCKS = 4;

	static const int COLOR = 0;

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
	Line(Block pivot);
};

class Square : public Shape
{
	Block* pivotToArr(Block pivot) override;

	Block* getPositionsAfterRotation() override;

public:
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
	LShape(Block pivot);
};

class ReverseLShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	ReverseLShape(Block pivot);
};

class SShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	SShape(Block pivot);
};

class ZShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	ZShape(Block pivot);
};

class TShape : public RadialShape
{
	Block* pivotToArr(Block pivot) override;

public:
	TShape(Block pivot);
};