#include "Player.h"

// Construct the player with mouse follow behaviour, and set the target to the players starting position
Player::Player(Grid* grid) : Agent(grid)
{
	m_position = Vec3(GetScreenWidth() / 2, GetScreenHeight() / 2, 0);

	mouseFollowBehaviour = new MouseFollowBehaviour();
	mouseFollowBehaviour->m_target = m_position;
	AddBehaviour(mouseFollowBehaviour);
}

Player::~Player()
{

}

// Draw the player as a polygon and the mouse follow target
void Player::Draw()
{
	DrawPoly({ m_position.x, m_position.y }, 6, 10, 0, PURPLE);
	mouseFollowBehaviour->Draw();
}