#pragma once
#ifndef __ENEMY__
#define __ENEMY__

#include "PhysicsEngine.h"
#include "Util.h"

class Enemy : public PhysicsEngine
{
public:
	Enemy(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype);
	~Enemy() override;

	// Life Cycle Functions
	virtual void Draw() override;
	virtual void Update() override;

private:
	void BuildAnimations();
};

#endif /* defined (__ENEMY__) */
