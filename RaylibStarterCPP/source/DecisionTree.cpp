#include "DecisionTree.h"

namespace DecisionTree
{
	// Condition which returns whether or not this agent is within pursue range of its target
	bool InPursueRange::conditionCheck(Agent* agent)
	{
		float distance = agent->m_position.Distance(agent->m_target->m_position);

		return distance < agent->m_pursueRadius;
	}

	// Condition which returns whether or not this agent is within flee range of its target
	bool InFleeRange::conditionCheck(Agent* agent)
	{
		float distance = agent->m_position.Distance(agent->m_target->m_position);

		return distance < agent->m_fleeRadius;
	}

	// Condition which returns whether or not this agents army outnumbers its targets army
	bool IsStronger::conditionCheck(Agent* agent)
	{
		// Compare the unit count of the player with that of the AI
		return agent->GetUnitCount() >= agent->m_target->GetUnitCount() + 2;
	}

	// Condition which returns whether or not resources still remain within the game map
	bool ResourcesRemain::conditionCheck(Agent* agent)
	{
		return agent->m_resourceList.size() > 0;
	}

	// Pursue action is a leaf action that contains the pursue logic for the enemy agent's AI.
	// The function simply gets the cell of the agent and the cell of its target, and sets the
	// agents path to be the path between these two cells.
	void PursueAction::makeDecision(Agent* agent)
	{
		Cell* currentCell = agent->m_currentCell;
		Cell* targetCell = agent->m_target->m_currentCell;

		agent->m_path = agent->m_grid->aStar(currentCell, targetCell);
	}

	// FleeAction is a leaf action that contains the flee logic for the enemy agent's AI.
	// The function will first get a list of the waypoints on a map (if it hasn't already),
	// and will then search through those waypoints to find the waypoint that is in the
	// most opposite direction to this agents target (i.e. the waypoint with the biggest
	// angle between it and the target), it then creates a path from this agent to that waypoint
	// for the agent to flee along.
	void FleeAction::makeDecision(Agent* agent)
	{
		// Fill the flee actions waypoint list with the grids waypoints once they have been connected
		if (m_fleeWaypoints.size() == 0)
			m_fleeWaypoints = agent->m_grid->m_waypoints;

		// If we have no fleeing target, iterate through the waypoints and find the one that is in the most opposite direction to the agents target
		if (m_currentWaypoint == nullptr)
		{
			m_currentWaypoint = m_fleeWaypoints.front();
			float currentAngleBetween = 0;
			for (auto waypoint : m_fleeWaypoints)
			{
				// Do not calculate waypoints the agent is sitting on due to cosine errors (0 degrees becomes 180)
				if (agent->m_grid->getCell(agent->m_position) == agent->m_grid->getCell(waypoint->m_position))
				{
					continue;
				}

				// Calculate the angle between this waypoint and the target
				Vec3 targetDisplacement = (agent->m_target->m_position - agent->m_position).GetNormalised();
				Vec3 waypointDisplacement = (waypoint->m_position - agent->m_position).GetNormalised();
				float angleBetween = acos(targetDisplacement.Dot(waypointDisplacement));

				// If the angle between is bigger than that of the current flee waypoint, set this waypoint as the new flee waypoint
				if (angleBetween > currentAngleBetween)
				{
					m_currentWaypoint = waypoint;
					currentAngleBetween = angleBetween;
				}
			}
		}

		// Set the agents path to be from itself to its current flee waypoint cell
		agent->m_path = agent->m_grid->aStar(agent->m_grid->getCell(agent->m_position), agent->m_grid->getCell(m_currentWaypoint->m_position));

		// If the agent has reached the waypoint cell, set current waypoint to nullptr to recalculate a new waypoint next update
		if (agent->m_grid->getCell(agent->m_position) == agent->m_grid->getCell(m_currentWaypoint->m_position))
		{
			// Reset the current waypoint to null so that a new flee point is found next update
			m_currentWaypoint = nullptr;
		}
	}

	// SearchAction is a leaf action that contains the search logic for the enemy agent's AI.
	// The function will first get a list of the waypoints on a map (if it hasn't already),
	// and will then on its first call search through all the waypoints to find the one that is 
	// currently closest to it's target and path towards said waypoint. On subsequent calls,
	// the function interates through the waypoint connections of it's current waypoint to find
	// the connected waypoint closest to the target to path to.
	void SearchAction::makeDecision(Agent* agent)
	{
		// Fill the search actions waypoint list with the grids waypoints once
		if (m_searchWaypoints.size() == 0)
			m_searchWaypoints = agent->m_grid->m_waypoints;

		// If a search waypoint is yet to be set, determine which is closest to the target and set it as the current waypoint
		if (m_currentWaypoint == nullptr)
		{
			m_currentWaypoint = m_searchWaypoints.front();
			for (auto waypoint : m_searchWaypoints)
			{
				// If this waypoint is closer to the target than the current closest, set it as the new current closest
				if (waypoint->m_position.Distance(agent->m_target->m_position) < m_currentWaypoint->m_position.Distance(agent->m_target->m_position))
				{
					m_currentWaypoint = waypoint;
				}
			}
		}

		// If we are at the waypoint cell, search through this waypoint's connections and find the next waypoint that is currently closest to this agents target
		else if (agent->m_position.Distance(m_currentWaypoint->m_position) < 0.5f)
		{
			Waypoint* currentClosest = m_currentWaypoint->adjacentWaypoints.front();
			for (auto adjacentWaypoint : m_currentWaypoint->adjacentWaypoints)
			{
				// If this adjacent waypoint is closer than the current closest, set it as the new current closest
				if (adjacentWaypoint->m_position.Distance(agent->m_target->m_position) < currentClosest->m_position.Distance(agent->m_target->m_position))
				{
					currentClosest = adjacentWaypoint;
				}
			}
			m_currentWaypoint = currentClosest;
		}

		// Set the agents path to be from itself to its current waypoint cell
		agent->m_path = agent->m_grid->aStar(agent->m_grid->getCell(agent->m_position), agent->m_grid->getCell(m_currentWaypoint->m_position));
	}

	// GatherAction is a leaf action that contains the gather logic for the enemy agent's AI.
	// The function will iterate through the resources contained in the agents resource list,
	// find the one closest to the agent, and then create a path from the agent to the resource.
	void GatherAction::makeDecision(Agent* agent)
	{
		Resource* closestResource = agent->m_resourceList.front();
		float currentDistanceToClosest = closestResource->m_position.DistanceSqr(agent->m_position);

		// Find the closest resource node to the agent
		for (auto resource : agent->m_resourceList)
		{
			float potentialDistanceToClosest = resource->m_position.DistanceSqr(agent->m_position);
			if (potentialDistanceToClosest < currentDistanceToClosest)
			{
				closestResource = resource;
				currentDistanceToClosest = closestResource->m_position.DistanceSqr(agent->m_position);
			}
		}

		// Get the cell of the closest resource found and create a path from the agent to it
		Cell* closestResourceCell = agent->m_grid->getCell(closestResource->m_position);
		agent->m_path = agent->m_grid->aStar(agent->m_grid->getCell(agent->m_position), closestResourceCell);
	}

	// If the condition check returns true, then this function will call makeDecision on the true decision branch,
	// and call makeDecision on the false decision branch otherwise
	void TrueFalseDecision::makeDecision(Agent* agent)
	{
		if (m_condition->conditionCheck(agent))
		{
			m_trueBranch->makeDecision(agent);
		}
		else
		{
			m_falseBranch->makeDecision(agent);
		}
	}
}