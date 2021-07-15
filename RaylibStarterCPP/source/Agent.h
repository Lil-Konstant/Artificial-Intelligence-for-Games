#pragma once
#include "raylib.h"
#include "MathsClasses.h"
#include "Grid.h"
#include <vector>

using namespace MathsClasses;

class Behaviour;
class Agent
{
public:
	Grid* m_grid;

	std::vector<Behaviour*> m_behaviourList;

	Vec3 m_position = Vec3(0, 0, 0);
	Vec3 m_velocity = Vec3(0, 0, 0);
	Vec3 m_force = Vec3(0, 0, 0);

	Agent* m_target = nullptr;
	float m_unitCount = 0;

	float m_frictionModifier = 0.98;
	float m_maxSpeed = 150;

	Agent(Grid* grid);
	virtual ~Agent();

	virtual void Update(float deltaTime);
	virtual void Draw();

	// virtual void AddUnit() { unitcount++, new unit }

protected:
	void AddBehaviour(Behaviour* behaviour);
	Vec3 Truncate(Vec3 vector, float truncateMax);
};