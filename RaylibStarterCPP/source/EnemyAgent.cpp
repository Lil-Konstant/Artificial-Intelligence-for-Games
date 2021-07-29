#include "EnemyAgent.h"

// Initialise the static leader pointer as null
EnemyAgent* EnemyAgent::m_leader = nullptr;
// Initialise the static state as moving
Agent::State EnemyAgent::m_state = Agent::State::STATE_MOVE;
// Initialise the static unit vector as null
std::vector<EnemyAgent*> EnemyAgent::m_enemyUnits = std::vector<EnemyAgent*>();
// Initialise the static armyDefeated bool as false
bool EnemyAgent::m_armyDefeated = false;

// The EnemyAgent constructor takes input of its radius, the grid its being placed on, as well as 
// an agent target (which is always the player leader). The constructor then sets this armies leader
// pointer to this unit if it's the first unit being created. The function sets this enemy's position
// to be a random position within 20 units of the leader, and adds this unit to the enemyUnits vector.
// Finally this function sets up the AI decision tree used for choosing which action the enemy leader
// should enact every decision tick.
EnemyAgent::EnemyAgent(Agent* target, Grid* grid, float radius) : Agent(grid, radius)
{
	// Initialise the first enemy agent as the enemy leader
	if (EnemyAgent::m_leader == nullptr)
	{
		m_leader = this;
	}

	// Spawn this unit within 20 units of the enemy leader
	m_position = Vec3(m_leader->m_position.x + (rand() % 20), m_leader->m_position.y + (rand() % 20), 0);
	m_enemyUnits.push_back(this);

	// Set this AI's target to be the player leader
	m_target = target;

	// Construct decision tree for the enemy agent (refer to the diagram in the TDD for a better view of the decision tree, as it uses the same branch numbering)
	TrueFalseDecision* branch3 = new TrueFalseDecision(new GatherAction(), new SearchAction(), new ResourcesRemain());
	TrueFalseDecision* branch6 = new TrueFalseDecision(new GatherAction(), new PursueAction(), new ResourcesRemain());
	TrueFalseDecision* branch7 = new TrueFalseDecision(new FleeAction(), new PursueAction(), new ResourcesRemain());
	TrueFalseDecision* branch4 = new TrueFalseDecision(new PursueAction(), branch6, new IsStronger());
	TrueFalseDecision* branch5 = new TrueFalseDecision(new PursueAction(), branch7, new IsStronger());
	TrueFalseDecision* branch1 = new TrueFalseDecision(new SearchAction(), branch3, new IsStronger());
	TrueFalseDecision* branch2 = new TrueFalseDecision(branch5, branch4, new InFleeRange());
	m_rootDecision = new TrueFalseDecision(branch2, branch1, new InPursueRange());
}

EnemyAgent::~EnemyAgent()
{

}

// Update will first attempt to collect any resources present in this enemy's current cell, it will then
// update the maximum movement speed of the enemy army based on how many units are present. The function
// then attempts to traverse the AI's decision tree (if update is being called by the leader and if the
// decision timer has run down) to enact the appropriate action and update the enemy's path. The function
// will then either move the agent along it's calculated path, or engage in combat with the player units
// depending on it's static state.
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
		// Clear the path and engage in combat
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

	// Add the forces to the unit
	UpdateMotion(deltaTime);
}


// CohesionBehaviour simply applies a force towards the enemy leader to keep it's units close
bool EnemyAgent::CohesionBehaviour()
{
	Vec3 leaderDisplacement = m_leader->m_position - m_position;
	Vec3 steeringForce = leaderDisplacement - m_velocity;
	AddForce(steeringForce * m_cohesionForce);

	return true;
}

// SeparationBehaviour iterates through all the other enemy units and finds the
// average force away from all of them, finally adding this force to this unit's
// current force multiplied by the separation factor. The force away from any
// one unit is inversely proportional to the distance between this unit and the other.
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

// AddUnit is an Agent override, and simply creates a new enemy agent
void EnemyAgent::AddUnit()
{
	EnemyAgent* newUnit = new EnemyAgent(m_target, m_grid, m_radius);
}

// KillUnit will delete this enemy unit, while also making sure that all references
// to it are appropriately removed and nullified. The function also checks if the
// unit being killed is the last in the army, or is the current leader of the army,
// as these are special cases that are handled differently.
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
		// Transfer the current enemy leader's target to the new leader
		m_enemyUnits.front()->m_target = m_leader->m_target;
		// Repoint the enemy army's leader pointer to the next in charge
		m_leader = m_enemyUnits.front();

		// Replace the player leader's target pointer with the new enemy leader
		m_target->m_target = m_leader;

		// Return before deleting this as it will invalidate the pointer in the main game loop (delete it there instead)
		m_leaderDeleted = true;
		return;
	}

	// Otherwise this unit is not special and can just be deleted
	delete this;
}

// FindClosest will iterate through the enemy army and return a pointer to the enemy unit
// that is closest to the inputted agent, this function is used for player units to determine
// which enemy they should target for attack.
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

// AttackSequence will update this units attack target with the closest player unit to it and then
// attempt to attack it if within range and if its attack is not on cooldown.
void EnemyAgent::AttackSequence(float deltaTime)
{
	// Update this agents attack target with the current closest player unit
	m_attackTarget = m_leader->m_target->FindClosest(this);

	// Seek the attack target, if within attack range Seek returns true, then attempt to attack it if the attack cooldown has run out
	m_attackCooldown -= deltaTime;
	if (SeekBehaviour(m_attackTarget) && m_attackCooldown <= 0)
	{
		// Damages this enemy's attack target and is true if the target is now dead
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

// Update motion will either add the force calculated this frame to this enemy's velocity (for normal units), or
// will simply set this enemy's velocity to the force itself (for leader units), this is so that the leader unit
// always stays squarly on it's path, whereas other enemy units float around less stiffly.
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

// Draw will draw the enemy leader as a red hexagon, and will draw other enemy units as a maroon hexagon
void EnemyAgent::Draw()
{
	if (this == m_leader)
	{
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, RED);
	}
	else
	{
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, MAROON);
	}

	// Draw a black border around the enemy unit
	DrawPolyLines({ m_position.x, m_position.y }, 6, m_radius, 0, BLACK);
}