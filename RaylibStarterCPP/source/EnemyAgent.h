#pragma once
#include "Agent.h"
#include "DecisionTree.h"

using namespace DecisionTree;

class EnemyAgent :
    public Agent
{
public:
    EnemyAgent(Agent* target);
    virtual ~EnemyAgent();

    virtual void Update(float deltaTime);
    void Draw() override;

    // Root node of decision tree, checked every update and recurses down linked decision branches
    TrueFalseDecision* m_rootDecision;

    //list<ResourceNodes> resourceList;
};

