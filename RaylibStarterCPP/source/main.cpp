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

    std::vector<Cell*> path;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        player->Update(deltaTime);
        Cell* playerCell = player->m_grid->getCell(player->m_position);
        enemy->Update(deltaTime);

        // Create a path to the selected cell
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            Vec3 mousePosition = Vec3(GetMouseX(), GetMouseY(), 0);
            Cell* destinationCell = grid->getCell(mousePosition);

            path = grid->aStar(playerCell, destinationCell);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKBLUE);
        grid->Draw();
        playerCell->Draw(true);

        for (int i = 0; i < path.size(); i++)
        {
            path[i]->Draw(true, path[i + 1]);
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