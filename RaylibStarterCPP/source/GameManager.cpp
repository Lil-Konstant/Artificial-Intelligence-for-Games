#include "GameManager.h"
#include <chrono>

void GameManager::Init(bool debugMode, float screenSize)
{
    m_debugMode = debugMode;
    m_screenSize = screenSize;

    InitWindow(m_screenSize, m_screenSize, "AI Demonstration - Ronan Richardson s210424");
    SetTargetFPS(60);

    m_grid = new Grid();
    
    m_grid->SetTerrain(Vec3(1, 1, 0), Vec3(5, 5, 0));

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
            // Delete the unit and repoint the enemy leader pointer
            Player* temp = m_playerLeader->m_leader;
            delete m_playerLeader;
            m_playerLeader = temp;
        }

        m_enemyLeader->m_enemyUnits[i]->Update(deltaTime);

        // If this unit is in aggro range of the player leader, change the player leader's army into attack state
        if (m_enemyLeader->m_position.Distance(m_playerLeader->m_position) < m_playerLeader->m_aggroRange && m_playerLeader->m_state == Agent::State::STATE_MOVE)
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
        // Player win sequence
        while (true)
        {
            BeginDrawing;
            ClearBackground(DARKBLUE);
            DrawText(TextFormat("Player 1 Wins!"), 200, 160, 40, BLUE);
            EndDrawing;
            if (IsKeyPressed(KEY_ESCAPE)) { break; }
        }
        break;
    case WinState::STATE_ENEMY_WIN:
        // Enemy win sequence
        while (true)
        {
            BeginDrawing;
            ClearBackground(DARKBLUE);
            DrawText(TextFormat("Player 1 Wins!"), 200, 160, 40, BLUE);
            EndDrawing;
            if (IsKeyPressed(KEY_ESCAPE)) { break; }
        }
        break;
    }
}

void GameManager::Draw()
{
    BeginDrawing();
    ClearBackground(DARKGREEN);

    m_grid->Draw(m_debugMode);
    DrawFPS(10, 10);

    // Draw out the player and enemies paths
    /*m_playerLeader->m_grid->getCell(m_playerLeader->m_position)->Draw(true);
    m_enemyLeader->m_grid->getCell(m_enemyLeader->m_position)->Draw(true);*/
    for (int i = 0; i < m_playerLeader->m_path.size(); i++)
    {
        if (i + 1 < (int)m_playerLeader->m_path.size())
            m_playerLeader->m_path[i]->Draw(m_debugMode, true, m_playerLeader->m_path[i + 1]);
        else
            m_playerLeader->m_path[i]->Draw(m_debugMode, true, m_playerLeader->m_path[i]);
    }
    for (int i = 0; i < m_enemyLeader->m_path.size(); i++)
    {
        if (i + 1 < (int)m_enemyLeader->m_path.size())
            m_enemyLeader->m_path[i]->Draw(m_debugMode, true, m_enemyLeader->m_path[i + 1]);
        else
            m_enemyLeader->m_path[i]->Draw(m_debugMode, true, m_enemyLeader->m_path[i]);
    }

    // Draw every unit in the player army
    for (auto unit : m_playerLeader->m_playerUnits)
        unit->Draw();
    // Draw every unit in the enemy army
    for (auto unit : m_enemyLeader->m_enemyUnits)
        unit->Draw();

    EndDrawing();
}

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
    // If is pressed, swap unit strength
    if (IsKeyPressed(KEY_Q))
    {
        m_enemyLeader->m_unitCount *= -1;
    }
    // Toggle debug mode
    if (IsKeyPressed(KEY_SPACE))
    {
        m_debugMode = !m_debugMode;
    }
}