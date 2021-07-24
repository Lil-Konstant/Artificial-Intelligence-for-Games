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
    bool TryCollision(GameObject* other) override { return true; }

    bool CohesionBehaviour() override;
    bool SeparationBehaviour() override;

    void AddUnit() override;

    // For unit traversal
    static EnemyAgent* m_leader;
    static std::vector<EnemyAgent*> m_enemyUnits;

    // Root node of decision tree, checked every update and recurses down linked decision branches
    TrueFalseDecision* m_rootDecision;
    float decisionTimer = 1;
};

