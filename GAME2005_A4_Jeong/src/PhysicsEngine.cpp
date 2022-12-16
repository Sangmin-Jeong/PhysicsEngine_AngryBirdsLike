#include "PhysicsEngine.h"
#include "Util.h"
#include "CollisionManager.h"
#include "Game.h"
#include "PlayScene.h"

PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::PhysicsEngine(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype) : defaultSpeed(10.0f),
	m_width(0), m_height(0), m_enabled(true), m_visible(true)
{
	SetMovementVals(mass, angle, speed, gravity, damping, startX, startY, type);
	SetSize(GetSize()+1);
	if (GetType() == GameObjectType::PROJECTILE)
	{
		m_pProjectiles.push_back(this);
	}
	else if (GetType() == GameObjectType::ENEMY)
	{
		m_pEnemies.push_back(this);
	}
	else if (GetType() == GameObjectType::ARROW)
	{
		m_pArrows.push_back(this);
	}
	else if (GetType() == GameObjectType::BLOCK)
	{
		m_pBlocks.push_back(this);
	}
	else if (GetType() == GameObjectType::BLOCK_FIX)
	{
		m_pFixedBlocks.push_back(this);
	}

	switch (mtype)
	{
	case MaterialType::WOOD:
		material = "WOOD";
		materialCOR = 0.6f;
		break;
	case MaterialType::STEEL:
		material = "STEEL";
		materialCOR = 0.5f;
		break;
	case MaterialType::RUBBER:
		material = "RUBBER";
		materialCOR = 0.8f;
		break;
	case MaterialType::TUNGSTEN:
		material = "TUNGSTEN";
		materialCOR = 0.3f;
		break;
	}
}


PhysicsEngine::~PhysicsEngine()
= default;

void PhysicsEngine::Draw()
{
	float scale = 3.0f;
	// Circle
	if (GetType() == GameObjectType::PROJECTILE || GetType() == GameObjectType::ENEMY)
	{
		Util::DrawCircle(GetTransform()->position, GetWidth() * 0.5f, Default);
	}
	else if (GetType() == GameObjectType::ARROW)
	{
		Util::DrawRect(glm::vec2(GetTransform()->position.x - GetWidth() / 2, GetTransform()->position.y - GetHeight() / 2), GetWidth(), GetHeight());
	}

	if (GetType() == GameObjectType::BLOCK || GetType() == GameObjectType::BLOCK_FIX)
	{
		glm::vec2 center = glm::vec2(GetTransform()->position.x + GetWidth() * 0.5f, GetTransform()->position.y + GetHeight() * 0.5f);
		//  Line Velocity
		Util::DrawLine(center, glm::vec2(center.x + velocity.x * scale,
			center.y + velocity.y * scale), RED);
		//  Line Gravity
		Util::DrawLine(center, glm::vec2(center.x,
			center.y + accelerationGravity.y * scale), PURPLE);
		//  Line Normal Force
		Util::DrawLine(center, glm::vec2(center.x + normalForce.x * scale,
			center.y + normalForce.y * scale), GREEN);
		//  Line Applied Force
		Util::DrawLine(center, glm::vec2(center.x + appliedForce.x * scale,
			center.y + appliedForce.y * scale), BLUE);
		//  Line Friction
		Util::DrawLine(center, glm::vec2(center.x + friction.x * scale,
			center.y + friction.y * scale), ORANGE);
	}
	else
	{
		//  Line Velocity
		Util::DrawLine(GetTransform()->position, glm::vec2(GetTransform()->position.x + velocity.x * scale,
			GetTransform()->position.y + velocity.y * scale), RED);
		//  Line Gravity
		Util::DrawLine(GetTransform()->position, glm::vec2(GetTransform()->position.x,
			GetTransform()->position.y + accelerationGravity.y * scale), PURPLE);
		//  Line Normal Force
		Util::DrawLine(GetTransform()->position, glm::vec2(GetTransform()->position.x + normalForce.x * scale,
			GetTransform()->position.y + normalForce.y * scale), GREEN);
		//  Line Applied Force
		Util::DrawLine(GetTransform()->position, glm::vec2(GetTransform()->position.x + appliedForce.x * scale,
			GetTransform()->position.y + appliedForce.y * scale), BLUE);
		//  Line Friction
		Util::DrawLine(GetTransform()->position, glm::vec2(GetTransform()->position.x + friction.x * scale,
			GetTransform()->position.y + friction.y * scale), ORANGE);
	}
}

