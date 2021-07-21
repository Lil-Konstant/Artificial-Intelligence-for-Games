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

    // Root node of decision tree, checked every update and recurses down linked decision branches
    TrueFalseDecision* m_rootDecision;

    //list<ResourceNodes> resourceList;
};

