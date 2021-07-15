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
    int screenSize = 500;

    InitWindow(screenSize, screenSize, "AI Demonstration - Ronan Richardson s210424");

    Grid* grid = new Grid();
    Player* player = new Player(grid);
    EnemyAgent* enemy = new EnemyAgent(player, grid);
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float deltaTime = GetFrameTime();
        // Update
        //----------------------------------------------------------------------------------
        player->Update(deltaTime);
        Cell* playerCell = player->m_grid->getCell(player->m_position);
        enemy->Update(deltaTime);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKBLUE);
        grid->Draw();
        playerCell->Draw(true);
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