void PhysicsEngine::Update()
{
	ProjectileMovement();

	// Collision check between projectiles and Targets
	for (unsigned i = 0; i < m_pProjectiles.size(); i++)
	{
		for (unsigned j = 0; j < m_pEnemies.size(); j++)
		{
			if (CollisionManager::CircleCircleCheck(m_pProjectiles[i], m_pEnemies[j]))
			{
				if(m_pEnemies[j]->GetOverTough() == true)
				{
					//Score
					delete m_pEnemies[j];
					m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), m_pEnemies[j]), m_pEnemies.end());
					PlayScene::SetScore(PlayScene::GetScore() + 1);
				}
				break;
			}
		}

	}

	// Collision check between Projectiles and Blocks
	for (unsigned i = 0; i < m_pProjectiles.size(); i++)
	{
		for (unsigned j = 0; j < m_pBlocks.size(); j++)
		{
			if (CollisionManager::CircleAABBCheck(m_pBlocks[j], m_pProjectiles[i]))
			{
				if (m_pBlocks[j]->GetOverTough() == true)
				{
					delete m_pBlocks[j];
					m_pBlocks.erase(std::remove(m_pBlocks.begin(), m_pBlocks.end(), m_pBlocks[j]), m_pBlocks.end());
				}
				break;
			}
		}
	}

	// Collision check between Projectiles and Fixed_Blocks
	for (unsigned i = 0; i < m_pProjectiles.size(); i++)
	{
		for (unsigned j = 0; j < m_pFixedBlocks.size(); j++)
		{
			if (CollisionManager::CircleAABBCheck(m_pFixedBlocks[j], m_pProjectiles[i]))
			{
				break;
			}
		}
	}

	// Collision detection between Enemies
	if (m_pEnemies.size() >= 2)
	{
		for (unsigned i = 0; i < m_pEnemies.size(); i++)
		{
			for (unsigned j = i + 1; j < m_pEnemies.size(); j++)
			{
				const int SIZE = m_pEnemies.size();
				if (CollisionManager::CircleCircleCheck(m_pEnemies[i], m_pEnemies[j]))
				{
					if (m_pEnemies[i]->GetOverTough() == true)
					{
						delete m_pEnemies[i];
						m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), m_pEnemies[i]), m_pEnemies.end());
						PlayScene::SetScore(PlayScene::GetScore() + 1);

					}
					if(SIZE > m_pEnemies.size())
					{
						--j;
						if (m_pEnemies[j]->GetOverTough() == true)
						{
							delete m_pEnemies[j];
							m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), m_pEnemies[j]), m_pEnemies.end());
							PlayScene::SetScore(PlayScene::GetScore() + 1);
						}
					}
					else
					{
						if (m_pEnemies[j]->GetOverTough() == true)
						{
							delete m_pEnemies[j];
							m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), m_pEnemies[j]), m_pEnemies.end());
							PlayScene::SetScore(PlayScene::GetScore() + 1);
						}
					}

					break;
				}
			}
		}
	}

	// Collision check between Enemies and Blocks
	for (unsigned i = 0; i < m_pEnemies.size(); i++)
	{
		for (unsigned j = 0; j < m_pBlocks.size(); j++)
		{
			if (CollisionManager::CircleAABBCheck(m_pBlocks[j], m_pEnemies[i]))
			{
				if (m_pEnemies[i]->GetOverTough() == true)
				{
					delete m_pEnemies[i];
					m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), m_pEnemies[i]), m_pEnemies.end());
					PlayScene::SetScore(PlayScene::GetScore() + 1);

				}

				if (m_pBlocks[j]->GetOverTough() == true)
				{
					delete m_pBlocks[j];
					m_pBlocks.erase(std::remove(m_pBlocks.begin(), m_pBlocks.end(), m_pBlocks[j]), m_pBlocks.end());
				}
				break;
			}
		}
	}

	// Collision check between Enemies and Fixed_Blocks
	for (unsigned i = 0; i < m_pEnemies.size(); i++)
	{
		for (unsigned j = 0; j < m_pFixedBlocks.size(); j++)
		{
			if (CollisionManager::CircleAABBCheck(m_pFixedBlocks[j], m_pEnemies[i]))
			{
				if (m_pEnemies[i]->GetOverTough() == true)
				{
					delete m_pEnemies[i];
					m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), m_pEnemies[i]), m_pEnemies.end());
					PlayScene::SetScore(PlayScene::GetScore() + 1);

				}
				break;
			}
		}
	}

	// Collision check between Arrows and Targets
	for (unsigned i = 0; i < m_pArrows.size(); i++)
	{
		for (unsigned j = 0; j < m_pEnemies.size(); j++)
		{
			if (CollisionManager::CircleAABBCheck(m_pArrows[i], m_pEnemies[j]))
			{
				if (m_pEnemies[j]->GetOverTough() == true)
				{
					delete m_pEnemies[j];
					m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), m_pEnemies[j]), m_pEnemies.end());
					PlayScene::SetScore(PlayScene::GetScore() + 1);

				}
				break;
			}
		}

	}

	// Collision detection between Arrows and Blocks
	for (unsigned i = 0; i < m_pArrows.size(); i++)
	{
		for (unsigned j = 0; j < m_pBlocks.size(); j++)
		{
			if (CollisionManager::AABBCheck(m_pBlocks[j], m_pArrows[i]))
			{
				if (m_pBlocks[j]->GetOverTough() == true)
				{
					delete m_pBlocks[j];
					m_pBlocks.erase(std::remove(m_pBlocks.begin(), m_pBlocks.end(), m_pBlocks[j]), m_pBlocks.end());
				}
				break;
			}
		}
	}

	// Collision detection between Arrows and Fixed_Blocks
	for (unsigned i = 0; i < m_pArrows.size(); i++)
	{
		for (unsigned j = 0; j < m_pFixedBlocks.size(); j++)
		{
			if (CollisionManager::AABBCheck(m_pFixedBlocks[j], m_pArrows[i]))
			{
				break;
			}
		}
	}


	for (unsigned i = 0; i < m_pFixedBlocks.size(); i++)
	{
		for (unsigned j = 0; j < m_pBlocks.size(); j++)
		{
			if (CollisionManager::AABBCheck(m_pFixedBlocks[i], m_pBlocks[j]))
			{
				if (m_pBlocks[j]->GetOverTough() == true)
				{
					delete m_pBlocks[j];
					m_pBlocks.erase(std::remove(m_pBlocks.begin(), m_pBlocks.end(), m_pBlocks[j]), m_pBlocks.end());
				}
				break;
			}
		}
	}

	// Collision detection between Blocks
	if (m_pBlocks.size() >= 2)
	{
		for (unsigned i = 0; i < m_pBlocks.size(); i++)
		{
			for (unsigned j = i + 1; j < m_pBlocks.size(); j++)
			{
				const int SIZE = m_pBlocks.size();
				if (CollisionManager::AABBCheck(m_pBlocks[i], m_pBlocks[j]))
				{
					if (m_pBlocks[i]->GetOverTough() == true)
					{
						delete m_pBlocks[i];
						m_pBlocks.erase(std::remove(m_pBlocks.begin(), m_pBlocks.end(), m_pBlocks[i]), m_pBlocks.end());

					}
					if (SIZE > m_pBlocks.size())
					{
						--j;
						if (m_pBlocks[j]->GetOverTough() == true)
						{
							delete m_pBlocks[j];
							m_pBlocks.erase(std::remove(m_pBlocks.begin(), m_pBlocks.end(), m_pBlocks[j]), m_pBlocks.end());
						}
					}
					else
					{
						if (m_pBlocks[j]->GetOverTough() == true)
						{
							delete m_pBlocks[j];
							m_pBlocks.erase(std::remove(m_pBlocks.begin(), m_pBlocks.end(), m_pBlocks[j]), m_pBlocks.end());
						}
					}
					break;
				}
			}
		}
	}
}

