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

	void Draw(bool debugMode);

	void SetTerrain(Vec3 startIndex, Vec3 endIndex);

	Cell* getCell(Vec3 position);
	std::vector<Cell*> aStar(Cell* startCell, Cell* endCell);

	static const int NUM_CELLS = 20;
	int CELL_SIZE = GetScreenHeight() / NUM_CELLS;

	// Construct the 4 search waypoints near the corners of the map
	Cell* topLeftWaypoint = getCell(Vec3(GetScreenWidth() / 10, GetScreenHeight() / 10, 0));
	Cell* topRightWaypoint = getCell(Vec3(GetScreenWidth() - (GetScreenWidth() / 10), GetScreenHeight() / 10, 0));
	Cell* bottomRightWaypoint = getCell(Vec3(GetScreenWidth() - (GetScreenWidth() / 10), GetScreenHeight() - (GetScreenHeight() / 10), 0));
	Cell* bottomLeftWaypoint = getCell(Vec3(GetScreenWidth() / 10, GetScreenHeight() - (GetScreenHeight() / 10), 0));

private:
	Cell m_gridArray[NUM_CELLS][NUM_CELLS];
};

// Used as a custom comparison operator for the A* priority queue (min heap)
struct CellCompare
{
	bool operator()(Cell* const& cell1, Cell* const& cell2)
	{
		return cell1->fScore > cell2->fScore;
	}
};