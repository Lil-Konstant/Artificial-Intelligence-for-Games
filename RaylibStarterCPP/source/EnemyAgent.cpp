#include "EnemyAgent.h"

EnemyAgent::EnemyAgent(Agent* target, Grid* grid) : Agent(grid)
{
	m_position = Vec3(GetScreenWidth() / 3, GetScreenHeight() / 3, 0);
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

}

void EnemyAgent::Draw()
{
	DrawPoly({ m_position.x, m_position.y }, 3, 10, 0, RED);
}