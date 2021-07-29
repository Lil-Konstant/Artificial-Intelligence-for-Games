#include "Agent.h"

// Initialise the static resource list as an empty list (resources are added on game setup)
std::vector<Resource*> Agent::m_resourceList = std::vector<Resource*>();

// Truncate takes an input vector and a maximum value, and will scale the vector's magnitude down to the maximum value
Vec3 Agent::Truncate(Vec3 vector, float truncateMax)
{
	float scaleRatio = truncateMax / vector.Magnitude();
	scaleRatio = scaleRatio < 1.0 ? scaleRatio : 1.0;
	return vector * scaleRatio;
}

// SeekBehaviour(Cell*) is used to move agents towards cells in their pathlists, the function simply
// calculates the vector pointing from this agent to the target cell and applies a steering force
// to the agent to get it to move in that direction
bool Agent::SeekBehaviour(Cell* target)
{
	// Calculate a vector pointing from this agent to the target cell
	Vec3 desiredVelocity = target->m_position - m_position;
	float distanceToTarget = desiredVelocity.Magnitude();

	// Distance is close enough so return true as we have reached the target cell
	if (distanceToTarget < 0.5f)
	{
		return true;
	}
	// As long as the distance is not 0, then normalise the vector and scale it by its move speed
	else
	{
		desiredVelocity = desiredVelocity.GetNormalised() * m_currentMoveSpeed;
		// Add the steering force to this agents current force and return false as we are yet to reach the front cell in the path
		AddForce(desiredVelocity - m_velocity);
		return false;
	}
}

// SeekBehaviour(Agent*) is used to move agents towards other agents during attack sequence, the function simply
// calculates the vector pointing from this agent to the target agent and applies a steering force
// to the agent to get it to move in that direction. If this agent is now within attack range of the target
// agent, return true so it can engage in damaging it's target.
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
		desiredVelocity = desiredVelocity.GetNormalised() * m_currentMoveSpeed;
		// Add the steering force to this agents current force
		AddForce(desiredVelocity - m_velocity);
		// Return false as this agent is not yet within attack range of the inputted target
		return false;
	}
}

// AttemptCollectResource will get the current cell of the agent and attempt to collect the resource in this cell
// if one is present and if this agent is within collision range of the resource.
void Agent::AttemptCollectResource()
{
	// Get the cell this agent unit is currently in	
	m_currentCell = m_grid->getCell(m_position);

	// If indexing outside of the grid, return
	if (m_currentCell == nullptr)
		return;

	// If there is a resource in this cell
	if (m_currentCell->m_resource != nullptr)
	{
		//Check for collision with the resource node in this cell
		if (m_currentCell->m_resource->TryCollision(this))
		{
			AddUnit();
			// Remove this resource from the agents static resource list and delete it
			m_resourceList.erase(std::find(m_resourceList.begin(), m_resourceList.end(), m_currentCell->m_resource));
			delete m_currentCell->m_resource;
			m_currentCell->m_resource = nullptr;
		}
	}
}

// Applies the inputted damage to this agents health and returns true if this agent is now dead
bool Agent::TakeDamage(float damage)
{
	m_health -= damage;

	return m_health <= 0;
}

// UpdateMoveSpeed will scale the agents current maximum move speed inversely proportional to its current army size
void Agent::UpdateMoveSpeed()
{
	float scaledSpeed = m_maxSpeed * (1 / (0.2f * (float)GetUnitCount()));
	m_currentMoveSpeed = scaledSpeed < m_maxSpeed ? scaledSpeed : m_maxSpeed;
}

// FollowPath is used to seek the current front node in this agents path, and removes nodes from the front of the pathlist
// as they are reached. The function also performs some basic vector maths to calculate if the agent is in between the first
// cell and second cells in it's path (within a 30 degree range), and if so it just skips the first node and heads straight to
// the second. This is to stop agents from continually moving backwards to their first node if the pathfinding function is
// called too quickly.
void Agent::FollowPath()
{
	// If there is at least two nodes in the path
	if (m_path.size() > 1)
	{
		// Some vector maths to stop agents from moving backwards if they are closer to their second path node
		// Draw a vector from the first node to the second node
		Vec3 nodeDisplacement = (m_path[1]->m_position - m_path[0]->m_position).GetNormalised();
		// Draw a vector from the first node to the agent
		Vec3 agentDisplacement = (m_position - m_path[0]->m_position).GetNormalised();
		// Get the angle between the two
		float angle = acos(nodeDisplacement.Dot(agentDisplacement));

		// If the angle is less than 30 degress then pop the front cell from the path list as the agent is close enough in the right direction to the next node
		if (angle < PI / 6)
		{
			m_path.erase(m_path.begin());
		}
	}

	// If there is still a path for the agent to follow after possibly erasing the first node
	if (m_path.size() > 0)
	{
		// Seeks to the first node in the agents path, is true if that node has now been reached, so erase it from the path list
		if (SeekBehaviour(m_path.front()))
		{
			m_path.erase(m_path.begin());
		}
	}
}