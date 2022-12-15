#include "Projectile.h"
#include "TextureManager.h"
#include "Util.h"

Projectile::Projectile(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype) : PhysicsEngine(mass, angle, speed, gravity, damping, startX, startY, type, mtype)
{
	SetMovementVals(mass, angle, speed, gravity, damping, startX, startY, type);
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/projectile.txt",
		"../Assets/sprites/projectile.png",
		"projectileSprites");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("projectileSprites"));

	// set frame width
	SetWidth(65);

	// set frame height
	SetHeight(65);

	GetTransform()->position = glm::vec2(GetStartingX(), GetStartingY());
	GetRigidBody()->velocity = GetVelocity();
	//GetRigidBody()->acceleration = 
	GetRigidBody()->isColliding = false;

	BuildAnimations();
}

Projectile::~Projectile()
= default;

void Projectile::Draw()
{
	PhysicsEngine::Draw();

	TextureManager::Instance().PlayAnimation("projectileSprites", GetAnimation("projectile"),
		GetTransform()->position, 1.0f, 0, 255, true, SDL_FLIP_HORIZONTAL);
}

void Projectile::Update()
{
	PhysicsEngine::Update();
}


void Projectile::BuildAnimations()
{
	auto projectile_animation = Animation();

	projectile_animation.name = "projectile";
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile1"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile2"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile3"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile4"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile5"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile6"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile7"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile8"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile9"));
	projectile_animation.frames.push_back(GetSpriteSheet()->GetFrame("projectile10"));

	SetAnimation(projectile_animation);
}