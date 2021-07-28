#include "Grid.h"

// The grid constructor simply makes sure that the cell class records the same cell size after the screen has been calculated
Grid::Grid()
{
	CELL_SIZE = GetScreenHeight() / NUM_CELLS;
	Cell::CELL_SIZE = CELL_SIZE;
}

Grid::~Grid()
{

}

void Grid::InitialiseEdges()
{
	for (int row = 0; row < NUM_CELLS; row++)
	{
		for (int column = 0; column < NUM_CELLS; column++)
		{
			// Make the position of the cell its centre
			m_gridArray[row][column].m_position = Vec3((column * CELL_SIZE) + (CELL_SIZE / 2), (row * CELL_SIZE) + (CELL_SIZE / 2), 0);

			// If the cell is traversable, setup its properties
			if (m_gridArray[row][column].m_traversable)
			{
				// Fill this cells edge list accordingly
				// If the left cell is within the grid array and traversable
				if (column - 1 >= 0 && m_gridArray[row][column - 1].m_traversable)
				{
					Edge left = Edge(&m_gridArray[row][column - 1], 1);
					m_gridArray[row][column].m_edges.push_back(left);
				}
				// If the top left cell is within the grid array and traversable
				if (column - 1 >= 0 && row - 1 >= 0 && m_gridArray[row - 1][column - 1].m_traversable)
				{
					Edge topLeft = Edge(&m_gridArray[row - 1][column - 1], 1.4f);
					m_gridArray[row][column].m_edges.push_back(topLeft);
				}
				// If the bottom left cell is within the grid array and traversable
				if (column - 1 >= 0 && row + 1 < NUM_CELLS && m_gridArray[row + 1][column - 1].m_traversable)
				{
					Edge bottomLeft = Edge(&m_gridArray[row + 1][column - 1], 1.4f);
					m_gridArray[row][column].m_edges.push_back(bottomLeft);
				}
				// If the right cell is within the grid array and traversable
				if (column + 1 < NUM_CELLS && m_gridArray[row][column + 1].m_traversable)
				{
					Edge right = Edge(&m_gridArray[row][column + 1], 1);
					m_gridArray[row][column].m_edges.push_back(right);
				}
				// If the top right cell is within the grid array and traversable
				if (column + 1 < NUM_CELLS && row - 1 >= 0 && m_gridArray[row - 1][column + 1].m_traversable)
				{
					Edge topRight = Edge(&m_gridArray[row - 1][column + 1], 1.4f);
					m_gridArray[row][column].m_edges.push_back(topRight);
				}
				// If the bottom right cell is within the grid array and traversable
				if (column + 1 < NUM_CELLS && row + 1 < NUM_CELLS && m_gridArray[row + 1][column + 1].m_traversable)
				{
					Edge bottomRight = Edge(&m_gridArray[row + 1][column + 1], 1.4f);
					m_gridArray[row][column].m_edges.push_back(bottomRight);
				}
				// If the top cell is within the grid array and traversable
				if (row - 1 >= 0 && m_gridArray[row - 1][column].m_traversable)
				{
					Edge top = Edge(&m_gridArray[row - 1][column], 1);
					m_gridArray[row][column].m_edges.push_back(top);
				}
				// If the bottom cell is within the grid array and traversable
				if (row + 1 < NUM_CELLS && m_gridArray[row + 1][column].m_traversable)
				{
					Edge bottom = Edge(&m_gridArray[row + 1][column], 1);
					m_gridArray[row][column].m_edges.push_back(bottom);
				}
			}
		}
	}

	// Initalise the 9 waypoints and connect them together appropriately
	InitialiseWaypoints();
}

