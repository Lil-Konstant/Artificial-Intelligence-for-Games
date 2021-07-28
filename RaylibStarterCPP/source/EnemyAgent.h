#pragma once
#include "Agent.h"
#include "DecisionTree.h"

using namespace DecisionTree;

class EnemyAgent :
    public Agent
{
public:
    EnemyAgent(Agent* target, Grid* grid, float radius);
    virtual ~EnemyAgent();

    void Update(float deltaTime) override;
    void Draw() override;
    Agent* FindClosest(Agent* agent) override;

    void AttackSequence(float deltaTime) override;

    bool CohesionBehaviour() override;
    bool SeparationBehaviour() override;

    void AddUnit() override;
    void KillUnit() override;

    // For unit traversal
    static EnemyAgent* m_leader;
    static std::vector<EnemyAgent*> m_enemyUnits;
    static bool m_armyDefeated;
    
    // Initialise this agents state as moving
    static State m_state;

    bool m_leaderDeleted = false;

    // Root node of decision tree, checked every update and recurses down linked decision branches
    TrueFalseDecision* m_rootDecision;
    float decisionTimer = 0.5f;

    void UpdateMotion(float deltaTime) override;

    bool TryCollision(GameObject* other) override { return true; }

    int GetUnitCount() override { return m_enemyUnits.size(); }
};

