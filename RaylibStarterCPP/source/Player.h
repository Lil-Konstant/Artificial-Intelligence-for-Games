#pragma once
#include "Agent.h"
#include "MouseFollowBehaviour.h"

class Player :
    public Agent
{
public:
	Player(Grid* grid);
	virtual ~Player();

	void Draw() override;

	MouseFollowBehaviour* mouseFollowBehaviour;
};

