#include "GameManager.h"
#include <chrono>

// Init first initialises a raylib window with the inputted screen size. It then will call
// 3 intialisation functions to set up the terrain, grid edges, and grid waypoints (this is
// done here as they require the screen size to be setup). The function then sets up the player
// and enemy leaders which act as the first units in both armies, and places them as opposite
// sides of the map. The function finally sets up 10 resource nodes and places them randomly
// around the map.
void GameManager::Init(bool debugMode, float screenSize)
{
    m_debugMode = debugMode;
    m_screenSize = screenSize;

    InitWindow(m_screenSize, m_screenSize, "AI Demonstration - Ronan Richardson s210424");
    SetTargetFPS(60);

    m_grid = new Grid();
    
    InitialiseTerrain();
    // Initialise the cells, edges and waypoints of the grid after terrain has been setup
    m_grid->InitialiseEdges();
    m_grid->InitialiseWaypoints();

    // Seed random for object placement
    srand(time(nullptr));

    // Create the player leader unit, initialise it's starting position at the bottom right and reserve 10 spaces in the units array
    m_playerLeader = new Player(m_grid, 10);
    m_playerLeader->m_position = m_grid->m_gridArray[m_grid->NUM_CELLS - 2][m_grid->NUM_CELLS - 2].m_position;
    m_playerLeader->m_playerUnits.reserve(10);

    // Create the enemy unit, initialise it's starting position at the top left and reserve 10 spaces in the units array
    m_enemyLeader = new EnemyAgent(m_playerLeader, m_grid, 10);
    m_enemyLeader->m_position = m_grid->m_gridArray[1][1].m_position;
    m_enemyLeader->m_enemyUnits.reserve(10);
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

        // Place the resource in the cells resource pointer, and update the resources position with the cell's
        placementCell->m_resource = resource;
        resource->m_position = placementCell->m_position;
        
        // Add the resource to the agents shared resource list
        m_enemyLeader->m_resourceList.push_back(resource);
    }
}

// InitialiseTerrain will call set terrain on the grid to set up border walls around the map, as well as 5
// obstacle blocks for agents to pathfind around.
void GameManager::InitialiseTerrain()
{
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
}

// UpdatePlayerArmy will iteratively call update on every player unit in the player army so long as the
// enemy's armyDefeated flag is still false (this means that once the enemy army is defeated, the player
// no longer has control and agents aren't updated). This function also handles the deletion of enemy
// leader units, as the GameManger loses track of it's enemyLeader pointer otherwise. This function
// also checks if any of the player units is within range of the enemy leader, and if so sets the 
// enemy army into an attack state.
void GameManager::UpdatePlayerArmy(float deltaTime)
{
    // Update the forces on each of the player units this frame if the enemy army is yet to be defeated
    // Also check if each player unit is within aggro range of the enemy leader
    for (int i = 0; i < m_playerLeader->GetUnitCount() && m_enemyLeader->m_armyDefeated == false; i++)
    {
        // If the enemy leader was killed but there are more enemies left
        if (m_enemyLeader->m_leaderDeleted == true)
        {
            // Delete the unit and repoint the GameManagers enemy leader pointer
            EnemyAgent* temp = m_enemyLeader->m_leader;
            delete m_enemyLeader;
            m_enemyLeader = temp;
        }

        m_playerLeader->m_playerUnits[i]->Update(deltaTime);

        // If this unit is in aggro range of the enemy leader, change the enemy's army into attack state
        if (m_playerLeader->m_playerUnits[i]->m_position.Distance(m_enemyLeader->m_position) < m_enemyLeader->m_aggroRange && m_enemyLeader->m_state == Agent::State::STATE_MOVE)
        {
            m_enemyLeader->m_state = Agent::State::STATE_ATTACK;
        }
    }
}

