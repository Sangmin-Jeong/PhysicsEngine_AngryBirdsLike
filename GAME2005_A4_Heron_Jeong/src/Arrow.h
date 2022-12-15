#pragma once
#include "Sprite.h"
#include "PhysicsEngine.h"

class Arrow : public PhysicsEngine
{
public:
	Arrow(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype);
	~Arrow() override;

	// Life Cycle Functions
	void Draw() override;
	void Update() override;

private:
	void BuildAnimations();
};

