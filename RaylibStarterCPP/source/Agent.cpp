#include "Agent.h"
#include "Behaviour.h"

Agent::~Agent()
{

}

Vec3 Agent::Truncate(Vec3 vector, float truncateMax)
{
	float scaleRatio = truncateMax / vector.Magnitude();
	scaleRatio = scaleRatio < 1.0 ? scaleRatio : 1.0;
	return vector * scaleRatio;
}

// Add a behaviour to the agents behaviour list, and set the behaviours agent pointer to this agent
void Agent::AddBehaviour(Behaviour* behaviour)
{
	m_behaviourList.push_back(behaviour);
	behaviour->m_agent = this;
}