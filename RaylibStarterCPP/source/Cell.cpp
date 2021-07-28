#include "Cell.h"

// Draw this cell as a black dot (with a yellow inline if selected), and draw the edge connections of this cell (yellow if along the enemy/player path)
void Cell::Draw(bool debugMode, bool selected, Cell* target)
{
	switch (m_traversable)
	{
	case 0:
		// Draw untraversable cells as a darkbrown square
		DrawRectangle(m_position.x - (CELL_SIZE / 2), m_position.y - (CELL_SIZE / 2), CELL_SIZE, CELL_SIZE, DARKBROWN);
		break;

	case 1:
		// If the cell is a part of the enemy or player path, draw it's centre as yellow and draw the path edges as yellow
		if (selected)
		{
			//DrawCircle(m_position.x, m_position.y, 3, YELLOW);

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
		
		// Otherwise if in debug mode, draw the centres of cells and their edge connections
		else if (debugMode)
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
		
		// Draw the resource node of this cell if it exists
		if (m_resource != nullptr)
			m_resource->Draw();
		
		break;
	}
}