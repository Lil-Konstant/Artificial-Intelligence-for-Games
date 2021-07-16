#pragma once

class Cell;
class Edge
{
public:
	Edge(Cell* target, float cost) : m_target(target), m_cost(cost) {};
	~Edge() {};

	Cell* m_target;
	float m_cost;
};