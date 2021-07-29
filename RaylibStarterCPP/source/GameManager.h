#pragma once
#include "Player.h"
#include "EnemyAgent.h"

using namespace MathsClasses;

class GameManager
{
private:
	Grid* m_grid = nullptr;

public:
	int m_screenSize;
	bool m_debugMode = false;

	// Pointers to each armies leaders used to call various function on each class
	Player* m_playerLeader = nullptr;
	EnemyAgent* m_enemyLeader = nullptr;

	// Intialisation functions
	void Init(bool debugMode, float screenSize);
	void InitialiseTerrain();
	
	// Iteratively call update on every agent in each army
	void UpdatePlayerArmy(float deltaTime);
	void UpdateEnemyArmy(float deltaTime);
	
	void Draw();
	void DebugMechanics();
};