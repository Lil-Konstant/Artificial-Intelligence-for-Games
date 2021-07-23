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
    
    // Create the leader unit, initialise it's starting position and reserve 100 spaces in the units array
    Player* leader = new Player(grid, 10);
    leader->m_position = Vec3(screenSize / 2 + (rand() % 100), screenSize / 2 + (rand() % 100), 0);;
    leader->m_playerUnits.reserve(100);
    
    // Initialise an army of 10 units (including the leader)
    for (int i = 0; i < 1; i++)
    {
        Player* unit = new Player(grid, 10);
        Vec3 position = Vec3(leader->m_position.x + (rand() % 100), leader->m_position.y + (rand() % 100), 0);
        unit->m_position = position;
    }

    // Create the resource nodes for the map and add them to their cells resource lists
    for (int i = 0; i < 10; i++)
    {
        Resource* resource = new Resource(10);
        resource->m_position = Vec3(rand() % screenSize, rand() % screenSize, 0);
        grid->getCell(resource->m_position)->m_resource = resource;
    }

    // Create the enemy unit, initialise it's starting position and reserve 100 spaces in the units array
    EnemyAgent* enemyLeader = new EnemyAgent(leader, grid, 10);
    enemyLeader->m_position = Vec3(GetScreenWidth() / 3, GetScreenHeight() / 3, 0);
    enemyLeader->m_enemyUnits.reserve(100);

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
        
        // Update the forces on each of the enemy units this frame
        for (auto unit : enemyLeader->m_enemyUnits)
            unit->Update(deltaTime);

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
        
        // If shift is pressed, spawn an enemy unit
        if (IsKeyPressed(KEY_LEFT_SHIFT))
        {
            enemyLeader->AddUnit();
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
            enemyLeader->m_grid->getCell(enemyLeader->m_position)->Draw(true);
            for (int i = 0; i < leader->m_path.size(); i++)
            {
                if (i + 1 < leader->m_path.size())
                    leader->m_path[i]->Draw(true, leader->m_path[i + 1]);
                else
                    leader->m_path[i]->Draw(true, leader->m_path[i]);
            }
            /*for (int i = 0; i < enemyLeader->m_path.size(); i++)
            {
                if (i + 1 < enemyLeader->m_path.size())
                    enemyLeader->m_path[i]->Draw(true, enemyLeader->m_path[i + 1]);
                else
                    enemyLeader->m_path[i]->Draw(true, enemyLeader->m_path[i]);
            }*/
            DrawFPS(10, 10);
        }

        // Draw every unit in the player army
        for (auto unit : leader->m_playerUnits)
            unit->Draw();
        // Draw every unit in the enemy army
        for (auto unit : enemyLeader->m_enemyUnits)
            unit->Draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}