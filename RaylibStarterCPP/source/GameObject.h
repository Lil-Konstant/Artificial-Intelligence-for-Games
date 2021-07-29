#pragma once
#include "MathsClasses.h"
#include "raylib.h"

using namespace MathsClasses;

/// <summary>
/// GameObject is the abstract base class that all in scene objects inherit from. It contains a means
/// of tracking the position of the objects on the screen using a vector3 (the z component is not used),
/// a radius that is used for collisions and abstract Update, Draw and TryCollision funcitons.
/// </summary>
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