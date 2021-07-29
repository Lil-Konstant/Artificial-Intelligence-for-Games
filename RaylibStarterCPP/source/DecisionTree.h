#pragma once
#include "Agent.h"
#include "Waypoint.h"

/// <summary>
/// The DecisionTree namespace contains the base decision class which contains a condition check method,
/// and the base decision class which contains the makeDecision method. Derived conditions are used to
/// evaluate data within the world and return either true or false for the condition being met. Leaf decisions
/// contain the actual agent logic for the decision being made (i.e. Gather, Search, Pursue and Flee). And finally
/// the TrueFalseBranch decision class contains the basic branch structure that the AI's decision tree is built up
/// with, each decision cooldown the AI will call make decision on it's root decision branch, which will then iteratively
/// call makeDecision on the appropriate branches (based on their condition checks) until a leaf decision is reached.
/// </summary>
namespace DecisionTree
{
	// Abstract condition base class, conditionCheck must return bool
	class Condition
	{
	public:
		virtual bool conditionCheck(Agent* agent) = 0;
	};

	class InPursueRange : public Condition
	{
		bool conditionCheck(Agent* agent);
	};

	class InFleeRange : public Condition
	{
		virtual bool conditionCheck(Agent* agent);
	};

	class IsStronger : public Condition
	{
		virtual bool conditionCheck(Agent* agent);
	};

	class ResourcesRemain : public Condition
	{
		virtual bool conditionCheck(Agent* agent);
	};

	// Abstract decision base class
	class Decision
	{
	public:
		virtual void makeDecision(Agent* agent) = 0;
	};

	class PursueAction : public Decision
	{
	public:
		virtual void makeDecision(Agent* agent);
	};

	class FleeAction : public Decision
	{
	private:
		Waypoint* m_currentWaypoint = nullptr;
		// The Game Manager intialises and connects waypoints on construction
		std::vector<Waypoint*> m_fleeWaypoints;

	public:
		virtual void makeDecision(Agent* agent);
	};

	class SearchAction : public Decision
	{
	private:
		Waypoint* m_currentWaypoint = nullptr;
		// The Game Manager intialises and connects search waypoints on construction
		std::vector<Waypoint*> m_searchWaypoints;

	public:
		virtual void makeDecision(Agent* agent);
	};
	
	class GatherAction : public Decision
	{
	public:
		virtual void makeDecision(Agent* agent);
	};

	// Branching decision class, uses a given condition to decide which decision to enact
	class TrueFalseDecision : public Decision
	{
	public:
		Decision* m_trueBranch;
		Decision* m_falseBranch;
		Condition* m_condition;

		TrueFalseDecision(Decision* trueBranch, Decision* falseBranch, Condition* condition) : m_trueBranch(trueBranch), m_falseBranch(falseBranch), m_condition(condition) {}
		virtual void makeDecision(Agent* agent);
	};
}