void PhysicsEngine::SetMaterial(MaterialType type)
{
	switch (type)
	{
	case MaterialType::WOOD:
		material = "WOOD";
		materialCOR = 0.6f;
		break;
	case MaterialType::STEEL:
		material = "STEEL";
		materialCOR = 0.5f;
		break;
	case MaterialType::RUBBER:
		material = "RUBBER";
		materialCOR = 0.8f;
		break;
	case MaterialType::TUNGSTEN:
		material = "TUNGSTEN";
		materialCOR = 0.3f;
		break;
	}
}

void PhysicsEngine::SetMovementVals(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type)
{
	SetMass(mass);
	SetAngle(angle);
	SetSpeed(speed);
	SetGravity(gravity);
	SetAccelerationGravity();
	SetDamping(damping);
	SetVelocity(Util::AngleMagnitudeToVec2(launchAngleDegrees, launchSpeed));
	SetMomentum(abs(mass * GetVelocity()));

	SetStartingX(startX);
	SetStartingY(startY);
	SetType(type);
}


void PhysicsEngine::ProjectileMovement()
{
	//Projectile Movement
	if (Game::Instance().GetCurScene() == SceneState::PLAY)
	{
		netForce = accelerationGravity;
		// a = F/m
		acceleration = netForce / mass;
		velocity += acceleration * deltaTime;
		GetTransform()->position += velocity * deltaTime;
		momentum = abs(mass * velocity);
	}
	// Checking if object is collided with boundary or not
	//if (isCollided == true)
	//{
	//	// On ground, appliedForce and friction are applied
	//	netForce = appliedForce + friction;
	//}
	//else
	//{
	//	// In air, only gravity is applied
	//	netForce = accelerationGravity;
	//}
}

