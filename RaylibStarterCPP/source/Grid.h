#pragma once
#include "raylib.h"
#include "MathsClasses.h"
#include "Cell.h"
#include "Edge.h"
#include <vector>
#include <queue>
#include <algorithm>

using namespace MathsClasses;

class Grid
{
public:
	Grid();
	~Grid();

	void Draw();

	Cell* getCell(Vec3 position);
	std::vector<Cell*> aStar(Cell* startCell, Cell* endCell);

private:
	static const int NUM_CELLS = 10;
	int CELL_SIZE = GetScreenHeight()/NUM_CELLS;
	Cell m_gridArray[NUM_CELLS][NUM_CELLS];
};

// Used as a custom comparison operator for the A* priority queue
struct CellCompare
{
	// True if cell1 should have priority over cell2
	bool operator()(Cell* const& cell1, Cell* const& cell2)
	{
		return cell1->fScore < cell2->fScore;
	}
};