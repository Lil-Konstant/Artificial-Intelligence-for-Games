#pragma once
#include "Agent.h"
#include "DecisionTree.h"

using namespace DecisionTree;

class EnemyAgent :
    public Agent
{
public:
    EnemyAgent(Agent* target, Grid* grid);
    virtual ~EnemyAgent();

    void Update(float deltaTime) override;
    void Draw() override;

    // Root node of decision tree, checked every update and recurses down linked decision branches
    TrueFalseDecision* m_rootDecision;

    //list<ResourceNodes> resourceList;
};

