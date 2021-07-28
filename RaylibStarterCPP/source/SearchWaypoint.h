#pragma once
#include "GameObject.h"
#include <vector>

// SearchWaypoint is a simple node-based class that is used for the enemy agent to navigate the map when in search mode.
// The map contains 9 Search Waypoints (diagramed in the TDD) that are equally spaced around, with each waypoint
// leading to it's adjacent waypoints (mimmicing a node/edge structure but on a much simpler level). The enemy agent 
// will find which waypoint is vaguely closest to the player currently and navigate to it from the waypoint it is currently
// at.
class SearchWaypoint : public GameObject
{
public:
	SearchWaypoint(float radius) : GameObject(radius) { }
	~SearchWaypoint() { }

	void Update(float deltaTime) override { };
	bool TryCollision(GameObject* other) { return true; }
	void Draw();

	std::vector<SearchWaypoint*> adjacentWaypoints;
};