#include "GameManager.h"

int main(int argc, char* argv[])
{
    GameManager gameManager;
    gameManager.Init(true, 1000);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (gameManager.UpdatePlayerArmy(deltaTime))
            break;
        if (gameManager.UpdateEnemyArmy(deltaTime))
            break;

        gameManager.DebugMechanics();

        gameManager.Draw();
    } 
    CloseWindow();

    return 0;
}