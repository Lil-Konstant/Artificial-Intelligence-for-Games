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

			return distance < 100;
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
	class PursureAction : public Decision
	{
	public:
		virtual void makeDecision(Agent* agent)
		{
			std::cout << "I AM PURSUING" << std::endl;
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