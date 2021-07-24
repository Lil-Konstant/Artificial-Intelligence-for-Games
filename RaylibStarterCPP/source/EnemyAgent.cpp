#include "EnemyAgent.h"

// Initialise the static leader pointer as null
EnemyAgent* EnemyAgent::m_leader = nullptr;
// Initialise the static unit vector as null
std::vector<EnemyAgent*> EnemyAgent::m_enemyUnits = std::vector<EnemyAgent*>();

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

	// DEBUGGING
	m_unitCount = 100;

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
	// Get the cell this player unit is currently in	
	m_currentCell = m_grid->getCell(m_position);

	// If there is a resource in this cell
	if (m_currentCell->m_resource != nullptr)
	{
		//Check for collisions with resource nodes within this cell
		if (m_currentCell->m_resource->TryCollision(this))
		{
			AddUnit();
			// Remove this resource from the agents shared resource list
			m_resourceList.erase(std::find(m_resourceList.begin(), m_resourceList.end(), m_currentCell->m_resource));
			delete m_currentCell->m_resource;
			m_currentCell->m_resource = nullptr;
		}
	}

	// Traverse the decision tree every frame (or every timer)
	//m_rootDecision->makeDecision(this);
	decisionTimer -= deltaTime;
	if (decisionTimer <= 0 && this == m_leader)
	{
		m_rootDecision->makeDecision(this);
		decisionTimer = 0.5;
	}
	
	// Reset the force for this frame to 0
	m_force = Vec3(0, 0, 0);

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
			// Some vector maths to stop agents from moving backwards if they are closer to their second path node
			// Draw a vector from the first node to the second node
			Vec3 nodeDisplacement = (m_path[1]->m_position - m_path[0]->m_position).GetNormalised();
			Vec3 agentDisplacement = (m_position - m_path[0]->m_position).GetNormalised();
			float angle = acos(nodeDisplacement.Dot(agentDisplacement));

			if (angle < PI / 4)
			{
				m_path.erase(m_path.begin());
			}

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

void EnemyAgent::Draw()
{
	DrawPoly({ m_position.x, m_position.y }, 6, m_radius, 0, RED);
	DrawPolyLines({ m_position.x, m_position.y }, 6, m_radius, 0, BLACK);
}