#include "raylib.h"
#include "MathsClasses.h"
#include "Grid.h"
#include "Player.h"
#include "EnemyAgent.h"

using namespace MathsClasses;

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    bool debugMode = true;
    int screenSize = 1000;
    InitWindow(screenSize, screenSize, "AI Demonstration - Ronan Richardson s210424");
    SetTargetFPS(60);

    srand(time(nullptr));

    Grid* grid = new Grid();
    Player* leader = new Player(grid);
    Vec3 position = Vec3(screenSize / 2 + (rand() % 100), screenSize / 2 + (rand() % 100), 0);
    leader->m_position = position;
    EnemyAgent* enemy = new EnemyAgent(leader, grid);
    for (int i = 0; i < 10; i++)
    {
        Player* unit = new Player(grid);
        position = Vec3(screenSize / 2 + (rand() % 100), screenSize / 2 + (rand() % 100), 0);
        unit->m_position = position;
    }

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        // Update the forces on each of the player units this frame
        for (auto unit : leader->m_playerUnits)
            unit->Update(deltaTime);
        // Update the enemy's position with their decision tree
        enemy->Update(deltaTime);

        // Paint the cell as untraversable
        if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        {
            Vec3 mousePosition = Vec3(GetMouseX(), GetMouseY(), 0);
            Cell* cell = grid->getCell(mousePosition);

            cell->m_traversable = !cell->m_traversable;
        }
        // If left clicking, spawn a new unit
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            leader->AddUnit();
        }
        // Toggle debug mode
        if (IsKeyPressed(KEY_SPACE))
        {
            debugMode = !debugMode;
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(DARKBLUE);
        
        // If in debug mode, draw the grid cells and their connections, as well as the players A* path
        if (debugMode)
        {
            grid->Draw();
            leader->m_grid->getCell(leader->m_position)->Draw(true);
            for (int i = 0; i < leader->m_path.size(); i++)
            {
                if (i + 1 < leader->m_path.size())
                    leader->m_path[i]->Draw(true, leader->m_path[i + 1]);
                else
                    leader->m_path[i]->Draw(true, leader->m_path[i]);
            }
            DrawFPS(10, 10);
        }

        for (auto unit : leader->m_playerUnits)
            unit->Draw();
        enemy->Draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}