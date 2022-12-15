#include "Block.h"

#include <iostream>

#include "Util.h"

Block::Block(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype) : PhysicsEngine(mass, angle, speed, gravity, damping, startX, startY, type, mtype)
{
	SetMovementVals(mass, angle, speed, gravity, damping, startX, startY, type);

	// set frame width
	SetWidth(150);

	// set frame height
	SetHeight(100);

	GetTransform()->position = glm::vec2(GetStartingX(), GetStartingY());
	GetRigidBody()->velocity = GetVelocity();
	//GetRigidBody()->acceleration = 
	GetRigidBody()->isColliding = false;
}

Block::~Block()
= default;

void Block::Draw()
{
	PhysicsEngine::Draw();
	/*Util::DrawFilledRect(GetTransform()->position, GetWidth(), GetHeight(), glm::vec4(0,0,1,0.5));*/
	Util::DrawRect(GetTransform()->position, GetWidth(), GetHeight(), glm::vec4(0, 0, 1, 0.5));
	//std::cout << position.x << std::endl;
}

void Block::Update()
{
	PhysicsEngine::Update();
}
