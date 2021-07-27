#pragma once
#include "Agent.h"
#include <iostream>

namespace DecisionTree
{
	class Condition
	{
	public:
		virtual bool conditionCheck(Agent* agent) = 0;
	};

	// Returns true if the agents target is within the agents range
	class InRange : public Condition
	{
		virtual bool conditionCheck(Agent* agent)
		{
			float distance = agent->m_position.Distance(agent->m_target->m_position);

			return distance < agent->m_detectionRadius;
		}
	};
	
	// Returns true if the agent has more troop strength than its target
	class IsStronger : public Condition
	{
		virtual bool conditionCheck(Agent* agent)
		{
			// Compare the unit count of the player with that of the AI
			return agent->m_unitCount > agent->m_target->m_unitCount;

			// return agent->GetUnitCount() > agent->m_target->GetUnitCount() + 2;
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
			std::cout << "I AM PURSUING" << std::endl;
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
	public:
		virtual void makeDecision(Agent* agent)
		{
			std::cout << "I AM FLEEING" << std::endl;

			// Create a vector that points away from the target, to the edge of this agents flee radius
			Vec3 fleeDirection = agent->m_position - agent->m_target->m_position;
			fleeDirection = fleeDirection.GetNormalised() * agent->m_detectionRadius;
			// Get the cell at this position for the agent to path to
			Cell* fleeCell = agent->m_grid->getCell(agent->m_position + fleeDirection);

			agent->m_path = agent->m_grid->aStar(agent->m_grid->getCell(agent->m_position), fleeCell);
		}
	};

	// Decision leaf node that executes the agents Search logic
	class SearchAction : public Decision
	{
	private:
		Cell* m_currentWaypointCell = nullptr;

	public:
		virtual void makeDecision(Agent* agent)
		{
			std::cout << "I AM SEARCHING" << std::endl;

			// If a search waypoint is yet to be set, determine which is closest and set it as the target waypoint
			if (m_currentWaypointCell == nullptr)
			{
				// Find the waypoint closest to the agent, starting with the top left as the current best guess
				m_currentWaypointCell = agent->m_grid->topLeftWaypoint;
				float currentShortest = agent->m_position.DistanceSqr(m_currentWaypointCell->m_position);

				// If the top right waypoint is closer
				if (agent->m_position.DistanceSqr(agent->m_grid->topRightWaypoint->m_position) < currentShortest)
				{
					m_currentWaypointCell = agent->m_grid->topRightWaypoint;
					currentShortest = agent->m_position.DistanceSqr(m_currentWaypointCell->m_position);
				}
				// If the bottom left waypoint is closer
				if (agent->m_position.DistanceSqr(agent->m_grid->bottomLeftWaypoint->m_position) < currentShortest)
				{
					m_currentWaypointCell = agent->m_grid->bottomLeftWaypoint;
					currentShortest = agent->m_position.DistanceSqr(m_currentWaypointCell->m_position);
				}
				// If the bottom right waypoint is closer
				if (agent->m_position.DistanceSqr(agent->m_grid->bottomRightWaypoint->m_position) < currentShortest)
				{
					m_currentWaypointCell = agent->m_grid->bottomRightWaypoint;
					currentShortest = agent->m_position.DistanceSqr(m_currentWaypointCell->m_position);
				}
			}

			// If we are at the waypoint cell, update it to the appropriate next waypoint (rotating clockwise)
			if (agent->m_grid->getCell(agent->m_position) == m_currentWaypointCell)
			{
				// If we have reached the top left waypoint, set our current waypoint to be the top right waypoint
				if (m_currentWaypointCell == agent->m_grid->topLeftWaypoint)
				{
					m_currentWaypointCell = agent->m_grid->topRightWaypoint;
				}
				// If we have reached the top right waypoint, set our current waypoint to be the bottom right waypoint
				else if (m_currentWaypointCell == agent->m_grid->topRightWaypoint)
				{
					m_currentWaypointCell = agent->m_grid->bottomRightWaypoint;
				}
				// If we have reached the bottom right waypoint, set our current waypoint to be the bottom left waypoint
				else if (m_currentWaypointCell == agent->m_grid->bottomRightWaypoint)
				{
					m_currentWaypointCell = agent->m_grid->bottomLeftWaypoint;
				}
				// If we have reached the bottom left waypoint, set our current waypoint to be the top left waypoint
				else if (m_currentWaypointCell == agent->m_grid->bottomLeftWaypoint)
				{
					m_currentWaypointCell = agent->m_grid->topLeftWaypoint;
				}
			}

			// Set the agents path to be from itself to its current waypoint cell
			agent->m_path = agent->m_grid->aStar(agent->m_grid->getCell(agent->m_position), m_currentWaypointCell);
		}
	};
	
	// Decision leaf node that executes the agents Gather logic
	class GatherAction : public Decision
	{
	public:
		virtual void makeDecision(Agent* agent)
		{
			std::cout << "I AM GATHERING" << std::endl;
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