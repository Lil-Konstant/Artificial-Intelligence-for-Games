#include "Agent.h"
#include "Behaviour.h"

// Initialise the static resource list as null
std::vector<Resource*> Agent::m_resourceList = std::vector<Resource*>();

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

bool Agent::SeekBehaviour(Cell* target)
{
	// Calculate a vector pointing from this agent to the target agent
	Vec3 desiredVelocity = target->m_position - m_position;
	float distanceToTarget = desiredVelocity.Magnitude();

	// Distance is 0 so return true as we have reached the target cell
	if (distanceToTarget < 10)
	{
		return true;
	}
	// As long as the distance is not 0, then normalise the vector and scale it by its move speed
	else
	{
		desiredVelocity = desiredVelocity.GetNormalised() * m_maxSpeed;
		// Add the steering force to this agents current force and return false as we are yet to reach the front cell in the path
		AddForce(desiredVelocity - m_velocity);
		return false;
	}
}

bool Agent::ArrivalBehaviour(Cell* target)
{
	// Seek towards the current target
	Vec3 desiredVelocity = target->m_position - m_position;
	float distanceToTarget = desiredVelocity.Magnitude();

	// Distance is 0 so return true as we have reached the target cell
	if (distanceToTarget < 1)
	{
		return true;
	}
	// If the target is within the arrival radius, scale the velocity down by the distance
	else if (distanceToTarget < m_arrivalRadius)
	{
		desiredVelocity = desiredVelocity.GetNormalised() * m_maxSpeed * ((distanceToTarget * 2) / m_arrivalRadius);
	}
	// Other wise the velocity is the normal seek velocity
	else
	{
		desiredVelocity = desiredVelocity.GetNormalised() * m_maxSpeed;
	}

	// Add the steering force to this agents current force and return false as we are yet to reach the front cell in the path
	AddForce(desiredVelocity - m_velocity);
	return false;
}