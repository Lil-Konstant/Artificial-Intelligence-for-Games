#pragma once
#include "GameObject.h"
#include <vector>

/// <summary>
/// Waypoint is a simple node-based class that is used for the enemy agent to navigate the map when in search mode and when fleeing.
/// The map contains 9 Waypoints (diagramed in the TDD) that are equally spaced around, with each waypoint
/// leading to it's adjacent waypoints (mimicking a node/edge structure but on a simpler level). In debug mode, waypoint nodes are
/// drawn as blue diamonds
/// </summary>
class Waypoint : public GameObject
{
public:
	Waypoint(float radius) : GameObject(radius) { }
	~Waypoint() { }

	void Update(float deltaTime) override { };
	bool TryCollision(GameObject* other) { return true; }
	void Draw();

	// List of waypoints this waypoint connects to
	std::vector<Waypoint*> adjacentWaypoints;
};