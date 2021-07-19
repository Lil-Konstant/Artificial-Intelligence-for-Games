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
    int screenSize = 1000;
    InitWindow(screenSize, screenSize, "AI Demonstration - Ronan Richardson s210424");

    Grid* grid = new Grid();
    Player* player = new Player(grid);
    EnemyAgent* enemy = new EnemyAgent(player, grid);
    bool debugMode = true;
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        
        // Update the players position with inputs and then get their current cell
        player->Update(deltaTime);
        // Update the enemy's position with their decision tree
        enemy->Update(deltaTime);

        // Paint the cell as untraversable
        if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        {
            Vec3 mousePosition = Vec3(GetMouseX(), GetMouseY(), 0);
            Cell* cell = grid->getCell(mousePosition);

            cell->m_traversable = !cell->m_traversable;
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
            player->m_grid->getCell(player->m_position)->Draw(true);
            for (int i = 0; i < player->m_path.size(); i++)
            {
                if (i + 1 < player->m_path.size())
                    player->m_path[i]->Draw(true, player->m_path[i + 1]);
                else
                    player->m_path[i]->Draw(true, player->m_path[i]);
            }
            DrawFPS(10, 10);
        }

        player->Draw();
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