#include "Grid.h"

Grid::Grid()
{
	for (int row = 0; row < NUM_CELLS; row++)
	{
		for (int column = 0; column < NUM_CELLS; column++)
		{
			// Make the position of the cell its centre
			m_gridArray[row][column].m_position = Vec3((column * CELL_SIZE) + (CELL_SIZE / 2), (row * CELL_SIZE) + (CELL_SIZE / 2), 0);

			// Fill this cells edge list accordingly
		}
	}
}

Grid::~Grid()
{

}

// Spacially indexes the grid map to return the grid cell of the inputted vector position
Cell* Grid::getCell(Vec3 position)
{
	int columnIndex = (int)(position.x / CELL_SIZE);
	int rowIndex = (int)(position.y / CELL_SIZE);

	Cell* cell = &m_gridArray[rowIndex][columnIndex];
	return cell;
}

// Visually represent the cells in the grid
void Grid::Draw()
{
	for (int row = 0; row < NUM_CELLS; row++)
	{
		for (int column = 0; column < NUM_CELLS; column++)
		{
			m_gridArray[row][column].Draw();
			Vec3 lineStart = Vec3((column * CELL_SIZE) + CELL_SIZE, 0, 0);
			Vec3 lineEnd = Vec3((column * CELL_SIZE) + CELL_SIZE, GetScreenHeight(), 0);
			DrawLine(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, BLACK);
		}
		Vec3 lineStart = Vec3(0, (row * CELL_SIZE) + CELL_SIZE, 0);
		Vec3 lineEnd = Vec3(GetScreenWidth(), (row * CELL_SIZE) + CELL_SIZE, 0);
		DrawLine(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, BLACK);
	}
}