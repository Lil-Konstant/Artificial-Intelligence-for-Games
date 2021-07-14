#pragma once
#include "Behaviour.h"
#include "raylib.h"

class MouseFollowBehaviour :
	public Behaviour
{
public:
	MouseFollowBehaviour() {};
	virtual ~MouseFollowBehaviour() {};

	virtual bool Update(float deltaTime);
	void Draw();

	Vec3 m_target = Vec3(0, 0, 0);

private:
	float m_arrivalRadius = 80.0f;
};