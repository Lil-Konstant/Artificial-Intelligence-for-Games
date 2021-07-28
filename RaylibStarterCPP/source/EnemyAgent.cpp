#include "EnemyAgent.h"

// Initialise the static leader pointer as null
EnemyAgent* EnemyAgent::m_leader = nullptr;
// Initialise the static state as moving
Agent::State EnemyAgent::m_state = Agent::State::STATE_MOVE;
// Initialise the static unit vector as null
std::vector<EnemyAgent*> EnemyAgent::m_enemyUnits = std::vector<EnemyAgent*>();
// Initialise the static armyDefeated bool as false
bool EnemyAgent::m_armyDefeated = false;

EnemyAgent::EnemyAgent(Agent* target, Grid* grid, float radius) : Agent(grid, radius)
{
	// Initialise the first enemy agent as the enemy leader
	if (EnemyAgent::m_leader == nullptr)
	{
		m_leader = this;
	}

	// Spawn this unit within 100 units of the enemy leader
	m_position = Vec3(m_leader->m_position.x + (rand() % 100), m_leader->m_position.y + (rand() % 100), 0);
	m_enemyUnits.push_back(this);

	// Set this AI's target to be the player leader
	m_target = target;

	// Construct decision tree for the enemy agent
	TrueFalseDecision* playerInteractionMode = new TrueFalseDecision(new PursueAction(), new FleeAction(), new IsStronger());
	TrueFalseDecision* npcRoamMode = new TrueFalseDecision(new SearchAction(), new GatherAction(), new IsStronger());
	m_rootDecision = new TrueFalseDecision(playerInteractionMode, npcRoamMode, new InRange());
}

EnemyAgent::~EnemyAgent()
{

}

void EnemyAgent::Update(float deltaTime)
{
	// Attempt to collect a resource if one is found in this enemy agents cell
	AttemptCollectResource();

	// Update the armies movespeed (in case units have been added to slow the army down)
	UpdateMoveSpeed();

	// Traverse the decision tree every timer tick to decide on the enemy agent's state
	decisionTimer -= deltaTime;
	if (decisionTimer <= 0 && this == m_leader)
	{
		m_rootDecision->makeDecision(this);
		decisionTimer = 0.5f;
	}
	
	// Reset the force for this frame to 0
	m_force = Vec3(0, 0, 0);

	// Either moves along the enemy path calculated in the decision phase, or engages in attack with player units
	switch (m_state)
	{
	case State::STATE_MOVE:
		// Move along the path calculated in the decision phase
		FollowPath();
		break;
	case State::STATE_ATTACK:
		m_path.clear();
		AttackSequence(deltaTime);
		break;
	}

	// If the unit we're updating isn't the leader, then calculate the cohesion and separation for flocking
	if (this != m_leader)
	{
		SeparationBehaviour();
		CohesionBehaviour();
	}

	UpdateMotion(deltaTime);
}

bool EnemyAgent::CohesionBehaviour()
{
	Vec3 leaderDisplacement = m_leader->m_position - m_position;
	Vec3 steeringForce = leaderDisplacement - m_velocity;
	AddForce(steeringForce * m_cohesionForce);

	return true;
}

bool EnemyAgent::SeparationBehaviour()
{
	Vec3 averageForce = Vec3(0, 0, 0);

	for (auto unit : m_enemyUnits)
	{
		if (unit != this)
		{
			// Calculate the vector pointing from the unit to this
			Vec3 displacementVec = m_position - unit->m_position;
			float distance = displacementVec.Magnitude();

			// If the vector is non-zero, normalise it and scale it by the repulsion factor
			if (distance != 0)
			{
				Vec3 repulsionForce = displacementVec.GetNormalised() * m_separationForce;
				repulsionForce = repulsionForce / (distance);
				// Add this neighbours repulsion force to the running force total
				averageForce = averageForce + repulsionForce;
			}
			else
				return true;
		}
	}

	AddForce(averageForce * m_separationForce);
	return true;
}

