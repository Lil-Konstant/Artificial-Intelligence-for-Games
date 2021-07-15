#pragma once
#include "raylib.h"
#include "MathsClasses.h"
#include "Cell.h"
#include "Edge.h"
#include <vector>

using namespace MathsClasses;

class Grid
{
public:
	Grid();
	~Grid();

	void Draw();

	Cell* getCell(Vec3 position);

private:
	static const int NUM_CELLS = 10;
	int CELL_SIZE = GetScreenHeight()/NUM_CELLS;
	Cell m_gridArray[NUM_CELLS][NUM_CELLS];
};