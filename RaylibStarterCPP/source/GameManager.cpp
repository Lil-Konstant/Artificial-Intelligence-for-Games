#include "GameManager.h"

void GameManager::Init(bool debugMode, float screenSize)
{
    m_debugMode = debugMode;
    m_screenSize = screenSize;

    InitWindow(m_screenSize, m_screenSize, "AI Demonstration - Ronan Richardson s210424");
    SetTargetFPS(60);

    m_grid = new Grid();
    
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

// True if the game has finished and the player has one
bool GameManager::UpdatePlayerArmy(float deltaTime)
{
    // Update the forces on each of the player units this frame
    // Also check if each player unit is within aggro range of the enemy leader
    for (auto unit : m_playerLeader->m_playerUnits)
    {
        unit->Update(deltaTime);

        // If the final enemy unit was killed by this player unit, trigger a player win state
        if (m_enemyLeader->m_armyDefeated == true)
        {
            WinSequence(WinState::STATE_PLAYER_WIN);
            return true;
        }

        // If the enemy leader was killed by this unit but there are more enemies left
        else if (m_enemyLeader->m_leaderDeleted == true)
        {
            // Delete the unit and repoint the enemy leader pointer
            EnemyAgent* temp = m_enemyLeader->m_leader;
            delete m_enemyLeader;
            m_enemyLeader = temp;
        }

        // If this unit is in aggro range of the enemy leader, change the enemy leader's army into attack state
        if (unit->m_position.Distance(m_enemyLeader->m_position) < m_enemyLeader->m_aggroRange && m_enemyLeader->m_state == Agent::State::STATE_MOVE)
        {
            m_enemyLeader->m_state = Agent::State::STATE_ATTACK;
        }
    }
    return false;
}

bool GameManager::UpdateEnemyArmy(float deltaTime)
{
    // Update the forces on each of the enemy units this frame
    for (auto unit : m_enemyLeader->m_enemyUnits)
    {
        unit->Update(deltaTime);

        // If the final player unit was killed by this enemy unit, trigger an enemy win state
        if (m_playerLeader->m_armyDefeated == true)
        {
            WinSequence(WinState::STATE_ENEMY_WIN);
            return true;
        }

        // If the enemy leader was killed by this unit but there are more enemies left
        else if (m_playerLeader->m_leaderDeleted == true)
        {
            // Delete the unit and repoint the enemy leader pointer
            Player* temp = m_playerLeader->m_leader;
            delete m_playerLeader;
            m_playerLeader = temp;
        }

        // If this unit is in aggro range of the player leader, change the player leader's army into attack state
        if (unit->m_position.Distance(m_playerLeader->m_position) < m_playerLeader->m_aggroRange && m_playerLeader->m_state == Agent::State::STATE_MOVE)
        {
            m_playerLeader->m_state = Agent::State::STATE_ATTACK;
        }
    }
    return false;
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
            // PLAYER WINS
            EndDrawing;
        }
        break;
    case WinState::STATE_ENEMY_WIN:
        // Enemy win sequence
        while (true)
        {
            BeginDrawing;
            // ENEMY WINS
            EndDrawing;
        }
        break;
    }
}

void GameManager::Draw()
{
    BeginDrawing();
    ClearBackground(DARKBLUE);

    // If in debug mode, draw the grid cells and their connections, as well as the players A* path
    if (m_debugMode)
    {
        m_grid->Draw();
        m_playerLeader->m_grid->getCell(m_playerLeader->m_position)->Draw(true);
        m_enemyLeader->m_grid->getCell(m_enemyLeader->m_position)->Draw(true);
        for (int i = 0; i < m_playerLeader->m_path.size(); i++)
        {
            if (i + 1 < m_playerLeader->m_path.size())
                m_playerLeader->m_path[i]->Draw(true, m_playerLeader->m_path[i + 1]);
            else
                m_playerLeader->m_path[i]->Draw(true, m_playerLeader->m_path[i]);
        }
        for (int i = 0; i < m_enemyLeader->m_path.size(); i++)
        {
            if (i + 1 < m_enemyLeader->m_path.size())
                m_enemyLeader->m_path[i]->Draw(true, m_enemyLeader->m_path[i + 1]);
            else
                m_enemyLeader->m_path[i]->Draw(true, m_enemyLeader->m_path[i]);
        }
        DrawFPS(10, 10);
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