#pragma once
#include "Edge.h"
#include "Resource.h"
#include <vector>
#include <iostream>

using namespace MathsClasses;

/// <summary>
/// The cell class is class used by the grid system of the game to allow for pathfinding and resource placement.
/// It contains a list of edges that connect it to other cells in the grid, a pointer to the resource currently 
/// placed in it (if there is one), a bool telling whether or not this cell can be traverse, and variables related 
/// to the A* algorithm implemented in the grid class. It also contains a draw function that draws the cell differently
/// depending on if it's traversable, currently selected and if the game is in debug mode.
/// </summary>
class Cell
{
public:
	Cell() {};
	~Cell() {};
	void Draw(bool debugMode, Cell* target = nullptr);

	Vec3 m_position = Vec3(0, 0, 0);
	std::vector<Edge> m_edges;
	Resource* m_resource = nullptr;
	bool m_traversable = true;

	// Used in A* pathfinding in the grid class
	float fScore = 0;
	float gScore = 0;
	Cell* m_previous = nullptr;

	// Cell size is the width and height of the cell, equal to ScreenHeight(or width) / Number of Cells, calculated in grid class
	static int CELL_SIZE;
};