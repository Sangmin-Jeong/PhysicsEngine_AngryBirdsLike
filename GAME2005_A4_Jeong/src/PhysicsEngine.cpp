#include "PhysicsEngine.h"
#include "Util.h"
#include "CollisionManager.h"
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
	else if (GetType() == GameObjectType::PLANE)
	{
		m_pTargets.push_back(this);
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
	if (GetType() == GameObjectType::PROJECTILE || GetType() == GameObjectType::PLANE)
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
		for (unsigned j = 0; j < m_pTargets.size(); j++)
		{
			if (CollisionManager::CircleCircleCheck(m_pProjectiles[i], m_pTargets[j]))
			{
				// p = m * v (Momentum = Mass * Velocity)
				// add each projectile�s momentum to the total momentum when it collided, using the formula 
				totalMomentum = m_pProjectiles[i]->GetMomentum() + m_pTargets[j]->GetMomentum();
				glm::vec2 direction = m_pTargets[j]->GetTransform()->position - m_pProjectiles[i]->GetTransform()->position;
				glm::vec2 nomal_direction = Util::Normalize(direction);

				// To convert momentum to velocity, using the formula velocity equal total momentum divided by 2 and divided by mass.
				// v = p / m (Velocity = (totalMomentum / 2) / mass) 
				m_pProjectiles[i]->SetVelocity(-nomal_direction * ((totalMomentum * 0.5f) / m_pProjectiles[i]->GetMass()));
				m_pTargets[j]->SetVelocity(nomal_direction * ((totalMomentum * 0.5f) / m_pTargets[j]->GetMass()));


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

	// Check Collision between Projectiles and Boundaries
	for (unsigned i = 0; i < m_pProjectiles.size(); i++)
	{
		for (unsigned k = 0; k < m_pBoundaries.size(); k++)
		{
			if (CollisionManager::LineCircleCheck(m_pBoundaries[k], m_pProjectiles[i]));
			{
				if (m_pProjectiles[i]->GetIsCollided() == true)
				{
					// Normal Force
					float mgy = accelerationGravity.y * cos((m_pBoundaries[k]->GetAngle() - 90.0f) * Util::Deg2Rad);
					m_pProjectiles[i]->SetNormalForce(m_pBoundaries[k]->GetNormal() * mgy);

					// Applied Force 
					float mgx = accelerationGravity.y * sin((m_pBoundaries[k]->GetAngle() - 90.0f) * Util::Deg2Rad);
					m_pProjectiles[i]->SetAppliedForce(Util::AngleMagnitudeToVec2(m_pBoundaries[k]->GetAngle() + 90.0f, mgx));

					// Direction of friction
					glm::vec2 normalFriction = Util::Normalize(-appliedForce);

					//to get the friction force, we used the formula, 
					// FrictionForce = coefficients of friction * parallel NormalForce(mgx)
					m_pProjectiles[i]->SetFriction((mgx * m_pProjectiles[i]->GetDamping()) * normalFriction);

					break;
				}
			}
		}
	}

	// Check Collision between Target and Boundaries
	for (unsigned l = 0; l < m_pTargets.size(); l++)
	{
		for (unsigned k = 0; k < m_pBoundaries.size(); k++)
		{
			if (CollisionManager::LineCircleCheck(m_pBoundaries[k], m_pTargets[l]));
			{
				if (m_pTargets[l]->GetIsCollided() == true)
				{

					// Normal Force
					float mgy = accelerationGravity.y * cos((m_pBoundaries[k]->GetAngle() - 90.0f) * Util::Deg2Rad);
					m_pTargets[l]->SetNormalForce(m_pBoundaries[k]->GetNormal() * mgy);

					// Applied Force 
					float mgx = accelerationGravity.y * sin((m_pBoundaries[k]->GetAngle() - 90.0f) * Util::Deg2Rad);
					m_pTargets[l]->SetAppliedForce(Util::AngleMagnitudeToVec2(m_pBoundaries[k]->GetAngle() + 90.0f, mgx));

					// Direction of friction
					glm::vec2 normalFriction = Util::Normalize(-appliedForce);

					// FrictionForce = coefficients of friction * parallel NormalForce
					m_pTargets[l]->SetFriction((mgx * m_pTargets[l]->GetDamping()) * normalFriction);

					break;
				}
			}
		}
	}

	// Collision detection between Targets
	if (m_pTargets.size() >= 2)
	{
		for (unsigned i = 0; i < m_pTargets.size(); i++)
		{
			// to avoid increasing 'i' is bigger than amount of objects in array
			if (i + 1 < m_pTargets.size())
				if (CollisionManager::CircleCircleCheck(m_pTargets[i], m_pTargets[i + 1]))
				{
					// p = m * v (Momentum = Mass * Velocity)
					totalMomentum = m_pTargets[i]->GetMomentum() + m_pTargets[i + 1]->GetMomentum();
					glm::vec2 direction = m_pTargets[i + 1]->GetTransform()->position - m_pTargets[i]->GetTransform()->position;
					glm::vec2 nomal_direction = Util::Normalize(direction);

					// v = p / m (Velocity = (totalMomentum / 2) / mass) 
					m_pTargets[i]->SetVelocity(-nomal_direction * ((totalMomentum * 0.5f) / m_pTargets[i]->GetMass()));
					m_pTargets[i + 1]->SetVelocity(nomal_direction * ((totalMomentum * 0.5f) / m_pTargets[i + 1]->GetMass()));

					break;
				}
		}
	}

	// Collision detection between Projectiles
	if (m_pProjectiles.size() >= 2)
	{
		for (unsigned i = 0; i < m_pProjectiles.size(); i++)
		{
			// to avoid increasing 'i' is bigger than amount of objects in array
			if (i + 1 < m_pProjectiles.size())
				if (CollisionManager::CircleCircleCheck(m_pProjectiles[i], m_pProjectiles[i + 1]))
				{
					// p = m * v (Momentum = Mass * Velocity)
					totalMomentum = m_pProjectiles[i]->GetMomentum() + m_pProjectiles[i + 1]->GetMomentum();
					glm::vec2 direction = m_pProjectiles[i + 1]->GetTransform()->position - m_pProjectiles[i]->GetTransform()->position;
					glm::vec2 nomal_direction = Util::Normalize(direction);

					// v = p / m (Velocity = (totalMomentum / 2) / mass)
					// Suppose that Wooden ball's Coefficient of restitution is 0.6
					// Rubber ball's Coefficient of restitution is 0.8 , Steel ball's 0.5
					cout << m_pProjectiles[i]->GetMaterialCOR() << " / " << m_pProjectiles[i]->GetMaterial() << endl;
					m_pProjectiles[i]->SetVelocity(-nomal_direction * ((totalMomentum * m_pProjectiles[i]->GetMaterialCOR()) / m_pProjectiles[i]->GetMass()));
					m_pProjectiles[i + 1]->SetVelocity(nomal_direction * ((totalMomentum * m_pProjectiles[i + 1]->GetMaterialCOR()) / m_pProjectiles[i + 1]->GetMass()));

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

	// Collision detection between Blocks and Fixed_Blocks
	for (unsigned i = 0; i < m_pFixedBlocks.size(); i++)
	{
		for (unsigned j = 0; j < m_pBlocks.size(); j++)
		{
			if (CollisionManager::AABBCheck(m_pFixedBlocks[i], m_pBlocks[j]))
			{
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
				if (CollisionManager::AABBCheck(m_pBlocks[i], m_pBlocks[j]))
				{
					break;
				}
			}
		}
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

	// Checking if object is collided with boundary or not
	if (isCollided == true)
	{
		// On ground, appliedForce and friction are applied
		netForce = appliedForce + friction;
	}
	else
	{
		// In air, only gravity is applied
		netForce = accelerationGravity;
	}

	// a = F/m
	acceleration = netForce / mass;
	velocity += acceleration * deltaTime;
	GetTransform()->position += velocity * deltaTime;
	momentum = abs(mass * velocity);
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
