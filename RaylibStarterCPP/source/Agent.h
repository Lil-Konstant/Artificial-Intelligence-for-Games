#pragma once
#include "GameObject.h"
#include "Grid.h"
#include <vector>

using namespace MathsClasses;

class Agent : public GameObject
{
public:
	Agent(Grid* grid, float radius) : GameObject(radius), m_grid(grid) {};
	virtual ~Agent();

	std::vector<Cell*> m_path;
	Grid* m_grid;

	Vec3 m_velocity = Vec3(0, 0, 0);
	Vec3 m_force = Vec3(0, 0, 0);

	static std::vector<Resource*> m_resourceList;
	// Stored in the leader agent only
	Agent* m_target = nullptr;

	void AttemptCollectResource();
	virtual void UpdateMotion(float deltaTime) = 0;
	virtual Agent* FindClosest(Agent* agent) = 0;

	virtual void AddUnit() = 0;
	virtual void KillUnit() = 0;

	// Small state machine to track whether agents should follow their path or attack agents
	enum class State
	{
		STATE_MOVE,
		STATE_ATTACK
	};

	bool TakeDamage(float damage);

	float m_health = 100;
	float m_damage = 10;
	// Range for switching to attack state on a target
	float m_aggroRange = 20;
	// Range for being able to actually damage a target
	float m_attackRange = 0.5f * m_aggroRange;
	float m_attackCooldown = 1;

	// Radius for AI agents player-in-range checks
	float m_detectionRadius = 200;

	// Pointer to the agent unit that this agent unit will attempt to attack during attack state
	Agent* m_attackTarget = nullptr;

	Cell* m_currentCell = nullptr;

	virtual int GetUnitCount() = 0;

protected:
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	virtual bool TryCollision(GameObject* other) = 0;

	// Behaviours for keeping the agent's army together
	void FollowPath();
	bool SeekBehaviour(Cell* target);
	bool SeekBehaviour(Agent* target);
	bool ArrivalBehaviour(Cell* target);
	virtual bool CohesionBehaviour() = 0;
	virtual bool SeparationBehaviour() = 0;
	virtual void AttackSequence(float deltaTime) = 0;
	
	void UpdateMoveSpeed();

	void AddForce(Vec3 force) { m_force = force + m_force; }
	Vec3 Truncate(Vec3 vector, float truncateMax);

	float m_frictionModifier = 0.99;
	
	float m_maxSpeed = 50;
	// Use to scale move speed down with unit count
	float m_currentMoveSpeed = 50;
	
	float m_cohesionForce = 5;
	
	float m_separationForce = 30;
	float m_arrivalRadius = 100;
};