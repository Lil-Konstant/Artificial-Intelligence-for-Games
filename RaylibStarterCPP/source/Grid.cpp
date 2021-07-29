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

// InitialiseEdges iterates through every cell in the array, and for each cell it first sets the cells position as the centre
// of its grid position, and then connects up the corresponding edges to this cell. Cells are connected orthogonally and diagonally,
// giving a cell a maximum of 8 possible edges. Edges that lead to untraversable cells are not connected. Orthogonal edges are given
// of traversal cost of 1, and diagonal edges are given a traversal cost of 1.4. This function is very ugly and inefficient, but as 
// it is only called once on initialisation it does not effect runtime performance.
void Grid::InitialiseEdges()
{
	for (int row = 0; row < NUM_CELLS; row++)
	{
		for (int column = 0; column < NUM_CELLS; column++)
		{
			// Make the position of the cell its centre
			m_gridArray[row][column].m_position = Vec3((column * CELL_SIZE) + (CELL_SIZE / 2), (row * CELL_SIZE) + (CELL_SIZE / 2), 0);

			// If the cell is traversable, setup its edges
			if (m_gridArray[row][column].m_traversable)
			{
				// Fill this cells edge list accordingly
				// If the left cell is within the grid array and traversable, lead an edge to it
				if (column - 1 >= 0 && m_gridArray[row][column - 1].m_traversable)
				{
					Edge left = Edge(&m_gridArray[row][column - 1], 1);
					m_gridArray[row][column].m_edges.push_back(left);
				}
				// If the top left cell is within the grid array and traversable, lead an edge to it
				if (column - 1 >= 0 && row - 1 >= 0 && m_gridArray[row - 1][column - 1].m_traversable)
				{
					Edge topLeft = Edge(&m_gridArray[row - 1][column - 1], 1.4f);
					m_gridArray[row][column].m_edges.push_back(topLeft);
				}
				// If the bottom left cell is within the grid array and traversable, lead an edge to it
				if (column - 1 >= 0 && row + 1 < NUM_CELLS && m_gridArray[row + 1][column - 1].m_traversable)
				{
					Edge bottomLeft = Edge(&m_gridArray[row + 1][column - 1], 1.4f);
					m_gridArray[row][column].m_edges.push_back(bottomLeft);
				}
				// If the right cell is within the grid array and traversable, lead an edge to it
				if (column + 1 < NUM_CELLS && m_gridArray[row][column + 1].m_traversable)
				{
					Edge right = Edge(&m_gridArray[row][column + 1], 1);
					m_gridArray[row][column].m_edges.push_back(right);
				}
				// If the top right cell is within the grid array and traversable, lead an edge to it
				if (column + 1 < NUM_CELLS && row - 1 >= 0 && m_gridArray[row - 1][column + 1].m_traversable)
				{
					Edge topRight = Edge(&m_gridArray[row - 1][column + 1], 1.4f);
					m_gridArray[row][column].m_edges.push_back(topRight);
				}
				// If the bottom right cell is within the grid array and traversable, lead an edge to it
				if (column + 1 < NUM_CELLS && row + 1 < NUM_CELLS && m_gridArray[row + 1][column + 1].m_traversable)
				{
					Edge bottomRight = Edge(&m_gridArray[row + 1][column + 1], 1.4f);
					m_gridArray[row][column].m_edges.push_back(bottomRight);
				}
				// If the top cell is within the grid array and traversable, lead an edge to it
				if (row - 1 >= 0 && m_gridArray[row - 1][column].m_traversable)
				{
					Edge top = Edge(&m_gridArray[row - 1][column], 1);
					m_gridArray[row][column].m_edges.push_back(top);
				}
				// If the bottom cell is within the grid array and traversable, lead an edge to it
				if (row + 1 < NUM_CELLS && m_gridArray[row + 1][column].m_traversable)
				{
					Edge bottom = Edge(&m_gridArray[row + 1][column], 1);
					m_gridArray[row][column].m_edges.push_back(bottom);
				}
			}
		}
	}
}

