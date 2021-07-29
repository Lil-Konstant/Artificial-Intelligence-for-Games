#pragma once
#include "raylib.h"
#include "MathsClasses.h"
#include "Cell.h"
#include "Edge.h"
#include "Waypoint.h"
#include <vector>
#include <queue>
#include <algorithm>

using namespace MathsClasses;

/// <summary>
/// The Grid class is the map framework for the game, and is what allows agents to manoeuvre around.
/// The grid is set up as a square, 2D array of cells all of equal sizing, this means that spacial indexing
/// can be used to convert a gameobject's position directly into an array index for the cell they are currently in.
/// The grid itself holds an array of cells, which can in turn hold resources, as well as an array of waypoints
/// that the AI uses for fleeing and searching navigation. The Grid class also implements the A* pathfinding algorithm
/// used to find the shortest path between two points on the grid itself.
/// </summary>
class Grid
{
public:
	Grid();
	~Grid();
	
	// Grid initialisation functions
	void InitialiseEdges();
	void InitialiseWaypoints();
	void SetTerrain(Vec3 startIndex, Vec3 endIndex);

	// Spacial indexing function
	Cell* getCell(Vec3 position);
	// Pathfinding algorithm
	std::vector<Cell*> aStar(Cell* startCell, Cell* endCell);

	void Draw(bool debugMode);

	// Number of cells along the height and width of the grid
	static const int NUM_CELLS = 20;
	int CELL_SIZE;

	Cell m_gridArray[NUM_CELLS][NUM_CELLS];
	std::vector<Waypoint*> m_waypoints;
};

// Used as a custom comparison operator for the A* priority queue (min heap)
struct CellCompare
{
	bool operator()(Cell* const& cell1, Cell* const& cell2)
	{
		return cell1->fScore > cell2->fScore;
	}
};