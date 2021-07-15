#pragma once
#include "Cell.h"

class Edge
{
public:
	Edge() {};
	~Edge() {};

	Cell* m_target;
	float m_cost;
};