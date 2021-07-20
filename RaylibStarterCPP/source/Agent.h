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
	std::vector<Cell*> m_path;

	std::vector<Behaviour*> m_behaviourList;

	Vec3 m_position = Vec3(0, 0, 0);
	Vec3 m_velocity = Vec3(0, 0, 0);
	Vec3 m_force = Vec3(0, 0, 0);

	Agent* m_target = nullptr;
	float m_unitCount = 0;

	Agent(Grid* grid);
	virtual ~Agent();

protected:
	virtual void Update(float deltaTime);
	virtual void Draw();

	void AddBehaviour(Behaviour* behaviour);
	// Behaviours for keeping the agent's army together
	virtual bool CohesionBehaviour() = 0;
	virtual bool SeparationBehaviour() = 0;
	
	virtual void AddUnit() = 0;
	//DeleteUnit()
	
	void AddForce(Vec3 force) { m_force = force + m_force; }
	Vec3 Truncate(Vec3 vector, float truncateMax);

	float m_frictionModifier = 0.99;
	float m_maxSpeed = 50;
	float m_cohesionForce = 5;
	float m_separationForce = 200;
};