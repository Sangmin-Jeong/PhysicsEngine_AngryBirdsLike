#include "Arrow.h"
#include "TextureManager.h"
#include "Util.h"

Arrow::Arrow(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype) : PhysicsEngine(mass, angle, speed, gravity, damping, startX, startY, type, mtype)
{
	SetMovementVals(mass, angle, speed, gravity, damping, startX, startY, type);
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/arrow.txt",
		"../Assets/sprites/arrow.png",
		"arrowSprites");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("arrowSprites"));

	// set frame width
	SetWidth(64);

	// set frame height
	SetHeight(32);

	GetTransform()->position = glm::vec2(GetStartingX(), GetStartingY());
	GetRigidBody()->velocity = GetVelocity();
	//GetRigidBody()->acceleration = 
	GetRigidBody()->isColliding = false;

	BuildAnimations();
}

Arrow::~Arrow()
= default;

void Arrow::Draw()
{
	PhysicsEngine::Draw();

	TextureManager::Instance().PlayAnimation("arrowSprites", GetAnimation("arrow"),
		GetTransform()->position, 1.0f, -90, 255, true, SDL_FLIP_NONE);
}

void Arrow::Update()
{
	PhysicsEngine::Update();
}


void Arrow::BuildAnimations()
{
	auto arrow_animation = Animation();

	arrow_animation.name = "arrow";
	arrow_animation.frames.push_back(GetSpriteSheet()->GetFrame("arrow1"));
	arrow_animation.frames.push_back(GetSpriteSheet()->GetFrame("arrow2"));
	arrow_animation.frames.push_back(GetSpriteSheet()->GetFrame("arrow3"));
	arrow_animation.frames.push_back(GetSpriteSheet()->GetFrame("arrow4"));

	SetAnimation(arrow_animation);
}