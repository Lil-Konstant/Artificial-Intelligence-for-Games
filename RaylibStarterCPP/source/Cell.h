#pragma once
#include "raylib.h"
#include "MathsClasses.h"
#include "Edge.h"
#include <vector>
#include <iostream>

using namespace MathsClasses;

class Cell
{
public:
	Cell() {};
	~Cell() {};

	// Draw this cell as a black dot (with a yellow inline if selected), and draw the edge connections of this cell (yellow if selected)
	void Draw(bool selected = false, Cell* target = nullptr)
	{
		switch (m_traversable)
		{
		case 0:
			DrawRectangle(m_position.x - (CELL_SIZE / 2), m_position.y - (CELL_SIZE / 2), CELL_SIZE, CELL_SIZE, BLACK);
			break;
		case 1:
			if (selected)
			{
				DrawCircle(m_position.x, m_position.y, 3, YELLOW);

				// Draw the target edge connection in yellow
				for (auto edge : m_edges)
				{
					if (edge.m_target == target)
					{
						Vec3 lineStart = Vec3(m_position.x, m_position.y, 0);
						Vec3 lineEnd = Vec3(edge.m_target->m_position.x, edge.m_target->m_position.y, 0);
						DrawLine(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, YELLOW);
					}
				}
			}
			else
			{
				DrawCircle(m_position.x, m_position.y, 6, BLACK);

				// Draw the edge connections in dark grey
				for (auto edge : m_edges)
				{
					Vec3 lineStart = Vec3(m_position.x, m_position.y, 0);
					Vec3 lineEnd = Vec3(edge.m_target->m_position.x, edge.m_target->m_position.y, 0);
					DrawLine(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, DARKGRAY);
				}
			}
			break;
		}
	}

	Vec3 m_position = Vec3(0, 0, 0);
	std::vector<Edge> m_edges;

	// Used in A* pathfinding
	float fScore = 0;
	float gScore = 0;
	Cell* m_previous = nullptr;
	bool m_traversable = true;

	// Debug
	int CELL_SIZE = GetScreenHeight() / 20;
};