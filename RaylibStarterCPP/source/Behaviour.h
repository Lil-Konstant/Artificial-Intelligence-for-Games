#pragma once
#include "Agent.h"
#include "MathsClasses.h"

class Behaviour
{
public:
	Behaviour() {}
	virtual ~Behaviour () {}

	// Abstract base function for behaviour execution
	virtual bool Update(float deltaTime) = 0;

	// The agent this behaviour gets attached to
	Agent* m_agent = nullptr;
};