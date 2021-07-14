#include "MouseFollowBehaviour.h"

#include <iostream>

// Checks if the player has clicked the screen this frame, if so it places a target at the mouse location and seeks the agent towards it
bool MouseFollowBehaviour::Update(float deltaTime)
{
	// If the player has left clicked this update
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		m_target = Vec3(GetMouseX(), GetMouseY(), 0);
		std::cout << "Targetting: " << m_target.x << ", " << m_target.y << std::endl;
	}

	// Seek towards the current target
	Vec3 desiredVector = m_target - m_agent->m_position;
	float distanceToTarget = desiredVector.Magnitude();


	if (distanceToTarget == 0)
		return true;
	else if (distanceToTarget < m_arrivalRadius)
		desiredVector = desiredVector.GetNormalised() * m_agent->m_maxSpeed * (distanceToTarget/m_arrivalRadius);
	else
		desiredVector = desiredVector.GetNormalised() * m_agent->m_maxSpeed;

	Vec3 steeringForce = desiredVector - m_agent->m_velocity;

	m_agent->m_force = m_agent->m_force + steeringForce;
	return true;
}

// Draw the follow target as an X to the screen if the agent is not already ontop of it
void MouseFollowBehaviour::Draw()
{
	if (m_target.Distance(m_agent->m_position) < 14)
		return;

	Vec3 topLeft = Vec3(m_target.x - 5, m_target.y - 5, 0);
	Vec3 topRight = Vec3(m_target.x + 5, m_target.y - 5, 0);
	Vec3 bottomLeft = Vec3(m_target.x - 5, m_target.y + 5, 0);
	Vec3 bottomRight = Vec3(m_target.x + 5, m_target.y + 5, 0);

	DrawLineEx({ topLeft.x, topLeft.y }, { bottomRight.x, bottomRight.y }, 0.5f, RED);
	DrawLineEx({ bottomLeft.x, bottomLeft.y }, { topRight.x, topRight.y }, 0.5f, RED);
}