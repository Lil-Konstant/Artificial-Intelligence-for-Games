#pragma once
#include "Player.h"
#include "EnemyAgent.h"

using namespace MathsClasses;

class GameManager
{
private:
	bool m_debugMode = false;
	int m_screenSize = 500;

	Grid* m_grid = nullptr;

	Player* m_playerLeader = nullptr;
	EnemyAgent* m_enemyLeader = nullptr;

	enum class WinState
	{
		STATE_PLAYER_WIN,
		STATE_ENEMY_WIN
	};

public:
	void Init(bool debugMode, float screenSize);
	bool UpdatePlayerArmy(float deltaTime);
	bool UpdateEnemyArmy(float deltaTime);
	void WinSequence(WinState winState);
	void Draw();
	void DebugMechanics();
};