void Grid::InitialiseWaypoints()
{
	// Initialise the 9 waypoints and their positions
	SearchWaypoint* waypoint1 = new SearchWaypoint(10);
	waypoint1->m_position = m_gridArray[2][2].m_position;
	SearchWaypoint* waypoint2 = new SearchWaypoint(10);
	waypoint2->m_position = m_gridArray[2][10].m_position;
	SearchWaypoint* waypoint3 = new SearchWaypoint(10);
	waypoint3->m_position = m_gridArray[2][17].m_position;
	SearchWaypoint* waypoint4 = new SearchWaypoint(10);
	waypoint4->m_position = m_gridArray[10][17].m_position;
	SearchWaypoint* waypoint5 = new SearchWaypoint(10);
	waypoint5->m_position = m_gridArray[17][17].m_position;
	SearchWaypoint* waypoint6 = new SearchWaypoint(10);
	waypoint6->m_position = m_gridArray[17][10].m_position;
	SearchWaypoint* waypoint7 = new SearchWaypoint(10);
	waypoint7->m_position = m_gridArray[17][2].m_position;
	SearchWaypoint* waypoint8 = new SearchWaypoint(10);
	waypoint8->m_position = m_gridArray[10][2].m_position;
	SearchWaypoint* waypoint9 = new SearchWaypoint(10);
	waypoint9->m_position = m_gridArray[10][10].m_position;

	// Add waypoint 1's connections
	waypoint1->adjacentWaypoints.push_back(waypoint2);
	waypoint1->adjacentWaypoints.push_back(waypoint8);
	waypoint1->adjacentWaypoints.push_back(waypoint9);

	// Add waypoint 2's connections
	waypoint2->adjacentWaypoints.push_back(waypoint1);
	waypoint2->adjacentWaypoints.push_back(waypoint3);
	waypoint2->adjacentWaypoints.push_back(waypoint9);

	// Add waypoint 3's connections
	waypoint3->adjacentWaypoints.push_back(waypoint2);
	waypoint3->adjacentWaypoints.push_back(waypoint4);
	waypoint3->adjacentWaypoints.push_back(waypoint9);

	// Add waypoint 4's connections
	waypoint4->adjacentWaypoints.push_back(waypoint3);
	waypoint4->adjacentWaypoints.push_back(waypoint5);
	waypoint4->adjacentWaypoints.push_back(waypoint9);

	// Add waypoint 5's connections
	waypoint5->adjacentWaypoints.push_back(waypoint4);
	waypoint5->adjacentWaypoints.push_back(waypoint6);
	waypoint5->adjacentWaypoints.push_back(waypoint9);

	// Add waypoint 6's connections
	waypoint6->adjacentWaypoints.push_back(waypoint5);
	waypoint6->adjacentWaypoints.push_back(waypoint7);
	waypoint6->adjacentWaypoints.push_back(waypoint9);

	// Add waypoint 7's connections
	waypoint7->adjacentWaypoints.push_back(waypoint6);
	waypoint7->adjacentWaypoints.push_back(waypoint8);
	waypoint7->adjacentWaypoints.push_back(waypoint9);

	// Add waypoint 8's connections
	waypoint8->adjacentWaypoints.push_back(waypoint1);
	waypoint8->adjacentWaypoints.push_back(waypoint7);
	waypoint8->adjacentWaypoints.push_back(waypoint9);

	// Add waypoint 9's connections
	waypoint9->adjacentWaypoints.push_back(waypoint1);
	waypoint9->adjacentWaypoints.push_back(waypoint2);
	waypoint9->adjacentWaypoints.push_back(waypoint3);
	waypoint9->adjacentWaypoints.push_back(waypoint4);
	waypoint9->adjacentWaypoints.push_back(waypoint5);
	waypoint9->adjacentWaypoints.push_back(waypoint6);
	waypoint9->adjacentWaypoints.push_back(waypoint7);
	waypoint9->adjacentWaypoints.push_back(waypoint8);

	// Add the search waypoints to the grid's search waypoint list
	m_searchWaypoints.push_back(waypoint9);
	m_searchWaypoints.push_back(waypoint8);
	m_searchWaypoints.push_back(waypoint7);
	m_searchWaypoints.push_back(waypoint6);
	m_searchWaypoints.push_back(waypoint5);
	m_searchWaypoints.push_back(waypoint4);
	m_searchWaypoints.push_back(waypoint3);
	m_searchWaypoints.push_back(waypoint2);
	m_searchWaypoints.push_back(waypoint1);
}

