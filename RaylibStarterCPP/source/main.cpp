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
    //srand(time(nullptr));

    Grid* grid = new Grid();
    
    // Create the leader unit, initialise it's starting position and reserve 100 spaces in the units array
    Player* playerLeader = new Player(grid, 10);
    playerLeader->m_position = Vec3(screenSize / 2 + (rand() % 100), screenSize / 2 + (rand() % 100), 0);;
    playerLeader->m_playerUnits.reserve(100);
    
    // Initialise an army of 10 units (including the leader)
    for (int i = 0; i < 1; i++)
    {
        Player* unit = new Player(grid, 10);
        Vec3 position = Vec3(playerLeader->m_position.x + (rand() % 100), playerLeader->m_position.y + (rand() % 100), 0);
        unit->m_position = position;
    }

    // Create the enemy unit, initialise it's starting position and reserve 100 spaces in the units array
    EnemyAgent* enemyLeader = new EnemyAgent(playerLeader, grid, 10);
    enemyLeader->m_position = Vec3(GetScreenWidth() / 3, GetScreenHeight() / 3, 0);
    enemyLeader->m_enemyUnits.reserve(100);
    playerLeader->m_target = enemyLeader;

    // Create the resource nodes for the map and add them to their cells resource lists
    for (int i = 0; i < 10; i++)
    {
        Resource* resource = new Resource(10);
        // Choose a random cell to place the resource at the centre of
        Cell* startingCell = grid->getCell(Vec3(rand() % screenSize, rand() % screenSize, 0));
        startingCell->m_resource = resource;
        resource->m_position = startingCell->m_position;
        // Add the resource to the agents shared resource list
        enemyLeader->m_resourceList.push_back(resource);
    }
    
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        // Update the forces on each of the player units this frame
        // Also check if each player unit is within aggro range of the enemy leader
        for (auto unit : playerLeader->m_playerUnits)
        {
            unit->Update(deltaTime);

            // If this unit is in aggro range of the enemy leader, change the enemy leader's army into attack state
            if (unit->m_position.Distance(enemyLeader->m_position) < enemyLeader->m_aggroRange && enemyLeader->m_state == Agent::State::STATE_MOVE)
            {
                enemyLeader->m_state = Agent::State::STATE_ATTACK;
            }
        }
        
        // Update the forces on each of the enemy units this frame
        for (auto unit : enemyLeader->m_enemyUnits)
        {
            unit->Update(deltaTime);

            // If this unit is in aggro range of the enemy leader, change the enemy leader's army into attack state
            if (unit->m_position.Distance(playerLeader->m_position) < playerLeader->m_aggroRange && playerLeader->m_state == Agent::State::STATE_MOVE)
            {
                playerLeader->m_state = Agent::State::STATE_ATTACK;
            }
        }

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
            playerLeader->AddUnit();
        }
        
        // If shift is pressed, spawn an enemy unit
        if (IsKeyPressed(KEY_LEFT_SHIFT))
        {
            enemyLeader->AddUnit();
        }
        
        // If is pressed, swap unit strength
        if (IsKeyPressed(KEY_Q))
        {
            enemyLeader->m_unitCount *= -1;
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
            playerLeader->m_grid->getCell(playerLeader->m_position)->Draw(true);
            enemyLeader->m_grid->getCell(enemyLeader->m_position)->Draw(true);
            for (int i = 0; i < playerLeader->m_path.size(); i++)
            {
                if (i + 1 < playerLeader->m_path.size())
                    playerLeader->m_path[i]->Draw(true, playerLeader->m_path[i + 1]);
                else
                    playerLeader->m_path[i]->Draw(true, playerLeader->m_path[i]);
            }
            for (int i = 0; i < enemyLeader->m_path.size(); i++)
            {
                if (i + 1 < enemyLeader->m_path.size())
                    enemyLeader->m_path[i]->Draw(true, enemyLeader->m_path[i + 1]);
                else
                    enemyLeader->m_path[i]->Draw(true, enemyLeader->m_path[i]);
            }
            DrawFPS(10, 10);
        }

        // Draw every unit in the player army
        for (auto unit : playerLeader->m_playerUnits)
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