// UpdateEnemyArmy will iteratively call update on every enemy unit in the enemy army so long as the
// players's armyDefeated flag is still false (this means that once the player army is defeated, the enemy
// AI no longer has control and agents aren't updated). This function also handles the deletion of player
// leader units, as the GameManger loses track of it's playerLeader pointer otherwise. This function
// also checks if any of the enemy units is within range of the player leader, and if so sets the 
// player army into an attack state.
void GameManager::UpdateEnemyArmy(float deltaTime)
{
    // Update the forces on each of the enemy units this frame if the player army is yet to be defeated
    // Also check if each enemy unit is within aggro range of the player leader
    for (int i = 0; i < m_enemyLeader->GetUnitCount() && m_playerLeader->m_armyDefeated == false; i++)
    {
        // If the player leader was killed by this unit but there are more enemies left
        if (m_playerLeader->m_leaderDeleted == true)
        {
            // Delete the unit and repoint the player leader pointer
            Player* temp = m_playerLeader->m_leader;
            delete m_playerLeader;
            m_playerLeader = temp;
        }

        m_enemyLeader->m_enemyUnits[i]->Update(deltaTime);

        // If this unit is in aggro range of the player leader, change the player's army into attack state
        if (m_enemyLeader->m_enemyUnits[i]->m_position.Distance(m_playerLeader->m_position) < m_playerLeader->m_aggroRange && m_playerLeader->m_state == Agent::State::STATE_MOVE)
        {
            m_playerLeader->m_state = Agent::State::STATE_ATTACK;
        }
    }
}

// Draw will draw 1 of 3 screens. If the enemy army has been defeated, it will continually draw the player win screen. If
// the player army has been defeated, it will continually draw the enemy win screen. Finally, if no win state has been reached,
// the function just calls draw on the grid to draw the map, and then calls draw on each of the player and enemy units (if in debug
// mode, the function will also draw out the enemy's current path).
void GameManager::Draw()
{
    BeginDrawing();
    ClearBackground(DARKGREEN);

    // If the final enemy unit was killed during player update phase, display player win text and prompt exit
    if (m_enemyLeader->m_armyDefeated == true)
    {
        // Draw every unit in the player army
        for (auto unit : m_playerLeader->m_playerUnits)
            unit->Draw();
        // Draw every unit in the enemy army
        for (auto unit : m_enemyLeader->m_enemyUnits)
            unit->Draw();

        DrawText(TextFormat("You Win!"), m_screenSize / 2 - 100, 30, 60, BLACK);
        DrawText("Press Esc To Exit...", 300, 90, 25, BLACK);
    }

    // If the final player unit was killed during enemy update phase, display enemy win text and prompt exit
    else if (m_playerLeader->m_armyDefeated == true)
    {
        // Draw every unit in the player army
        for (auto unit : m_playerLeader->m_playerUnits)
            unit->Draw();
        // Draw every unit in the enemy army
        for (auto unit : m_enemyLeader->m_enemyUnits)
            unit->Draw();

        DrawText(TextFormat("You Lose!"), m_screenSize / 2 - 130, 30, 60, BLACK);
        DrawText("Press Esc To Exit...", 300, 100, 25, BLACK);
    }

    // Otherwise draw the screen as normal
    else
    {
        // Draw the grid, if in debug mode draw the cells centres and their edges
        m_grid->Draw(m_debugMode);
        DrawFPS(10, 10);

        // Draw every unit in the enemy army
        for (auto unit : m_enemyLeader->m_enemyUnits)
            unit->Draw();
        // Draw every unit in the player army
        for (auto unit : m_playerLeader->m_playerUnits)
            unit->Draw();

        // If in debug mode, draw out the enemy's path
        if (m_debugMode)
        {
            // First draw a line from the enemy leader to the first cell in its path
            if (m_enemyLeader->m_path.size() > 0)
            {
                DrawLine(m_enemyLeader->m_position.x, m_enemyLeader->m_position.y, m_enemyLeader->m_path.front()->m_position.x, m_enemyLeader->m_path.front()->m_position.y, YELLOW);
            }
            // Then for each cell in the path, call draw on it and pass the next cell in the path as the target parameter
            for (int i = 0; i < m_enemyLeader->m_path.size(); i++)
            {
                if (i + 1 < (int)m_enemyLeader->m_path.size())
                    m_enemyLeader->m_path[i]->Draw(false, m_enemyLeader->m_path[i + 1]);
                else
                    m_enemyLeader->m_path[i]->Draw(false, m_enemyLeader->m_path[i]);
            }
        }
    }

    EndDrawing();
}

// DebugMechanics checks for button presses allowing for toggling cell traversability, as well as
// spawning player and enemy units. This function is only called the main() update if debug mode is
// on.
void GameManager::DebugMechanics()
{
    // If middle clicking, toggle the cell's traversability
    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
    {
        Vec3 mousePosition = Vec3(GetMouseX(), GetMouseY(), 0);
        Cell* cell = m_grid->getCell(mousePosition);

        cell->m_traversable = !cell->m_traversable;
    }
    // If left clicking, spawn a new player unit
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