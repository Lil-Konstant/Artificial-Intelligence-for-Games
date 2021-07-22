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

	m_position = Vec3(m_leader->m_position.x + (rand() % 100), m_leader->m_position.y + (rand() % 100), 0);
	m_enemyUnits.push_back(this);

	m_target = target;

	// DEBUGGING
	m_unitCount = -100;

	// Construct decision tree for the enemy agent
	TrueFalseDecision* playerInteractionMode = new TrueFalseDecision(new PursureAction(), new FleeAction(), new IsStronger());
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
			delete m_currentCell->m_resource;
			m_currentCell->m_resource = nullptr;
		}
	}

	// Traverse the decision tree every frame (or every timer)
	m_rootDecision->makeDecision(this);
}

bool EnemyAgent::CohesionBehaviour()
{
	//Vec3 leaderDisplacement = m_leader->m_position - m_position;
	//Vec3 steeringForce = leaderDisplacement - m_velocity;
	//AddForce(steeringForce * m_cohesionForce);

	return true;
}

bool EnemyAgent::SeparationBehaviour()
{
	//Vec3 averageForce = Vec3(0, 0, 0);

	//for (auto unit : m_playerUnits)
	//{
	//	if (unit != this)
	//	{
	//		// Calculate the vector pointing from the unit to this
	//		Vec3 displacementVec = m_position - unit->m_position;
	//		float distance = displacementVec.Magnitude();

	//		// If the vector is non-zero, normalise it and scale it by the repulsion factor
	//		if (distance != 0)
	//		{
	//			Vec3 repulsionForce = displacementVec.GetNormalised() * m_separationForce;
	//			repulsionForce = repulsionForce / (distance);
	//			// Add this neighbours repulsion force to the running force total
	//			averageForce = averageForce + repulsionForce;
	//		}
	//		else
	//			return true;
	//	}
	//}

	//AddForce(averageForce * m_cohesionForce);
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
	DrawPoly({ m_position.x, m_position.y }, 3, m_radius, 0, RED);
}