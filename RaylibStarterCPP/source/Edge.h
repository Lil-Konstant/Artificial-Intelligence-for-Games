#pragma once

class Cell;

/// <summary>
/// The Edge class is a simple class used by the grid/cell system in the game. It contains a pointer
/// to the cell this edge connects to and a float denoting the cost of traversing the edge.
/// </summary>
class Edge
{
public:
	Edge(Cell* target, float cost) : m_target(target), m_cost(cost) {};
	~Edge() {};

	Cell* m_target;
	float m_cost;
};