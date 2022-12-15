#pragma once
#ifndef __BLOCK__
#define __BLOCK__

#include "Sprite.h"
#include "PhysicsEngine.h"

class Block : public PhysicsEngine
{
public:
	Block(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype);
	~Block() override;

	void Draw() override;
	void Update() override;


private:
};

#endif /* defined (__BLOCK__) */