Transform* PhysicsEngine::GetTransform()
{
	return &m_transform;
}

RigidBody* PhysicsEngine::GetRigidBody()
{
	return &m_rigidBody;
}

int PhysicsEngine::GetWidth() const
{
	return m_width;
}

int PhysicsEngine::GetHeight() const
{
	return m_height;
}

GameObjectType PhysicsEngine::GetType() const
{
	return m_type;
}

void PhysicsEngine::SetWidth(const int new_width)
{
	m_width = new_width;
}

void PhysicsEngine::SetHeight(const int new_height)
{
	m_height = new_height;
}

void PhysicsEngine::SetType(const GameObjectType new_type)
{
	m_type = new_type;
}

void PhysicsEngine::SetEnabled(const bool state)
{
	m_enabled = state;
}

bool PhysicsEngine::IsEnabled() const
{
	return m_enabled;
}

void PhysicsEngine::SetVisible(const bool state)
{
	m_visible = state;
}

bool PhysicsEngine::IsVisible() const
{
	return m_visible;
}

SpriteSheet* PhysicsEngine::GetSpriteSheet()
{
	return m_pSpriteSheet;
}

Animation& PhysicsEngine::GetAnimation(const std::string& name)
{
	return m_pAnimations[name];
}

void PhysicsEngine::SetSpriteSheet(SpriteSheet* sprite_sheet)
{
	m_pSpriteSheet = sprite_sheet;
}

void PhysicsEngine::SetAnimation(const Animation& animation)
{
	if (!CheckIfAnimationExists(animation.name))
	{
		m_pAnimations[animation.name] = animation;
	}
}

bool PhysicsEngine::CheckIfAnimationExists(const std::string& id)
{
	return m_pAnimations.find(id) != m_pAnimations.end();
}

