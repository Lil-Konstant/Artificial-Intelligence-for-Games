#include "Agent.h"
#include "Behaviour.h"

Agent::Agent(Grid* grid) : m_grid(grid) {}

Agent::~Agent()
{

}

Vec3 Agent::Truncate(Vec3 vector, float truncateMax)
{
	float scaleRatio = truncateMax / vector.Magnitude();
	scaleRatio = scaleRatio < 1.0 ? scaleRatio : 1.0;
	return vector * scaleRatio;
}

// Use the behaviours in this agents behaviour list to calculate and add to the total force on this agent this frame
void Agent::Update(float deltaTime)
{
	// Reset the force for this frame to 0
	m_force = Vec3(0, 0, 0);

	// For each Behaviour in Behaviour list
	for (int i = 0; i < m_behaviourList.size(); i++)
	{
		// Call the Behaviour’s Update function
		m_behaviourList[i]->Update(deltaTime);
	}

	// Add Force multiplied by delta time to Velocity, truncate with the max speed to not over speed
	m_velocity = Truncate(m_velocity + (m_force * deltaTime), m_maxSpeed);
	// Add Velocity multiplied by delta time to Position
	m_position = m_position + (m_velocity * deltaTime);
	// Scale the velocity down according to the friction
	m_velocity = m_velocity * m_frictionModifier;
}

// Draw the agent as a circle
void Agent::Draw()
{
	DrawCircle(m_position.x, m_position.y, 4, PURPLE);
}

// Add a behaviour to the agents behaviour list, and set the behaviours agent pointer to this agent
void Agent::AddBehaviour(Behaviour* behaviour)
{
	m_behaviourList.push_back(behaviour);
	behaviour->m_agent = this;
}