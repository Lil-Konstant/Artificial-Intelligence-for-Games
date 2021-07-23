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
	bool TryCollision(GameObject* other) override { return true; }

	bool CohesionBehaviour() override;
	bool SeparationBehaviour() override;

	void AddUnit() override;
};

