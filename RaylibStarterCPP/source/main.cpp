#include "GameManager.h"

int main(int argc, char* argv[])
{
    GameManager gameManager;
    gameManager.Init(true, 1000);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        gameManager.UpdatePlayerArmy(deltaTime);
        
        // If the final enemy unit was killed during player update phase, display player win text
        if (gameManager.m_enemyLeader->m_armyDefeated == true)
        {
            gameManager.m_debugMode = false;
            BeginDrawing;
            ClearBackground(DARKGREEN);
            DrawText(TextFormat("You Win!"), gameManager.m_screenSize / 2 - 100, 160, 60, GREEN);
            EndDrawing;
        }

        gameManager.UpdateEnemyArmy(deltaTime);

        // If the final player unit was killed during enemy update phase, display enemy win text
        if (gameManager.m_playerLeader->m_armyDefeated == true)
        {
            gameManager.m_debugMode = false;
            BeginDrawing;
            ClearBackground(DARKGREEN);
            DrawText(TextFormat("The Enemy Wins!"), gameManager.m_screenSize / 2 - 250, 160, 60, RED);
            EndDrawing;
        }

        gameManager.DebugMechanics();

        gameManager.Draw();
    } 
    CloseWindow();

    return 0;
}