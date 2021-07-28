#pragma once
#include "Agent.h"
#include "SearchWaypoint.h"

namespace DecisionTree
{
	class Condition
	{
	public:
		virtual bool conditionCheck(Agent* agent) = 0;
	};

	// Returns true if the agents target is within the agents detection radius
	class InRange : public Condition
	{
		virtual bool conditionCheck(Agent* agent)
		{
			float distance = agent->m_position.Distance(agent->m_target->m_position);

			return distance < agent->m_detectionRadius;
		}
	};
	
	// Returns true if the agent has at least 2 more troop strength than its target
	class IsStronger : public Condition
	{
		virtual bool conditionCheck(Agent* agent)
		{
			// Compare the unit count of the player with that of the AI
			return agent->GetUnitCount() >= agent->m_target->GetUnitCount() + 2;
		}
	};

	// Returns true if there are resources left on the map
	class ResourcesRemain : public Condition
	{
		virtual bool conditionCheck(Agent* agent)
		{
			return agent->m_resourceList.size() > 0;
		}
	};

	// Abstract decision base class
	class Decision
	{
	public:
		virtual void makeDecision(Agent* agent) = 0;
	};

	// Decision leaf node that executes the agents Pursue logic
	class PursueAction : public Decision
	{
	public:
		virtual void makeDecision(Agent* agent)
		{
			Cell* currentCell = agent->m_currentCell;
			Cell* targetCell = agent->m_target->m_currentCell;

			std::vector<Cell*> newPath = agent->m_grid->aStar(currentCell, targetCell);
			std::vector<Cell*> pathMinusStart = newPath;
			pathMinusStart.erase(pathMinusStart.begin());  

			// If the path to the new path is different from the current path, update the current path
			if (agent->m_path != newPath && agent->m_path != pathMinusStart)
			{
				agent->m_path = newPath;
			}
		}
	};

	// Decision leaf node that executes the agents Flee logic
	class FleeAction : public Decision
	{
	private:
		SearchWaypoint* m_currentWaypoint = nullptr;

		// The Game Manager intialises and connects waypoints on construction
		std::vector<SearchWaypoint*> m_waypoints;

	public:
		virtual void makeDecision(Agent* agent)
		{
			// Fill the search actions waypoint list with the grids waypoints once
			if (m_waypoints.size() == 0)
				m_waypoints = agent->m_grid->m_searchWaypoints;

			// If we have no fleeing target, iterate through the waypoints and find the one that is in the most opposite direction to the agents target
			if (m_currentWaypoint == nullptr)
			{
				m_currentWaypoint = m_waypoints.front();
				float currentAngleBetween = 0;
				for (auto waypoint : m_waypoints)
				{
					// Do not calculate waypoints the agent is sitting on due to rounding errors (0 degrees becomes 180)
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
	};

	// Decision leaf node that executes the agents Search logic
	class SearchAction : public Decision
	{
	private:
		SearchWaypoint* m_currentWaypoint = nullptr;

		// The Game Manager intialises and connects search waypoints on construction
		std::vector<SearchWaypoint*> m_searchWaypoints;

	public:
		virtual void makeDecision(Agent* agent)
		{
			// Fill the search actions waypoint list with the grids waypoints once
			if (m_searchWaypoints.size() == 0)
				m_searchWaypoints = agent->m_grid->m_searchWaypoints;

			// If a search waypoint is yet to be set, determine which is closest and set it as the target waypoint
			if (m_currentWaypoint == nullptr)
			{
				m_currentWaypoint = m_searchWaypoints.front();
				for (auto waypoint : m_searchWaypoints)
				{
					// If this waypoint is closer than the current closest, set it as the new current closest
					if (waypoint->m_position.Distance(agent->m_position) < m_currentWaypoint->m_position.Distance(agent->m_position))
					{
						m_currentWaypoint = waypoint;
					}
				}
			}

			// If we are at the waypoint cell, search through this waypoints connections and find the waypoint that is currently closest to this agents target
			if (agent->m_position.Distance(m_currentWaypoint->m_position) < 0.5f)
			{
				SearchWaypoint* currentClosest = m_currentWaypoint->adjacentWaypoints.front();
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
	};
	
	// Decision leaf node that executes the agents Gather logic
	class GatherAction : public Decision
	{
	public:
		virtual void makeDecision(Agent* agent)
		{
			if (agent->m_resourceList.size() > 0)
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

				// Pathfind towards the closest resource node
				Cell* closestResourceCell = agent->m_grid->getCell(closestResource->m_position);
				std::vector<Cell*> newPath = agent->m_grid->aStar(agent->m_grid->getCell(agent->m_position), closestResourceCell);
				std::vector<Cell*> pathMinusStart = newPath;
				pathMinusStart.erase(pathMinusStart.begin());

				// If the path to the new path is different from the current path, update the current path
				if (agent->m_path != newPath && agent->m_path != pathMinusStart)
				{
					agent->m_path = newPath;
				}
			}
			else
			{
				// there are no resources left to collect, so roam?
			}
		}
	};

	// Generic decision branch class
	class TrueFalseDecision : public Decision
	{
	public:
		Decision* m_trueBranch;
		Decision* m_falseBranch;
		Condition* m_condition;

		TrueFalseDecision(Decision* trueBranch, Decision* falseBranch, Condition* condition) : m_trueBranch(trueBranch), m_falseBranch(falseBranch), m_condition(condition) {}

		virtual void makeDecision(Agent* agent)
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
	};
}