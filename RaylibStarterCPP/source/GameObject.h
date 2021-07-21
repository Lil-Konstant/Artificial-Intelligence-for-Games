#pragma once
#include "MathsClasses.h"
#include "raylib.h"

using namespace MathsClasses;

class GameObject
{
public:
	GameObject(float radius) : m_radius(radius) {};
	~GameObject() {};

	Vec3 m_position = Vec3(0, 0, 0);
	float m_radius;

protected:
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	virtual bool TryCollision(GameObject* other) = 0;
};