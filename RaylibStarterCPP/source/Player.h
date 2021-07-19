#pragma once
#include "Agent.h"
#include "MouseFollowBehaviour.h"

class Player :
    public Agent
{
public:
	Player(Grid* grid);
	virtual ~Player();

	void Update(float deltaTime) override;
	void Draw() override;

	bool Seek(Cell* target);
	bool Arrive(Cell* target);

	MouseFollowBehaviour* mouseFollowBehaviour;

	float m_arrivalRadius = 150;
};

