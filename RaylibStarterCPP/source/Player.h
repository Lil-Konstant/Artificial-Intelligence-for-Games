#pragma once
#include "Agent.h"

class Player :
    public Agent
{
public:
	Player(Grid* grid);
	virtual ~Player();

	// For unit traversal
	static Player* m_leader;
	static std::vector<Player*> m_playerUnits;

	void Update(float deltaTime) override;
	void Draw() override;

	bool SeekBehaviour(Cell* target);
	bool ArrivalBehaviour(Cell* target);
	bool CohesionBehaviour() override;
	bool SeparationBehaviour() override;

	void AddUnit() override;

	float m_arrivalRadius = 150;
};

