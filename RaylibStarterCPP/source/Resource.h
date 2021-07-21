#pragma once
#include "GameObject.h"

class Resource : public GameObject
{
public:
	Resource(float radius);
	~Resource();

	void Update(float deltaTime) override;
	void Draw() override;
	bool TryCollision(GameObject* other);
};