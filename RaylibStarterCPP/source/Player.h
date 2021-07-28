#pragma once
#include "Agent.h"

class Player :
    public Agent
{
public:
	Player(Grid* grid, float radius);
	virtual ~Player();

	// For unit traversal
	static Player* m_leader;
	static std::vector<Player*> m_playerUnits;

	void Update(float deltaTime) override;
	void Draw() override;
	Agent* FindClosest(Agent* agent) override;

	void AttackSequence(float deltaTime) override;

	// Initialise this agents state as moving
	static State m_state;
	static bool m_armyDefeated;
	Agent* m_attackTarget = nullptr;

	bool m_leaderDeleted = false;

	bool CohesionBehaviour() override;
	bool SeparationBehaviour() override;

	void AddUnit() override;
	void KillUnit() override;

	void UpdateMotion(float deltaTime) override;

	bool TryCollision(GameObject* other) override { return true; }

	int GetUnitCount() override { return m_playerUnits.size(); }
};

