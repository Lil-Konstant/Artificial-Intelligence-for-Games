#include "Player.h"

// Initialise the static leader pointer as null
Player* Player::m_leader = nullptr;
// Initialise the static unit vector as null
std::vector<Player*> Player::m_playerUnits = std::vector<Player*>();


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
	// Get the cell this player unit is currently in	
	m_currentCell = m_grid->getCell(m_position);

	// If there is a resource in this cell
	if (m_currentCell->m_resource != nullptr)
	{
		//Check for collisions with resource nodes within this cell
		if (m_currentCell->m_resource->TryCollision(this))
		{
			AddUnit();
			delete m_currentCell->m_resource;
			m_currentCell->m_resource = nullptr;
		}
	}

	// Reset the force for this frame to 0
	m_force = Vec3(0, 0, 0);

	// Create a path to the selected cell if right clicking
	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		Vec3 mousePosition = Vec3(GetMouseX(), GetMouseY(), 0);
		Cell* destinationCell = m_grid->getCell(mousePosition);

		m_path = m_grid->aStar(m_currentCell, destinationCell);
		m_path.erase(m_path.begin());
	}

	// If there is a path for the player agent to follow
	if (m_path.size() > 0)
	{
		// If the front cell is the last cell in the path, use arrival behaviour
		if (m_path.size() == 1)
		{
			// If the front cell has now been reached, pop it off the path list
			if (ArrivalBehaviour(m_path.front()))
			{
				m_path.erase(m_path.begin());
			}
		}
		// Otherwise seek towards the front cell in the path list
		else
		{
			// If the front cell has now been reached, pop it off the path list
			if (SeekBehaviour(m_path.front()))
			{
				m_path.erase(m_path.begin());
			}
		}
	}

	// If the unit we're updating isn't the leader, then calculate the cohesion and separation for flocking
	if (this != m_leader)
	{
		SeparationBehaviour();
		CohesionBehaviour();
	}

	// Add Force multiplied by delta time to Velocity, truncate with the max speed to not over speed
	m_velocity = Truncate(m_velocity + (m_force * deltaTime), m_maxSpeed);
	// Add Velocity multiplied by delta time to Position
	m_position = m_position + (m_velocity * deltaTime);
	// Scale the velocity down according to the friction
	m_velocity = m_velocity * m_frictionModifier;
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

// Draw the player as a polygon and the mouse follow target
void Player::Draw()
{
	if (this == m_leader)
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, YELLOW);
	else
		DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, PURPLE);
	
	DrawPolyLines({ m_position.x, m_position.y }, 6, m_radius, 0, BLACK);
}