// Spacially indexes the grid map to return the grid cell of the inputted vector position
Cell* Grid::getCell(Vec3 position)
{
	Cell* cell = nullptr;

	int columnIndex = (int)(position.x / CELL_SIZE);
	int rowIndex = (int)(position.y / CELL_SIZE);
	if (columnIndex < NUM_CELLS && rowIndex < NUM_CELLS)
		cell = &m_gridArray[rowIndex][columnIndex];
	
	return cell;
}

std::vector<Cell*> Grid::aStar(Cell* startCell, Cell* endCell)
{
	// Validate the input
	if (startCell == nullptr || endCell == nullptr)
	{
		return std::vector<Cell*>();
	}
	if (startCell == endCell)
	{
		std::vector<Cell*> singleNodePath;
		singleNodePath.push_back(startCell);
		return singleNodePath;
	}

	// Clear the previous A* calculations from the grid cells
	for (int row = 0; row < NUM_CELLS; row++)
	{
		for (int column = 0; column < NUM_CELLS; column++)
		{
			m_gridArray[row][column].gScore = 0;
			m_gridArray[row][column].fScore = 0;
			m_gridArray[row][column].m_previous = nullptr;
		}
	}

	// Create our open and closed lists to track progress
	std::priority_queue<Cell*, std::vector<Cell*>, CellCompare> openList;
	std::vector<Cell*> closedList;

	openList.push(startCell);

	Cell* currentCell = nullptr;
	while (!openList.empty())
	{
		// Grab the next lowest f-score cell from the priority queue
		currentCell = openList.top();
		// Move the current cell from the open list to the closed list
		openList.pop();
		closedList.push_back(currentCell);

		//If the destination cell was added to the closed list, the shortest path has been found
		if (currentCell == endCell)
		{
			break;
		}

		// For each cell connected to the current cell
		for (auto edge : currentCell->m_edges)
		{
			// If the edge target is yet to be processed and put in the closed list, and is also traversable, then attempt to process it
			if (std::find(closedList.begin(), closedList.end(), edge.m_target) == closedList.end() && edge.m_target->m_traversable)
			{
				// Calculate the g-score from the current node to the edge target, and the f-score of the target
				float gScore = currentCell->gScore + edge.m_cost;
				float fScore = gScore + edge.m_target->m_position.Distance(endCell->m_position);

				// If we are yet to visit the target, set its g-score and f-score and update its parent pointer
				if (edge.m_target->gScore == 0 && edge.m_target->fScore == 0 && edge.m_target->m_previous == nullptr)
				{
					edge.m_target->gScore = gScore;
					edge.m_target->fScore = fScore;
					edge.m_target->m_previous = currentCell;
					openList.push(edge.m_target);
				}

				// Otherwise the cell has been visited, if the new path fScore is shorter, then update the target
				else if (fScore < edge.m_target->fScore)
				{
					edge.m_target->gScore = gScore;
					edge.m_target->fScore = fScore;
					edge.m_target->m_previous = currentCell;
				}
			}
		}
	}

	// The shortest path has now been created, so back track and create a forward path list
	std::vector<Cell*> path;
	currentCell = endCell;

	while (currentCell != nullptr)
	{
		path.insert(path.begin(), currentCell);
		currentCell = currentCell->m_previous;
	}

	return path;
}

void Grid::SetTerrain(Vec3 startIndex, Vec3 endIndex)
{
	for (int row = startIndex.y; row <= endIndex.y; row++)
	{
		for (int column = startIndex.x; column <= endIndex.x; column++)
		{
			m_gridArray[row][column].m_traversable = false;
		}
	}
}

// Visually represent the cells in the grid (and the search waypoints if in debug mode)
void Grid::Draw(bool debugMode)
{
	for (int row = 0; row < NUM_CELLS; row++)
	{
		for (int column = 0; column < NUM_CELLS; column++)
		{
			m_gridArray[row][column].Draw(debugMode);
			// Draw the edges of the cell
			DrawRectangleLines(m_gridArray[row][column].m_position.x - (CELL_SIZE / 2), m_gridArray[row][column].m_position.y - (CELL_SIZE / 2), CELL_SIZE, CELL_SIZE, BLACK);
		}
	}

	// If in debug mode, draw the search waypoints
	if (debugMode)
	{
		for (auto waypoint : m_searchWaypoints)
		{
			waypoint->Draw();
		}
	}
}