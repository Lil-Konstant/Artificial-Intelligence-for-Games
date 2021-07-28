#pragma once
#include "raylib.h"
#include "MathsClasses.h"
#include "Cell.h"
#include "Edge.h"
#include "SearchWaypoint.h"
#include <vector>
#include <queue>
#include <algorithm>

using namespace MathsClasses;

class Grid
{
public:
	Grid();
	~Grid();
	
	void InitialiseEdges();
	void InitialiseWaypoints();
	
	void SetTerrain(Vec3 startIndex, Vec3 endIndex);

	Cell* getCell(Vec3 position);
	std::vector<Cell*> aStar(Cell* startCell, Cell* endCell);

	void Draw(bool debugMode);

	// Number of cells along the height and width of the grid
	static const int NUM_CELLS = 20;
	int CELL_SIZE;

	std::vector<SearchWaypoint*> m_searchWaypoints;

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