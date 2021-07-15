#pragma once
#include "raylib.h"
#include "MathsClasses.h"
#include <vector>

using namespace MathsClasses;

class Edge;
class Cell
{
public:
	Cell() {};
	~Cell() {};

	void Draw(bool selected = false)
	{
		if (selected)
		{
			DrawCircle(m_position.x, m_position.y, 3, YELLOW);
		}
		else
		{
			DrawCircle(m_position.x, m_position.y, 6, BLACK);
		}
	}

	Vec3 m_position = Vec3(0, 0, 0);
	std::vector<Edge> m_edges;

	// Used in A* pathfinding
	float fScore = 0;
	float gScore = 0;
	Cell* m_previous;
};