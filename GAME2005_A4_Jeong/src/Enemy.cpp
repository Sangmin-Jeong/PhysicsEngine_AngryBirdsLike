#include "Enemy.h"
#include "TextureManager.h"

Enemy::Enemy(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype) : PhysicsEngine(mass, angle, speed, gravity, damping, startX, startY, type, mtype)
{
	SetMovementVals(mass, angle, speed, gravity, damping, startX, startY, type);
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/enemy/enemy.txt",
		"../Assets/sprites/enemy/enemy.png", 
		"enemyspritesheet");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("enemyspritesheet"));

	// set frame width
	SetWidth(65);

	// set frame height
	SetHeight(65);

	GetTransform()->position = glm::vec2(GetStartingX(), GetStartingY());
	GetRigidBody()->velocity = GetVelocity();
	//GetRigidBody()->acceleration = glm::vec2(0, GetGravity());
	GetRigidBody()->isColliding = false;

	SetToughness(glm::vec2(300.0f, 300.0f));

	BuildAnimations();
}

Enemy::~Enemy()
= default;

void Enemy::Draw()
{
	PhysicsEngine::Draw();
	// draw the plane sprite with simple propeller animation
	TextureManager::Instance().PlayAnimation("enemyspritesheet", GetAnimation("enemy"),
		GetTransform()->position, 0.5f, 0, 255, true);
}

void Enemy::Update()
{
	PhysicsEngine::Update();
}

void Enemy::BuildAnimations()
{
	auto enemy_animation = Animation();

	enemy_animation.name = "enemy";
	enemy_animation.frames.push_back(GetSpriteSheet()->GetFrame("enemy1"));
	enemy_animation.frames.push_back(GetSpriteSheet()->GetFrame("enemy2"));
	enemy_animation.frames.push_back(GetSpriteSheet()->GetFrame("enemy3"));
	enemy_animation.frames.push_back(GetSpriteSheet()->GetFrame("enemy4"));

	SetAnimation(enemy_animation);
}