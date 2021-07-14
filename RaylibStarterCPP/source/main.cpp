#include "raylib.h"
#include "MathsClasses.h"
#include "Player.h"
#include "EnemyAgent.h"

using namespace MathsClasses;

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "AI Demonstration - Ronan Richardson s210424");

    Player* player = new Player();
    EnemyAgent* enemy = new EnemyAgent(player);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float deltaTime = GetFrameTime();
        // Update
        //----------------------------------------------------------------------------------
        player->Update(deltaTime);
        enemy->Update(deltaTime);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKBLUE);
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