void EnemyAgent::AddUnit()
{
	EnemyAgent* newUnit = new EnemyAgent(m_target, m_grid, m_radius);
	// Spawn the new unit at a random position near the leader
	newUnit->m_position = m_leader->m_position + Vec3(rand() % 50, rand() % 50, 0) - Vec3(rand() % 50, rand() % 50, 0);
}

// "Kills" this enemy unit and removes all references of it
void EnemyAgent::KillUnit()
{
	// Remove this unit from the enemyUnits list
	m_enemyUnits.erase(std::find(m_enemyUnits.begin(), m_enemyUnits.end(), this));

	// If killing the last enemy in the enemy army, exit early for a player win state
	if (m_enemyUnits.size() == 0)
	{
		EnemyAgent::m_armyDefeated = true;
		return;
	}

	// If killing the leader unit, replace it with the next in charge
	if (this == m_leader)
	{
		// Transfer the leaders target to the new leader
		m_enemyUnits.front()->m_target = m_leader->m_target;
		// Repoint the enemy armies leader pointer to the next in charge
		m_leader = m_enemyUnits.front();

		// Replace the player leaders target pointer with the new enemy leader
		m_target->m_target = m_leader;

		// Return before deleting this as it will invalidate the pointer in the main game loop (delete it there instead)
		m_leaderDeleted = true;
		return;
	}

	// Otherwise this unit is not special and can just be deleted
	delete this;
}

// Finds the closest enemy agent in this army to the inputted agent
Agent* EnemyAgent::FindClosest(Agent* agent)
{
	Agent* closestEnemy = m_enemyUnits.front();
	float closestDistanceSqr = closestEnemy->m_position.DistanceSqr(agent->m_position);

	for (auto unit : m_enemyUnits)
	{
		// If this unit is closer to the input agent than the current closest
		if (unit->m_position.DistanceSqr(agent->m_position) < closestDistanceSqr)
		{
			closestEnemy = unit;
			closestDistanceSqr = unit->m_position.DistanceSqr(agent->m_position);
		}
	}

	return closestEnemy;
}

// Run attack procedures
void EnemyAgent::AttackSequence(float deltaTime)
{
	// Update this agents attack target with the current closest enemy
	m_attackTarget = m_leader->m_target->FindClosest(this);

	// Seek the attack target, if within attack range, then attempt to attack it if the attack cooldown has run out
	m_attackCooldown -= deltaTime;
	if (SeekBehaviour(m_attackTarget) && m_attackCooldown <= 0)
	{
		// Damages this agents attack target and is true if the target is now dead
		if (m_attackTarget->TakeDamage(m_damage))
		{
			// Destroy the target and remove all references to it
			m_attackTarget->KillUnit();

			// Reset this units attack target to null as it is now dead
			m_attackTarget = nullptr;
		}

		// Reset the attack cooldown back to 1 second
		m_attackCooldown = 1;
	}
}

void EnemyAgent::UpdateMotion(float deltaTime)
{
	if (this == m_leader)
	{
		// Set the velocity equal to the force for linear movement along the path
		m_velocity = Truncate((m_force * deltaTime * m_currentMoveSpeed), m_currentMoveSpeed);
	}	
	else
	{
		// Add Force multiplied by delta time to Velocity, truncate with the max speed to not over speed
		m_velocity = Truncate(m_velocity + (m_force * deltaTime), m_currentMoveSpeed);
	}

	// Add Velocity multiplied by delta time to Position
	m_position = m_position + (m_velocity * deltaTime);
	// Scale the velocity down according to the friction
	m_velocity = m_velocity * m_frictionModifier;
}

void EnemyAgent::Draw()
{
	if (this == m_leader)
	{
		// Draw out the enemy's path
		if (m_path.size() > 0)
			DrawLine(m_position.x, m_position.y, m_path.front()->m_position.x, m_path.front()->m_position.y, YELLOW);
		for (int i = 0; i < m_path.size(); i++)
		{
			if (i + 1 < (int)m_path.size())
				m_path[i]->Draw(false, true, m_path[i + 1]);
			else
				m_path[i]->Draw(false, true, m_path[i]);
		}

		// Draw the enemy leader in black
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, BLACK);
	}
	else
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, RED);

	DrawPolyLines({ m_position.x, m_position.y }, 6, m_radius, 0, BLACK);
}