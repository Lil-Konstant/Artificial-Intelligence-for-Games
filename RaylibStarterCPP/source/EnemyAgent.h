#pragma once
#include "Agent.h"
#include "DecisionTree.h"

using namespace DecisionTree;

/// <summary>
/// EnemyAgent is derived from the agent class, and contains static variables relating to the enemy army, as
/// well as agent overrides for various agent behaviours. Most notably, the enemy agent class contains a decision
/// tree configured on construction that it uses every decision tick to choose which state action to enact.
/// </summary>
class EnemyAgent :
    public Agent
{
public:
    EnemyAgent(Agent* target, Grid* grid, float radius);
    virtual ~EnemyAgent();

    void Update(float deltaTime) override;
    void Draw() override;
    bool TryCollision(GameObject* other) override { return true; }

    void UpdateMotion(float deltaTime) override;
    void AttackSequence(float deltaTime) override;
    Agent* FindClosest(Agent* agent) override;
    int GetUnitCount() override { return m_enemyUnits.size(); }
    
    // Behaviours for keeping army units apart
    bool CohesionBehaviour() override;
    bool SeparationBehaviour() override;

    // Increasing/decreasing army size
    void AddUnit() override;
    void KillUnit() override;

    // Army variables that are static and therefore shared between every enemy
    static EnemyAgent* m_leader;
    static std::vector<EnemyAgent*> m_enemyUnits;
    static bool m_armyDefeated;
    static State m_state;

    bool m_leaderDeleted = false;

    // Root node of decision tree, checked every decision tick and recurses down linked decision branches
    TrueFalseDecision* m_rootDecision;
    float decisionTimer = 0.5f;
};

