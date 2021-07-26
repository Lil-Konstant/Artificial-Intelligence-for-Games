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

	// Distance is close enough so return true as we have reached the target cell
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

//
bool Agent::SeekBehaviour(Agent* target)
{
	// Calculate a vector pointing from this agent to the target agent
	Vec3 desiredVelocity = target->m_position - m_position;
	float distanceToTarget = desiredVelocity.Magnitude();

	// Distance is less that attack range so return as this agent can now attack
	if (distanceToTarget < m_attackRange)
	{
		return true;
	}
	// As long as we are not in attack range, then normalise the vector and scale it by the agents move speed
	else
	{
		desiredVelocity = desiredVelocity.GetNormalised() * m_maxSpeed;
		// Add the steering force to this agents current force
		AddForce(desiredVelocity - m_velocity);
		// Return false as this agent is not yet within attack range of the inputted target
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

void Agent::AttemptCollectResource()
{
	// Get the cell this player unit is currently in	
	m_currentCell = m_grid->getCell(m_position);

	// If there is a resource in this cell
	if (m_currentCell->m_resource != nullptr)
	{
		//Check for collisions with resource nodes within this cell
		if (m_currentCell->m_resource->TryCollision(this))
		{
			AddUnit();
			// Remove this resource from the agents shared resource list
			m_resourceList.erase(std::find(m_resourceList.begin(), m_resourceList.end(), m_currentCell->m_resource));
			delete m_currentCell->m_resource;
			m_currentCell->m_resource = nullptr;
		}
	}
}

// Returns true if this agent is now dead
bool Agent::TakeDamage(float damage)
{
	m_health -= damage;

	return m_health <= 0;
}

void Agent::UpdateMotion(float deltaTime)
{
	// Add Force multiplied by delta time to Velocity, truncate with the max speed to not over speed
	m_velocity = Truncate(m_velocity + (m_force * deltaTime), m_maxSpeed);
	// Add Velocity multiplied by delta time to Position
	m_position = m_position + (m_velocity * deltaTime);
	// Scale the velocity down according to the friction
	m_velocity = m_velocity * m_frictionModifier;
}

void Agent::FollowPath()
{
	// If there is a path for the player agent to follow
	if (m_path.size() > 0)
	{
		// If the front cell is the last cell in the path, use arrival behaviour
		if (m_path.size() == 1)
		{
			// If the front cell has now been reached, pop it off the path list
			if (ArrivalBehaviour(m_path.front()))
			{
				m_path.erase(m_path.begin());
			}
		}
		// Otherwise seek towards the front cell in the path list
		else
		{
			// Some vector maths to stop agents from moving backwards if they are closer to their second path node
			// Draw a vector from the first node to the second node
			Vec3 nodeDisplacement = (m_path[1]->m_position - m_path[0]->m_position).GetNormalised();
			Vec3 agentDisplacement = (m_position - m_path[0]->m_position).GetNormalised();
			float angle = acos(nodeDisplacement.Dot(agentDisplacement));

			if (angle < PI / 4)
			{
				m_path.erase(m_path.begin());
			}

			// If the front cell has now been reached, pop it off the path list
			if (SeekBehaviour(m_path.front()))
			{
				m_path.erase(m_path.begin());
			}
		}
	}
}