#pragma once
#include "raylib.h"
#include "MathsClasses.h"
#include "Edge.h"
#include "Resource.h"
#include <vector>
#include <iostream>

using namespace MathsClasses;

class Cell
{
public:
	Cell() {};
	~Cell() {};

	void Draw(bool debugMode, bool selected = false, Cell* target = nullptr);

	Vec3 m_position = Vec3(0, 0, 0);
	std::vector<Edge> m_edges;

	Resource* m_resource = nullptr;

	// Used in A* pathfinding
	float fScore = 0;
	float gScore = 0;
	Cell* m_previous = nullptr;
	bool m_traversable = true;

	// Debug
	int CELL_SIZE = GetScreenHeight() / 20;
};