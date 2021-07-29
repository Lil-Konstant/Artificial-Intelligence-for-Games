#pragma once
#include "GameObject.h"
#include "Grid.h"

using namespace MathsClasses;

/// <summary>
/// Agent is an abstract class that both the Enemy AI and player controlled agent inherit from. It contains various abstract functions
/// for manipulating and updating armies in the player and enemy classes. It also contains variables that agents use when calculating
/// both their decisions and their unit forces, as well as a small state machine that supports moving or attacking.
/// </summary>
class Agent : public GameObject
{
public:
	// Small state machine to dictate whether agents should follow their path or attack enemy agents
	enum class State
	{
		STATE_MOVE,
		STATE_ATTACK
	};

	Agent(Grid* grid, float radius) : GameObject(radius), m_grid(grid) {};
	virtual ~Agent() {};

	virtual void UpdateMotion(float deltaTime) = 0;
	virtual Agent* FindClosest(Agent* agent) = 0;
	
	virtual void KillUnit() = 0;
	bool TakeDamage(float damage);

	// Overridden in player and enemy as they have different static unit vectors 
	virtual int GetUnitCount() = 0;

	// Grid interaction variables
	Grid* m_grid;
	std::vector<Cell*> m_path;
	Cell* m_currentCell = nullptr;
	// Used for the enemy AI to gather resources if there are any left
	static std::vector<Resource*> m_resourceList;

	// Targets the other agents leader and is used for various AI and player checks
	Agent* m_target = nullptr;

	// Range for switching to attack state on a target
	float m_aggroRange = 30;

	// Radii for AI agents player-in-range checks
	float m_fleeRadius = 120;
	float m_pursueRadius = 300;

	// Pointer to the agent unit that this agent unit will attempt to attack during attack state
	Agent* m_attackTarget = nullptr;

protected:
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	virtual bool TryCollision(GameObject* other) = 0;

	virtual void AddUnit() = 0;
	void AttemptCollectResource();

	// Path movement functions
	void FollowPath();
	bool SeekBehaviour(Cell* target);

	// Behaviours for keeping the derived agent's armies together
	virtual bool CohesionBehaviour() = 0;
	virtual bool SeparationBehaviour() = 0;

	virtual void AttackSequence(float deltaTime) = 0;
	// Used for seeking towards agents during attack sequencing
	bool SeekBehaviour(Agent* target);

	// Updates the current movespeed according to the unit count
	void UpdateMoveSpeed();

	void AddForce(Vec3 force) { m_force = force + m_force; }
	Vec3 Truncate(Vec3 vector, float truncateMax);
	
	// Physics variables
	Vec3 m_velocity = Vec3(0, 0, 0);
	Vec3 m_force = Vec3(0, 0, 0);

	// Agent attacking and health stats
	float m_health = 50;
	float m_damage = 10;
	// Range for being able to actually do damage to a target
	float m_attackRange = 0.15f * m_aggroRange;
	float m_attackCooldown = 1;

	float m_maxSpeed = 60;
	// Used to scale move speed down with unit count
	float m_currentMoveSpeed = 60;
	// Used to naturally scale movespeed down over time
	float m_frictionModifier = 0.99;
	
	// Flocking forces for units
	float m_cohesionForce = 5;
	float m_separationForce = 30;
};