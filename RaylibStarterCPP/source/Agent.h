#pragma once
#include "GameObject.h"
#include "Grid.h"
#include <vector>

using namespace MathsClasses;

class Behaviour;
class Agent : public GameObject
{
public:
	Agent(Grid* grid, float radius) : GameObject(radius), m_grid(grid) {};
	virtual ~Agent();

	std::vector<Cell*> m_path;
	Grid* m_grid;

	std::vector<Behaviour*> m_behaviourList;

	Vec3 m_velocity = Vec3(0, 0, 0);
	Vec3 m_force = Vec3(0, 0, 0);

	Agent* m_target = nullptr;
	float m_unitCount = 0;

	virtual void AddUnit() = 0;
	//DeleteUnit()

	Cell* m_currentCell = nullptr;

protected:
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	virtual bool TryCollision(GameObject* other) = 0;

	void AddBehaviour(Behaviour* behaviour);
	
	// Behaviours for keeping the agent's army together
	bool SeekBehaviour(Cell* target);
	bool ArrivalBehaviour(Cell* target);
	virtual bool CohesionBehaviour() = 0;
	virtual bool SeparationBehaviour() = 0;
	
	void AddForce(Vec3 force) { m_force = force + m_force; }
	Vec3 Truncate(Vec3 vector, float truncateMax);

	float m_frictionModifier = 0.99;
	float m_maxSpeed = 50;
	float m_cohesionForce = 5;
	float m_separationForce = 30;
	float m_arrivalRadius = 150;
};