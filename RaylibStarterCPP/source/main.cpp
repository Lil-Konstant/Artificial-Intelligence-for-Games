#include "GameManager.h"

int main(int argc, char* argv[])
{
    GameManager gameManager;
    gameManager.Init(true, 800);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Toggle debug mode if space is pressed and allow for debug controls if in debug mode
        if (IsKeyPressed(KEY_SPACE))
            gameManager.m_debugMode = !gameManager.m_debugMode;
        if (gameManager.m_debugMode)
            gameManager.DebugMechanics();

        gameManager.UpdatePlayerArmy(deltaTime);
        
        // If the final enemy unit was killed during player update phase, display player win text
        if (gameManager.m_enemyLeader->m_armyDefeated == true)
        {
            gameManager.WinSequence(gameManager.GetPlayerWin());
        }

        gameManager.UpdateEnemyArmy(deltaTime);

        // If the final player unit was killed during enemy update phase, display enemy win text
        if (gameManager.m_playerLeader->m_armyDefeated == true)
        {
            gameManager.WinSequence(gameManager.GetEnemyWin());
        }

        gameManager.Draw();
    } 
    CloseWindow();

    return 0;
}