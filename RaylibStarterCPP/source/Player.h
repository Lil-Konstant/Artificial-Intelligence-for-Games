#pragma once
#include "Agent.h"
#include "MouseFollowBehaviour.h"

class Player :
    public Agent
{
public:
	Player();
	virtual ~Player();

	void Draw() override;

	MouseFollowBehaviour* mouseFollowBehaviour;
};

