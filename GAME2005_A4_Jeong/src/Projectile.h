#pragma once
#include "Sprite.h"
#include "PhysicsEngine.h"

class Projectile : public PhysicsEngine
{
public:
	Projectile(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype);
	~Projectile() override;

	// Life Cycle Functions
	void Draw() override;
	void Update() override;

private:
	void BuildAnimations();
};