// InitialiseWaypoints sets up the 9 waypoints depicted in the TDD, connects them to eachother as they
// are depicted to be connected, finally adding these waypoints to the grids waypoint list. This function is
// called by the game manager after the screen has been initialised.
void Grid::InitialiseWaypoints()
{
	// Initialise the 9 waypoints and their positions
	Waypoint* waypoint1 = new Waypoint(10);
	waypoint1->m_position = m_gridArray[2][2].m_position;
	Waypoint* waypoint2 = new Waypoint(10);
	waypoint2->m_position = m_gridArray[2][10].m_position;
	Waypoint* waypoint3 = new Waypoint(10);
	waypoint3->m_position = m_gridArray[2][17].m_position;
	Waypoint* waypoint4 = new Waypoint(10);
	waypoint4->m_position = m_gridArray[10][17].m_position;
	Waypoint* waypoint5 = new Waypoint(10);
	waypoint5->m_position = m_gridArray[17][17].m_position;
	Waypoint* waypoint6 = new Waypoint(10);
	waypoint6->m_position = m_gridArray[17][10].m_position;
	Waypoint* waypoint7 = new Waypoint(10);
	waypoint7->m_position = m_gridArray[17][2].m_position;
	Waypoint* waypoint8 = new Waypoint(10);
	waypoint8->m_position = m_gridArray[10][2].m_position;
	Waypoint* waypoint9 = new Waypoint(10);
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
	m_waypoints.push_back(waypoint9);
	m_waypoints.push_back(waypoint8);
	m_waypoints.push_back(waypoint7);
	m_waypoints.push_back(waypoint6);
	m_waypoints.push_back(waypoint5);
	m_waypoints.push_back(waypoint4);
	m_waypoints.push_back(waypoint3);
	m_waypoints.push_back(waypoint2);
	m_waypoints.push_back(waypoint1);
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

// aStar is the A* algorithm implemented for AI and player pathfinding around the map.
// The function uses the standard A* algorithm to continually query cells with the smallest
// F-score. Cells currently being evaluated in the open list are sorted using a custom comparison
// function (shown in the .h file) with the std priority queue created as a min heap. The H-score
// is calculated using the Euler distance from the queried cell to the end cell. The function returns
// a vector list of the cells along the shortest path in order of their traversal.
std::vector<Cell*> Grid::aStar(Cell* startCell, Cell* endCell)
{
	// For invalid inputs, return an empty list
	if (startCell == nullptr || endCell == nullptr)
	{
		return std::vector<Cell*>();
	}
	// If start and end cells are the same, return a path containing just this cell
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

	// Create our open and closed lists to track progress and push the starting cell onto the open list
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

		//If the destination cell was added to the closed list, the shortest path has been found so break
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

				// If we are yet to have previously visited the target, set its g-score and f-score and update its parent pointer
				if (edge.m_target->gScore == 0 && edge.m_target->fScore == 0 && edge.m_target->m_previous == nullptr)
				{
					edge.m_target->gScore = gScore;
					edge.m_target->fScore = fScore;
					edge.m_target->m_previous = currentCell;
					openList.push(edge.m_target);
				}

				// Otherwise the cell has been visited, if the new paths fScore is shorter, then update the target
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

// SetTerrain is used to set up untraversable "terrain" within the grid map, the function takes
// a start index and an end index, and sets every cell within the x and y range of the start and end
// index as untraversable.
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

// Draw every cell in the grid (and the search waypoints if in debug mode)
void Grid::Draw(bool debugMode)
{
	for (int row = 0; row < NUM_CELLS; row++)
	{
		for (int column = 0; column < NUM_CELLS; column++)
		{
			m_gridArray[row][column].Draw(debugMode);
			// Draw the cells boundaries so the maps grid structure is visible
			DrawRectangleLines(m_gridArray[row][column].m_position.x - (CELL_SIZE / 2), m_gridArray[row][column].m_position.y - (CELL_SIZE / 2), CELL_SIZE, CELL_SIZE, BLACK);
		}
	}

	// If in debug mode, draw the search waypoints
	if (debugMode)
	{
		for (auto waypoint : m_waypoints)
		{
			waypoint->Draw();
		}
	}
}