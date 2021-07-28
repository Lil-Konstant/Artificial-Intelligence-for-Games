#include "GameManager.h"
#include <chrono>

void GameManager::Init(bool debugMode, float screenSize)
{
    m_debugMode = debugMode;
    m_screenSize = screenSize;

    InitWindow(m_screenSize, m_screenSize, "AI Demonstration - Ronan Richardson s210424");
    SetTargetFPS(60);

    m_grid = new Grid();
    
    // Border walls
    m_grid->SetTerrain(Vec3(0, 0, 0), Vec3(m_grid->NUM_CELLS - 1, 0, 0));
    m_grid->SetTerrain(Vec3(0, 1, 0), Vec3(0, m_grid->NUM_CELLS - 1, 0));
    m_grid->SetTerrain(Vec3(m_grid->NUM_CELLS - 1, 1, 0), Vec3(m_grid->NUM_CELLS - 1, m_grid->NUM_CELLS - 1, 0));
    m_grid->SetTerrain(Vec3(0, m_grid->NUM_CELLS - 1, 0), Vec3(m_grid->NUM_CELLS - 1, m_grid->NUM_CELLS - 1, 0));

    // Block 1
    m_grid->SetTerrain(Vec3(3, 3, 0), Vec3(4, 4, 0));
    // Block 2
    m_grid->SetTerrain(Vec3(13, 13, 0), Vec3(13, 16, 0));
    m_grid->SetTerrain(Vec3(14, 12, 0), Vec3(14, 15, 0));
    m_grid->SetTerrain(Vec3(15, 11, 0), Vec3(15, 14, 0));
    // Block 3
    m_grid->SetTerrain(Vec3(3, 16, 0), Vec3(7, 16, 0));
    m_grid->SetTerrain(Vec3(5, 17, 0), Vec3(7, 17, 0));
    // Block 4
    m_grid->SetTerrain(Vec3(14, 4, 0), Vec3(15, 7, 0));
    // Block 5
    m_grid->SetTerrain(Vec3(4, 7, 0), Vec3(6, 7, 0));
    m_grid->SetTerrain(Vec3(4, 8, 0), Vec3(4, 9, 0));

    // Initialise the cells and edges of the grid after terrain has been setup
    m_grid->InitialiseEdges();

    // Seed random for object placement
    srand(time(nullptr));

    // Create the player leader unit, initialise it's starting position and reserve 100 spaces in the units array
    m_playerLeader = new Player(m_grid, 10);
    m_playerLeader->m_position = Vec3(m_screenSize / 2 + (rand() % 100), m_screenSize / 2 + (rand() % 100), 0);;
    m_playerLeader->m_playerUnits.reserve(100);

    // Initialise a player army of 10 units (including the leader)
    for (int i = 0; i < 1; i++)
    {
        Player* unit = new Player(m_grid, 10);
        Vec3 position = Vec3(m_playerLeader->m_position.x + (rand() % 100), m_playerLeader->m_position.y + (rand() % 100), 0);
        unit->m_position = position;
    }

    // Create the enemy unit, initialise it's starting position and reserve 100 spaces in the units array
    m_enemyLeader = new EnemyAgent(m_playerLeader, m_grid, 10);
    m_enemyLeader->m_position = Vec3(GetScreenWidth() / 3, GetScreenHeight() / 3, 0);
    m_enemyLeader->m_enemyUnits.reserve(100);
    // Give the player leader the enemy leader as a target
    m_playerLeader->m_target = m_enemyLeader;

    // Create the resource nodes for the map and add them to their cells resource pointers
    for (int i = 0; i < 10; i++)
    {
        Resource* resource = new Resource(10);
        
        // Choose a random cell to place the resource at the centre of
        Cell* placementCell = m_grid->getCell(Vec3(rand() % m_screenSize, rand() % m_screenSize, 0));
        // Make sure the resource isn't placed inside an untraversable cell
        while (!placementCell->m_traversable)
            placementCell = m_grid->getCell(Vec3(rand() % m_screenSize, rand() % m_screenSize, 0));

        placementCell->m_resource = resource;
        resource->m_position = placementCell->m_position;
        // Add the resource to the agents shared resource list
        m_enemyLeader->m_resourceList.push_back(resource);
    }
}

