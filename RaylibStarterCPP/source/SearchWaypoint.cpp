#include "SearchWaypoint.h"

// Draw the search waypoint as a red square
void SearchWaypoint::Draw()
{
	DrawPoly({ m_position.x, m_position.y }, 4, m_radius, 0, DARKBLUE);
	DrawPolyLines({ m_position.x, m_position.y }, 4, m_radius, 0, BLACK);
}