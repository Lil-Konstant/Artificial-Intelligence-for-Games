#include "Player.h"

// Construct the player with mouse follow behaviour, and set the target to the players starting position
Player::Player(Grid* grid) : Agent(grid)
{
	m_position = Vec3(GetScreenWidth() / 2, GetScreenHeight() / 2, 0);
}

Player::~Player()
{

}

void Player::Update(float deltaTime)
{
	// Reset the force for this frame to 0
	m_force = Vec3(0, 0, 0);

	// Create a path to the selected cell if right clicking
	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		Vec3 mousePosition = Vec3(GetMouseX(), GetMouseY(), 0);
		Cell* playerCell = m_grid->getCell(m_position);
		Cell* destinationCell = m_grid->getCell(mousePosition);

		m_path = m_grid->aStar(playerCell, destinationCell);
	}

	// If there is a path for the player agent to follow
	if (m_path.size() > 0)
	{
		// If the front cell is the last cell in the path, use arrival behaviour
		if (m_path.size() == 1)
		{
			// If the front cell has now been reached, pop it off the path list
			if (Arrive(m_path.front()))
			{
				m_path.erase(m_path.begin());
			}
		}
		// Otherwise seek towards the front cell in the path list
		else
		{
			// If the front cell has now been reached, pop it off the path list
			if (Seek(m_path.front()))
			{
				m_path.erase(m_path.begin());
			}
		}
	}

	// Add Force multiplied by delta time to Velocity, truncate with the max speed to not over speed
	m_velocity = Truncate(m_velocity + (m_force * deltaTime), m_maxSpeed);
	// Add Velocity multiplied by delta time to Position
	m_position = m_position + (m_velocity * deltaTime);
	// Scale the velocity down according to the friction
	m_velocity = m_velocity * m_frictionModifier;
}

bool Player::Seek(Cell* target)
{
	// Calculate a vector pointing from this agent to the target agent
	Vec3 desiredVelocity = target->m_position - m_position;
	float distanceToTarget = desiredVelocity.Magnitude();

	// Distance is 0 so return true as we have reached the target cell
	if (distanceToTarget < 10)
	{
		return true;
	}
	// As long as the distance is not 0, then normalise the vector and scale it by its move speed
	else
	{
		desiredVelocity = desiredVelocity.GetNormalised() * m_maxSpeed;
		// Add the steering force to this agents current force and return false as we are yet to reach the front cell in the path
		m_force = m_force + (desiredVelocity - m_velocity);
		return false;
	}
}

bool Player::Arrive(Cell* target)
{
	// Seek towards the current target
	Vec3 desiredVelocity = target->m_position - m_position;
	float distanceToTarget = desiredVelocity.Magnitude();

	// Distance is 0 so return true as we have reached the target cell
	if (distanceToTarget < 1)
	{
		return true;
	}
	// If the target is within the arrival radius, scale the velocity down by the distance
	else if (distanceToTarget < m_arrivalRadius)
	{
		desiredVelocity = desiredVelocity.GetNormalised() * m_maxSpeed * ((distanceToTarget * 3) / m_arrivalRadius);
	}
	// Other wise the velocity is the normal seek velocity
	else
	{
		desiredVelocity = desiredVelocity.GetNormalised() * m_maxSpeed;
	}

	// Add the steering force to this agents current force and return false as we are yet to reach the front cell in the path
	m_force = m_force + (desiredVelocity - m_velocity);
	return false;
}

// Draw the player as a polygon and the mouse follow target
void Player::Draw()
{
	DrawPoly({ m_position.x, m_position.y }, 6, 10, 0, PURPLE);
}