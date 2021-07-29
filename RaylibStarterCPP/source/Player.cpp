#include "Player.h"

// Initialise the static leader pointer as null
Player* Player::m_leader = nullptr;
// Initialise the static state as moving
Agent::State Player::m_state = Agent::State::STATE_MOVE;
// Initialise the static unit vector as null
std::vector<Player*> Player::m_playerUnits = std::vector<Player*>();
// Initialise the static armyDefeated bool as false
bool Player::m_armyDefeated = false;

// The Player constructor will initialise the static leader pointer if this is the first player unit being created,
// sets this player unit's position to be a random position within 20 units of the leader, and finally adds this unit to the 
// playerUnits vector.
Player::Player(Grid* grid, float radius) : Agent(grid, radius)
{
	// If this is the first player agent being created, make it the leader and add control behaviour
	if (Player::m_leader == nullptr)
	{
		m_leader = this;
	}

	m_position = Vec3(m_leader->m_position.x + (rand() % 20), m_leader->m_position.y + (rand() % 20), 0);
	m_playerUnits.push_back(this);
}

Player::~Player()
{

}

// Update will first attempt to collect any resources present in this player unit's current cell, it will then
// update the maximum movement speed of the player army based on how many units are present. The function
// will then either check for player input to create a new path and move along it, or engage in combat with the 
// enemy units depending on it's static state.
void Player::Update(float deltaTime)
{
	// Attempt to collect a resource if one is found in this player agents cell
	AttemptCollectResource();

	// Update the armies movespeed (in case units have been added to slow the army down)
	UpdateMoveSpeed();

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

			if (destinationCell->m_traversable)
				m_path = m_grid->aStar(m_currentCell, destinationCell);
		}
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

// CohesionBehaviour simply applies a force towards the player leader to keep it's units close
bool Player::CohesionBehaviour()
{
	Vec3 leaderDisplacement = m_leader->m_position - m_position;
	Vec3 steeringForce = leaderDisplacement - m_velocity;
	AddForce(steeringForce * m_cohesionForce);

	return true;
}

// SeparationBehaviour iterates through all the other player units and finds the
// average force away from all of them, finally adding this force to this unit's
// current force multiplied by the separation factor. The force away from any
// one unit is inversely proportional to the distance between this unit and the other.
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

// AddUnit is an Agent override, and simply creates a new player agent
void Player::AddUnit()
{
	Player* newUnit = new Player(m_grid, m_radius);
}

// KillUnit will delete this player unit, while also making sure that all references
// to it are appropriately removed and nullified. The function also checks if the
// unit being killed is the last in the army, or is the current leader of the army,
// as these are special cases that are handled differently.
void Player::KillUnit()
{
	// Remove this unit from the playerUnits list
	m_playerUnits.erase(std::find(m_playerUnits.begin(), m_playerUnits.end(), this));

	// If killing the last player in the player army, exit early for an enemy win state
	if (m_playerUnits.size() == 0)
	{
		Player::m_armyDefeated = true;
		return;
	}

	// If killing the leader unit, replace it with the next in charge
	if (this == m_leader)
	{
		// Transfer the current player leader's target to the new leader
		m_playerUnits.front()->m_target = m_leader->m_target;
		// Repoint the player army's leader pointer to the new leader
		m_leader = m_playerUnits.front();

		// Replace the enemy leaders target pointer with the new player leader
		m_target->m_target = m_leader;

		// Return before deleting this as it will invalidate the pointer in the main game loop (delete it there instead)
		m_leaderDeleted = true;
		return;
	}

	// Otherwise this unit is not special and can just be deleted
	delete this;
}

// FindClosest will iterate through the player army and return a pointer to the player unit
// that is closest to the inputted agent, this function is used for enemy units to determine
// which player unit they should target for attack.
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

// AttackSequence will update this units attack target with the closest player unit to it and then
// attempt to attack it if within range and if its attack is not on cooldown.
void Player::AttackSequence(float deltaTime)
{
	// Update this agents attack target with the current closest enemy unit
	m_attackTarget = m_leader->m_target->FindClosest(this);

	// Seek the attack target, if within attack range Seek returns true, then attempt to attack it if the attack cooldown has run out
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

// Update motion will either add the force calculated this frame to this player's velocity (for normal units), or
// will simply set this player's velocity to the force itself (for leader units), this is so that the leader unit
// always stays squarly on it's path, whereas other enemy units float around less stiffly.
void Player::UpdateMotion(float deltaTime)
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

// Draw will draw the player leader as a blue circle, and will draw other player units as a dark blue circle.
// Draw will also draw out the player leader's currrent path as a yellow line.
void Player::Draw()
{
	if (this == m_leader)
	{
		// First draw a line from the player leader to the first cell in its path
		if (m_path.size() > 0)
		{
			DrawLine(m_position.x, m_position.y, m_path.front()->m_position.x, m_path.front()->m_position.y, YELLOW);
		}
		// Then for each cell in the path, call draw on it and pass the next cell in the path as the target parameter
		for (int i = 0; i < m_path.size(); i++)
		{
			if (i + 1 < (int)m_path.size())
				m_path[i]->Draw(false, m_path[i + 1]);
			else
				m_path[i]->Draw(false, m_path[i]);
		}

		// Draw the player leader in blue
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, BLUE);
	}
	else
	{
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, DARKBLUE);
	}
	
	// Draw a black border around the player unit
	DrawPolyLines({ m_position.x, m_position.y }, 6, m_radius, 0, BLACK);
}