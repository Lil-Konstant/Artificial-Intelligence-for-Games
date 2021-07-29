#pragma once
#include "Agent.h"

/// <summary>
/// Player is derived from the agent class, and contains static variables relating to the player army, as
/// well as agent overrides for various agent behaviours.
/// </summary>
class Player :
    public Agent
{
public:
	Player(Grid* grid, float radius);
	virtual ~Player();

	void Update(float deltaTime) override;
	void Draw() override;
	bool TryCollision(GameObject* other) override { return true; }

	void UpdateMotion(float deltaTime) override;
	void AttackSequence(float deltaTime) override;
	Agent* FindClosest(Agent* agent) override;
	int GetUnitCount() override { return m_playerUnits.size(); }
	
	// Behaviours for keeping army units apart
	bool CohesionBehaviour() override;
	bool SeparationBehaviour() override;

	// Increasing/decreasing army size
	void AddUnit() override;
	void KillUnit() override;

	// Army variables that are static and therefore shared between every player unit
	static Player* m_leader;
	static std::vector<Player*> m_playerUnits;
	static State m_state;
	static bool m_armyDefeated;

	bool m_leaderDeleted = false;
};

