#include "EnemyAgent.h"

EnemyAgent::EnemyAgent(Agent* target)
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

void EnemyAgent::Draw()
{
	DrawPoly({ m_position.x, m_position.y }, 3, 10, 0, RED);
}