// 
void GameManager::UpdatePlayerArmy(float deltaTime)
{
    // Update the forces on each of the player units this frame if the enemy army is yet to be defeated
    // Also check if each player unit is within aggro range of the enemy leader
    for (int i = 0; i < m_playerLeader->GetUnitCount() && m_enemyLeader->m_armyDefeated == false; i++)
    {
        // If the enemy leader was killed by this unit but there are more enemies left
        if (m_enemyLeader->m_leaderDeleted == true)
        {
            // Delete the unit and repoint the enemy leader pointer
            EnemyAgent* temp = m_enemyLeader->m_leader;
            delete m_enemyLeader;
            m_enemyLeader = temp;
        }

        m_playerLeader->m_playerUnits[i]->Update(deltaTime);

        // If this unit is in aggro range of the enemy leader, change the enemy leader's army into attack state
        if (m_playerLeader->m_playerUnits[i]->m_position.Distance(m_enemyLeader->m_position) < m_enemyLeader->m_aggroRange && m_enemyLeader->m_state == Agent::State::STATE_MOVE)
        {
            m_enemyLeader->m_state = Agent::State::STATE_ATTACK;
        }
    }
}

void GameManager::UpdateEnemyArmy(float deltaTime)
{
    for (int i = 0; i < m_enemyLeader->GetUnitCount() && m_playerLeader->m_armyDefeated == false; i++)
    {
        // If the enemy leader was killed by this unit but there are more enemies left
        if (m_playerLeader->m_leaderDeleted == true)
        {
            // Delete the unit and repoint the player leader pointer
            Player* temp = m_playerLeader->m_leader;
            delete m_playerLeader;
            m_playerLeader = temp;
        }

        m_enemyLeader->m_enemyUnits[i]->Update(deltaTime);

        // If this unit is in aggro range of the enemy leader, change the enemy leader's army into attack state
        if (m_enemyLeader->m_enemyUnits[i]->m_position.Distance(m_playerLeader->m_position) < m_playerLeader->m_aggroRange && m_playerLeader->m_state == Agent::State::STATE_MOVE)
        {
            m_playerLeader->m_state = Agent::State::STATE_ATTACK;
        }
    }
}

// Run the win sequence corresponding to either a player or enemy win
void GameManager::WinSequence(WinState winState)
{
    switch (winState)
    {
    case WinState::STATE_PLAYER_WIN:
        m_debugMode = false;
        BeginDrawing;
        ClearBackground(DARKGREEN);
        DrawText(TextFormat("You Win!"), m_screenSize / 2 - 100, 160, 60, GREEN);
        EndDrawing;
        break;
    case WinState::STATE_ENEMY_WIN:
        m_debugMode = false;
        BeginDrawing;
        ClearBackground(DARKGREEN);
        DrawText(TextFormat("The Enemy Wins!"), m_screenSize / 2 - 250, 160, 60, RED);
        EndDrawing;
        break;
    }
}

// Calls draw on the grid which draws every cell on the map (and their connections if in debug mode), and then
// calls draw on every unit in both the player and enemy armies
void GameManager::Draw()
{
    BeginDrawing();
    ClearBackground(DARKGREEN);

    // Draw the grid, if in debug mode draw the cells centres and their edges
    m_grid->Draw(m_debugMode);
    DrawFPS(10, 10);

    // Draw every unit in the player army
    for (auto unit : m_playerLeader->m_playerUnits)
        unit->Draw();
    // Draw every unit in the enemy army
    for (auto unit : m_enemyLeader->m_enemyUnits)
        unit->Draw();

    EndDrawing();
}

// 
void GameManager::DebugMechanics()
{
    // Paint the cell as untraversable
    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
    {
        Vec3 mousePosition = Vec3(GetMouseX(), GetMouseY(), 0);
        Cell* cell = m_grid->getCell(mousePosition);

        cell->m_traversable = !cell->m_traversable;
    }
    // If left clicking, spawn a new unit
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        m_playerLeader->AddUnit();
    }
    // If shift is pressed, spawn an enemy unit
    if (IsKeyPressed(KEY_LEFT_SHIFT))
    {
        m_enemyLeader->AddUnit();
    }
    
}