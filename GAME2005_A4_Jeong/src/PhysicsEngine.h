#pragma once
#ifndef __PHYSICSENGINE__
#define __PHYSICSENGINE__

#include <iostream>
#include "SpriteSheet.h"
#include "Animation.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Boundary.h"
#include <string>
#include <vector>
#include <cmath>
#include "GameObjectType.h"

using namespace std;

const float PHYSICS_TIMESTEP = 1.0f / 60.0f;

// enums
enum class MaterialType {
	RUBBER,
	WOOD,
	STEEL,
	TUNGSTEN,
};

class PhysicsEngine
{
public:
	PhysicsEngine();
	PhysicsEngine(float mass,float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type, MaterialType mtype);
	virtual ~PhysicsEngine();

	// Draw the object
	virtual void Draw();

	// Update the object
	virtual void Update();

	// getters for common variables
	Transform* GetTransform();

	// getters and setters for physics properties
	RigidBody* GetRigidBody();

	// getters and setters for game object properties
	[[nodiscard]] int GetWidth() const;
	[[nodiscard]] int GetHeight() const;
	void SetWidth(int new_width);
	void SetHeight(int new_height);
	[[nodiscard]] GameObjectType GetType() const;
	void SetType(GameObjectType new_type);

	void SetEnabled(bool state);
	[[nodiscard]] bool IsEnabled() const;

	void SetVisible(bool state);
	[[nodiscard]] bool IsVisible() const;

	SpriteSheet* GetSpriteSheet();
	Animation& GetAnimation(const std::string& name);

	void SetSpriteSheet(SpriteSheet* sprite_sheet);
	void SetAnimation(const Animation& animation);

	
	void ProjectileMovement();

	float GetAngle() { return launchAngleDegrees; }
	float GetSpeed() { return launchSpeed; }
	float GetGravityX() { return gravity.x; }
	float GetGravityY() { return gravity.y; }
	float GetDamping() { return damping; }
	float GetStartingX() { return statingX; }
	float GetStartingY() { return statingY; }
	glm::vec2 GetVelocity() { return velocity; }
	float GetMass() {return mass; }
	glm::vec2 GetAccel() { return acceleration; }
	glm::vec2 GetFriction() { return friction; }
	glm::vec2 GetNormalForce() { return normalForce; }
	glm::vec2 GetAppliedForce() { return appliedForce; }
	bool GetIsCollided() { return isCollided; }
	glm::vec2 GetMomentum() { return momentum; }
	glm::vec2 GetTotalMomentum() { return totalMomentum; }
	float GetMaterialCOR() { return materialCOR; }
	char* GetMaterial() { return material; }

	void SetAngle(float v) { launchAngleDegrees = v; }
	void SetSpeed(float v) { launchSpeed = v; }
	void SetGravity(float v) { gravity = glm::vec2(0, v); }
	void SetAccelerationGravity() { accelerationGravity = glm::vec2(gravity.x * mass, gravity.y * mass); }
	void SetDamping(float v) {  this->damping = v; }
	void SetStartingX(float v) { statingX = v; }
	void SetStartingY(float v) { statingY = v; }
	void SetVelocity(glm::vec2 v) { velocity = v; }
	void SetNormalForce(glm::vec2 v) { normalForce = v; }
	void SetFriction(glm::vec2 v) { friction = v; }
	void SetMass(float v) { this->mass = v; }
	void SetColor(glm::vec4 color) { Default = color; }
	void SetIsCollided(bool b) { isCollided = b; }
	void SetAppliedForce(glm::vec2 v2) { appliedForce = v2; }
	void SetMomentum(glm::vec2 v2) { momentum = v2; }
	void SetTotalMomentum(glm::vec2 v2) { totalMomentum = v2; }

	void SetMovementVals(float mass, float angle, float speed, float gravity, float damping, float startX, float startY, GameObjectType type);

	vector<PhysicsEngine*> GetObjects() { return physicsObjects; }
	vector<PhysicsEngine*> GetProjectiles() { return m_pProjectiles; }
	vector<PhysicsEngine*> GetEnemies() { return m_pEnemies; }
	vector<Boundary*> GetBoundaries() { return m_pBoundaries; }
	vector<PhysicsEngine*> GetBlocks() { return m_pBlocks; }
	vector<PhysicsEngine*> GetFixedBlocks() { return m_pFixedBlocks; }
	vector<PhysicsEngine*> GetArrows() { return m_pArrows; }
	void AddObject(PhysicsEngine* object) { physicsObjects.push_back(object); }
	void RemoveProjectile() { m_pProjectiles.pop_back(); }
	void RemoveEnemy() { m_pEnemies.pop_back(); }
	void RemoveArrow() { m_pArrows.pop_back(); }
	void RemoveBlock() { m_pBlocks.pop_back(); }
	void RemoveFixedBlock() { m_pFixedBlocks.pop_back(); }
	void AddBoundary(Boundary* boundary) { m_pBoundaries.push_back(boundary); }

	void SetSize(int s) { size = s; }
	int GetSize() { return size; }
	float overlap;

	//void SetIsActive(bool active) { isActive = active; }
	//bool GetIsActive() { return isActive; }

private:
	inline static vector<PhysicsEngine*> physicsObjects;
	inline static vector<PhysicsEngine*> m_pProjectiles;
	inline static vector<PhysicsEngine*> m_pEnemies;
	inline static vector<PhysicsEngine*> m_pBlocks;
	inline static vector<PhysicsEngine*> m_pFixedBlocks;
	inline static vector<PhysicsEngine*> m_pArrows;
	inline static vector<Boundary*> m_pBoundaries;
	inline static int size;

	// transform component
	Transform m_transform;

	// rigid body component
	RigidBody m_rigidBody;

	// size variables
	int m_width;
	int m_height;
	GameObjectType m_type;

	bool m_enabled;
	bool m_visible;


	bool CheckIfAnimationExists(const std::string& id);

	SpriteSheet* m_pSpriteSheet;

	// Colors
	glm::vec4 PURPLE = glm::vec4(0.8,0.6,0.8,1);
	glm::vec4 ORANGE = glm::vec4(1, 0.9, 0.8, 1);
	glm::vec4 GREEN = glm::vec4(0.5, 0.8, 0.4, 1);
	glm::vec4 RED = glm::vec4(1, 0.0, 0.0, 1);
	glm::vec4 BLUE = glm::vec4(0, 0, 1, 1);
	glm::vec4 YELLOW = glm::vec4(1, 0.8, 0.1, 1);
	glm::vec4 Default = glm::vec4(0, 1, 0, 1);

	std::unordered_map<std::string, Animation> m_pAnimations;

	// Time
	float deltaTime = PHYSICS_TIMESTEP;

	float launchAngleDegrees;
	float launchSpeed;
	float defaultSpeed;
	
	/*float accelerationGravity; */// could be Vec2 to store direction
									// GravityScale(float)
	float damping;				// Damping(Friction)(float)
	float statingY;
	float statingX;

	glm::vec2 gravity;
	glm::vec2 accelerationGravity;
	glm::vec2 velocity;
	glm::vec2 normalForce;
	glm::vec2 friction;
	float mass;
	glm::vec2 appliedForce;
	glm::vec2 acceleration;
	glm::vec2 netForce;

	glm::vec2 momentum;
	glm::vec2 totalMomentum;

	float materialCOR;
	char* material;

	bool isCollided = false;
	//bool isActive;

	// F=ma , a = F/m
};

#endif /* defined (__PHYSICSENGINE__) */
