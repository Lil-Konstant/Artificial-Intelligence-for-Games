#include "Cell.h"

// Initialise the static cell size as 0, it will be set during grid construction as the size of the screen is needed for the calculation
int Cell::CELL_SIZE = 0;

// If this cell is untraversable, the draw function simple draws this cell as a dark brown square the size of the cell.
// If in debug mode, the function will draw the centre position of the cell as a black circle, and its edge connections
// as dark grey lines leading to their target cells. If this cell has a resource, then it draws it at its position (the centre
// of the cell) as a brown circle. The target parameter is nullptr by default, and is used by the player and enemy agents when
// attempting to draw their current paths, the agents attempt to draw each cell in their current path iteratively, each time passing the
// next cell in the path as the target so the draw function can draw lines connecting each path cell up.
void Cell::Draw(bool debugMode, Cell* target)
{
	switch (m_traversable)
	{
	case false:
		// Draw untraversable cells as a darkbrown square
		DrawRectangle(m_position.x - (CELL_SIZE / 2), m_position.y - (CELL_SIZE / 2), CELL_SIZE, CELL_SIZE, DARKBROWN);
		break;

	case true:
		// If in debug mode, draw the centres of cells and their edge connections in dark grey
		if (debugMode)
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
		
		// If any of the edges of this cell point to the inputted target (if one was given), draw a yellow line from this cell along the edge
		if (target != nullptr)
		{
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

		// Draw the resource node of this cell if it exists
		if (m_resource != nullptr)
			m_resource->Draw();
		
		break;
	}
}