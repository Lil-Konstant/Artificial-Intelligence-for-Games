#pragma once
#include "Player.h"
#include "EnemyAgent.h"

using namespace MathsClasses;

class GameManager
{
private:
	Grid* m_grid = nullptr;

	enum class WinState
	{
		STATE_PLAYER_WIN,
		STATE_ENEMY_WIN
	};

public:
	int m_screenSize;
	bool m_debugMode = false;
	
	WinState GetPlayerWin() { return WinState::STATE_PLAYER_WIN; }
	WinState GetEnemyWin() { return WinState::STATE_ENEMY_WIN; }

	Player* m_playerLeader = nullptr;
	EnemyAgent* m_enemyLeader = nullptr;

	void Init(bool debugMode, float screenSize);
	void UpdatePlayerArmy(float deltaTime);
	void UpdateEnemyArmy(float deltaTime);
	void WinSequence(WinState winState);
	void Draw();
	void DebugMechanics();
};