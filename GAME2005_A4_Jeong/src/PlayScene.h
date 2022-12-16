#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Boundary.h"
#include "Projectile.h"
#include "Arrow.h"
#include "Block.h"
#include "PhysicsEngine.h"
#include "CollisionManager.h"
#include "Enemy.h"

//const float PHYSICS_TIMESTEP = 1.0f / 60.0f;

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Start() override;
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	void ChangeProjectile();
	void ResetObject();

	static inline void SetScore(int i) { score = i; }
	static inline int GetScore() { return score; }
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Player* m_pPlayer{};
	bool m_playerFacingRight{};

	//vector<PhysicsEngine*> m_pProjectiles;
	//vector<PhysicsEngine*> m_pEnemies;

	PhysicsEngine physicsEngine;

	// UI Items
	Button* m_pBackButton{};
	Button* m_pNextButton{};
	Label* m_pInstructionsLabel{};

	// Input Control
	int m_pCurrentInputType{};
	//void GetPlayerInput();
	void GetKeyboardInput();

	float angle = 0.0f; 
	float speed = 0.0f;
	float gravity = 0.0f;
	float stY = 400.0f;
	float stX = 100.0f;
	float damp;
	float mass;
	float blockMass = 5.0f;

	bool isHighestPoint;
	bool check = true;
	glm::vec2 highestPoint;
	Label* m_pPointLabel2{};

	// Score
	Label* m_pPointLabel{};
	static inline int score = 0;

	// Time
	uint32_t startTime = SDL_GetTicks();
	uint32_t currentTime;
	double elapsedTime;


	// Slingshot
	glm::vec2 slingshotPoint = glm::vec2(200.0f, 400.0f);
	bool IsSelected = false;
	float slingshotPower = 10.0f;

	glm::vec2 groundPoint = glm::vec2(600, 550);

	glm::vec4 PURPLE = glm::vec4(0.8, 0.6, 0.8, 1);
	glm::vec4 ORANGE = glm::vec4(1, 0.9, 0.8, 1);
	glm::vec4 GREEN = glm::vec4(0.5, 0.8, 0.4, 1);
	glm::vec4 RED = glm::vec4(1, 0.0, 0.0, 1);
	glm::vec4 BLUE = glm::vec4(0, 0, 1, 1);
	glm::vec4 YELLOW = glm::vec4(1, 0.8, 0.1, 1);
};

#endif /* defined (__PLAY_SCENE__) */