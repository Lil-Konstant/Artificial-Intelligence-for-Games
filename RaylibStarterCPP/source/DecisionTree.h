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

			return distance < 50;
		}
	};
	
	// Returns true if the agent has more troop strength than its target
	class IsStronger : public Condition
	{
		virtual bool conditionCheck(Agent* agent)
		{
			// Compare the unit count of the player with that of the AI
			return agent->m_unitCount > agent->m_target->m_unitCount;
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
	public:
		virtual void makeDecision(Agent* agent)
		{
			std::cout << "I AM FLEEING" << std::endl;
		}
	};

	// Decision leaf node that executes the agents Search logic
	class SearchAction : public Decision
	{
	public:
		virtual void makeDecision(Agent* agent)
		{
			std::cout << "I AM SEARCHING" << std::endl;
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

				for (auto resource : agent->m_resourceList)
				{
					float potentialDistanceToClosest = resource->m_position.DistanceSqr(agent->m_position);
					if (potentialDistanceToClosest < currentDistanceToClosest)
					{
						closestResource = resource;
						currentDistanceToClosest = closestResource->m_position.DistanceSqr(agent->m_position);
					}
				}

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