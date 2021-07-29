#include "GameManager.h"

int main(int argc, char* argv[])
{
    // Set up and display the intro screen
    InitWindow(800, 800, "AI Demonstration - Ronan Richardson s210424");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(10, 10, 780, 780, DARKGREEN);

        // Title card
        DrawText("AI Demonstration", 75, 20, 75, BLACK);
        DrawText("Unit Warfare", 230, 100, 50, BLACK);
        
        // Intro message
        DrawText("Welcome to Unit Warfare. The aim of the game is for you the", 30, 170, 25, BLACK);
        DrawText("player to use your army of units to defeat an AI player in", 30, 195, 25, BLACK);
        DrawText("control of their own army of units. You can increase your", 30, 220, 25, BLACK);
        DrawText("army by collecting the resources scattered around the map", 30, 245, 25, BLACK);
        DrawText("(drawn as yellow circles), but keep in mind the AI can do so", 30, 270, 25, BLACK);
        DrawText("aswell. The game ends when either you or the AI run out of", 30, 295, 25, BLACK);
        DrawText("units in your army, good luck!", 30, 320, 25, BLACK);
        
        // Player controls
        DrawText("Controls:", 330, 400, 35, BLACK);
        DrawText("Movement - right click to move your army.", 180, 445, 20, BLACK);
        DrawText("Resources - move into resources to collect them.", 145, 470, 20, BLACK);
        DrawText("Attacking - units will automatically attack when in range of an enemy.", 50, 495, 20, BLACK);
        
        // Debug controls
        DrawText("Debug Controls:", 270, 550, 35, BLACK);
        DrawText("Spacebar - toggles debug mode.", 240, 595, 20, BLACK);
        DrawText("Left Mouse - adds a unit to the player army.", 170, 620, 20, BLACK);
        DrawText("Left Shift - adds a unit to the enemy army.", 180, 645, 20, BLACK);
        DrawText("Middle Mouse - toggles a tile to untraversable.", 160, 670, 20, BLACK);
        
        DrawText("Press Esc To Continue...", 250, 760, 25, RED);
        EndDrawing();
    }
    
    // Set up the main game screen
    GameManager gameManager;
    gameManager.Init(false, 800);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Toggle debug mode if space is pressed and allow for debug controls if in debug mode
        if (IsKeyPressed(KEY_SPACE))
            gameManager.m_debugMode = !gameManager.m_debugMode;
        if (gameManager.m_debugMode)
            gameManager.DebugMechanics();

        gameManager.UpdatePlayerArmy(deltaTime);
        gameManager.UpdateEnemyArmy(deltaTime);

        gameManager.Draw();
    } 
    CloseWindow();

    return 0;
}