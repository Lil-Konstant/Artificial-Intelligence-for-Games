#include "Resource.h"

// Draw resource nodes as brown circles 
void Resource::Draw()
{
	DrawCircle(m_position.x, m_position.y, m_radius, BROWN);
	DrawCircleLines(m_position.x, m_position.y, m_radius, BLACK);
}

// Checks for a collision with this resource node and the inputted gameobject, true if so
bool Resource::TryCollision(GameObject* other)
{
	float distance = m_position.Distance(other->m_position);
	return distance < (m_radius + other->m_radius);
}