#include "Player.h"

// Initialise the static leader pointer as null
Player* Player::m_leader = nullptr;
// Initialise the static state as moving
Agent::State Player::m_state = Agent::State::STATE_MOVE;
// Initialise the static unit vector as null
std::vector<Player*> Player::m_playerUnits = std::vector<Player*>();
// Initialise the static armyDefeated bool as false
bool Player::m_armyDefeated = false;

// Construct the player with mouse follow behaviour, and set the target to the players starting position
Player::Player(Grid* grid, float radius) : Agent(grid, radius)
{
	// If this is the first player agent being created, make it the leader and add control behaviour
	if (Player::m_leader == nullptr)
	{
		m_leader = this;
	}

	m_position = Vec3(m_leader->m_position.x + (rand() % 100), m_leader->m_position.y + (rand() % 100), 0);
	m_playerUnits.push_back(this);
}

Player::~Player()
{

}

void Player::Update(float deltaTime)
{
	// Attempt to collect a resource if one is found in this player agents cell
	AttemptCollectResource();

	// Reset the force for this frame to 0
	m_force = Vec3(0, 0, 0);

	// Either updates and moves along the player path, or attempts to engage in attack with enemies
	switch (m_state)
	{
	case State::STATE_MOVE:
		// Create a path to the selected cell if right clicking
		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
		{
			Vec3 mousePosition = Vec3(GetMouseX(), GetMouseY(), 0);
			Cell* destinationCell = m_grid->getCell(mousePosition);

			m_path = m_grid->aStar(m_currentCell, destinationCell);
		}
		FollowPath();
		break;
	case State::STATE_ATTACK:
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

bool Player::CohesionBehaviour()
{
	Vec3 leaderDisplacement = m_leader->m_position - m_position;
	Vec3 steeringForce = leaderDisplacement - m_velocity;
	AddForce(steeringForce * m_cohesionForce);

	return true;
}

bool Player::SeparationBehaviour()
{
	Vec3 averageForce = Vec3(0, 0, 0);

	for (auto unit : m_playerUnits)
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

// Adds a new player unit into the playerUnits vector
void Player::AddUnit()
{
	Player* newUnit = new Player(m_grid, m_radius);
	// Spawn the new unit at a random position near the leader
	newUnit->m_position = m_leader->m_position + Vec3(rand() % 50, rand() % 50, 0) - Vec3(rand() % 50, rand() % 50, 0);
}

// "Kills" this player unit and removes all references of it
void Player::KillUnit()
{
	std::cout << "PLAYER DEAD LOL" << std::endl;

	// Remove this unit from the playerUnits list
	m_playerUnits.erase(std::find(m_playerUnits.begin(), m_playerUnits.end(), this));

	// If killing the last enemy in the enemy army, exit early for a player win state
	if (m_playerUnits.size() == 0)
	{
		Player::m_armyDefeated = true;
		return;
	}

	// If killing the leader unit, replace it with the next in charge
	if (this == m_leader)
	{
		// Repoint the enemy armies leader pointer to the next in charge
		m_leader = m_playerUnits.front();

		// Replace the player leaders target pointer with the new enemy leader
		m_target->m_target = m_leader;

		// Return before deleting this as it will invalidate the pointer in the main game loop (delete it there instead)
		m_leaderDeleted = true;
		return;
	}

	// Otherwise this unit is not special and can just be deleted
	delete this;
}

// Returns the closest unit in this player's army to the inputted agent
Agent* Player::FindClosest(Agent* agent)
{
	Agent* closestPlayer = m_playerUnits.front();
	float closestDistanceSqr = closestPlayer->m_position.DistanceSqr(agent->m_position);

	for (auto unit : m_playerUnits)
	{
		// If this unit is closer to the input agent than the current closest
		if (unit->m_position.DistanceSqr(agent->m_position) < closestDistanceSqr)
		{
			closestPlayer = unit;
			closestDistanceSqr = unit->m_position.DistanceSqr(agent->m_position);
		}
	}

	return closestPlayer;
}

// Run attack procedures
void Player::AttackSequence(float deltaTime)
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

// Draw the player as a polygon and the mouse follow target
void Player::Draw()
{
	if (this == m_leader)
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, YELLOW);
	else
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, PURPLE);
	
	DrawPolyLines({ m_position.x, m_position.y }, 6, m_radius, 0, BLACK);
}