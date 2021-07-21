#include "Resource.h"

Resource::Resource(float radius) : GameObject(radius)
{

}

Resource::~Resource()
{

}

void Resource::Update(float deltaTime)
{

}

void Resource::Draw()
{
	DrawCircle(m_position.x, m_position.y, m_radius, YELLOW);
}

// Checks for a collision with this resource node and the inputted agent object
bool Resource::TryCollision(GameObject* other)
{
	float distance = m_position.Distance(other->m_position);

	if (distance < (m_radius + other->m_radius))
	{
		return true;
	}
	return false;
}