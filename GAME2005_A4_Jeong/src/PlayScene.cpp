#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	// Projectile
	physicsEngine.AddObject(new Projectile(10.0f, 0, 0, 0, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 32, GameObjectType::PROJECTILE, MaterialType::STEEL));
	physicsEngine.GetProjectiles().back()->SetGravity(0.0f);
	physicsEngine.GetProjectiles().back()->SetAccelerationGravity();

	// Enemies
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 550, groundPoint.y - 36, GameObjectType::ENEMY, MaterialType::STEEL));
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 650, groundPoint.y - 36, GameObjectType::ENEMY, MaterialType::STEEL));
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 600, 140, GameObjectType::ENEMY, MaterialType::STEEL));


	// Boundaries
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(350,300), 230.0f));
	//physicsEngine.AddBoundary(new Boundary(groundPoint, 90.0f, 1000.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(700,400), 135.0f, 1000.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(slingshotPoint.x + 20, slingshotPoint.y + 50), 0.0f, 50.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(slingshotPoint.x, slingshotPoint.y), 90.0f, 10.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(800, 400), 0.0f, 1000.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(0, 400), 0.0f, 1000.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(400, 000), 90.0f, 1000.0f));

	// Fixed-Block
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 0, 0, 0, groundPoint.y, GameObjectType::BLOCK_FIX, MaterialType::STEEL));
	physicsEngine.GetFixedBlocks().back()->SetWidth(800);
	physicsEngine.GetFixedBlocks().back()->SetHeight(50);

	// Blocks
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 450, 450, GameObjectType::BLOCK, MaterialType::STEEL));
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 450, 360, GameObjectType::BLOCK, MaterialType::STEEL));
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 450, 270, GameObjectType::BLOCK, MaterialType::STEEL));
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 700, 450, GameObjectType::BLOCK, MaterialType::STEEL));
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 700, 360, GameObjectType::BLOCK, MaterialType::STEEL));
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 700, 270, GameObjectType::BLOCK, MaterialType::STEEL));
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 450, 200, GameObjectType::BLOCK, MaterialType::STEEL));
	physicsEngine.GetBlocks().back()->SetWidth(300);
	physicsEngine.GetBlocks().back()->SetHeight(50);

	const SDL_Color red = { 255, 0, 0, 255 };
	m_pPointLabel = new Label("Score: " + to_string(score), "Consolas", 20, red, glm::vec2(50, 30));
	AddChild(m_pPointLabel);


	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });

}

void PlayScene::Draw()
{
	DrawDisplayList();

	// Draw Physics Objects
	if (!physicsEngine.GetProjectiles().empty())
	{
		for (int i = 0; i < physicsEngine.GetProjectiles().size(); i++)
		{
			physicsEngine.GetProjectiles()[i]->Draw();

		}
	}

	if (!physicsEngine.GetEnemies().empty())
	{
		for (int i = 0; i < physicsEngine.GetEnemies().size(); i++)
		{
			physicsEngine.GetEnemies()[i]->Draw();
			//Util::DrawFilledRect(physicsEngine.GetEnemies()[i]->GetTransform()->position, 10, 10, RED);
		}
	}

	if (!physicsEngine.GetBoundaries().empty())
	{
		for (int i = 0; i < physicsEngine.GetBoundaries().size(); i++)
		{
			physicsEngine.GetBoundaries()[i]->Draw();
		}
	}

	if (!physicsEngine.GetBlocks().empty())
	{
		for (int i = 0; i < physicsEngine.GetBlocks().size(); i++)
		{
			physicsEngine.GetBlocks()[i]->Draw();
		}
	}

	if (!physicsEngine.GetFixedBlocks().empty())
	{
		for (int i = 0; i < physicsEngine.GetFixedBlocks().size(); i++)
		{
			physicsEngine.GetFixedBlocks()[i]->Draw();
		}
	}

	if (!physicsEngine.GetArrows().empty())
	{
		for (int i = 0; i < physicsEngine.GetArrows().size(); i++)
		{
			physicsEngine.GetArrows()[i]->Draw();
		}
	}

	Util::DrawFilledRect(slingshotPoint, 20, 150, RED);

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
	//Score
	m_pPointLabel->SetText("Score: " + to_string(score));

	//cout << physicsEngine.GetEnemies().back()->GetMomentum().x << " / " << physicsEngine.GetEnemies().back()->GetMomentum().y << endl;
	//cout << physicsEngine.GetProjectiles().back()->GetMomentum().x << " / " << physicsEngine.GetProjectiles().back()->GetMomentum().y << endl;

	//Time
	currentTime = SDL_GetTicks();
	elapsedTime = (currentTime - startTime) / 1000.0;
	/*cout << elapsedTime << endl;*/

	if (!physicsEngine.GetProjectiles().empty())
	{
		for (int i = 0; i < physicsEngine.GetProjectiles().size(); i++)
		{
			physicsEngine.GetProjectiles()[i]->Update();
			//cout << physicsEngine.GetProjectiles()[0]->GetFriction().x << " / " << physicsEngine.GetProjectiles()[0]->GetFriction().y << endl;
			//cout << physicsEngine.GetProjectiles()[i]->GetAccel().x << " / " << physicsEngine.GetProjectiles()[i]->GetAccel().y << endl;
			//cout << physicsEngine.GetProjectiles()[0]->GetNormalForce().x << " / " << physicsEngine.GetProjectiles()[0]->GetNormalForce().y << endl;
			//cout << physicsEngine.GetProjectiles()[0]->GetVelocity().x << " / " << physicsEngine.GetProjectiles()[0]->GetVelocity().y << endl;
			//cout << physicsEngine.GetProjectiles()[0]->GetAppliedForce().x << " / " << physicsEngine.GetProjectiles()[0]->GetAppliedForce().y << endl;
			//cout << physicsEngine.GetProjectiles()[0]->GetMomentum().x << " / " << physicsEngine.GetProjectiles()[0]->GetMomentum().y << endl;
			/*cout << physicsEngine.GetProjectiles()[0]->GetIsCollided() << endl;*/
			//cout << physicsEngine.GetProjectiles()[0]->GetGravityX() << " / " << physicsEngine.GetProjectiles()[0]->GetGravityY() << endl;
			//cout << physicsEngine.GetProjectiles()[0]->GetAccel().x << " / " << physicsEngine.GetProjectiles()[0]->GetAccel().y << endl;
			//cout << physicsEngine.GetProjectiles()[0]->GetAppliedForce().x + physicsEngine.GetProjectiles()[0]->GetFriction().x << " / " << physicsEngine.GetProjectiles()[0]->GetAppliedForce().y + physicsEngine.GetProjectiles()[0]->GetFriction().y << endl;

		}
	}

	if (!physicsEngine.GetBoundaries().empty())
	{
		for (int i = 0; i < physicsEngine.GetBoundaries().size(); i++)
		{
			physicsEngine.GetBoundaries()[i]->Update();
		}
	}

	if (!physicsEngine.GetArrows().empty())
	{
		for (int i = 0; i < physicsEngine.GetArrows().size(); i++)
		{
			physicsEngine.GetArrows()[i]->Update();
		}
	}

	if (!physicsEngine.GetBlocks().empty())
	{
		for (int i = 0; i < physicsEngine.GetBlocks().size(); i++)
		{
			physicsEngine.GetBlocks()[i]->Update();
			//cout << physicsEngine.GetBlocks()[0]->GetVelocity().x << " / " << physicsEngine.GetBlocks()[0]->GetVelocity().y << endl;
			//cout << physicsEngine.GetBlocks()[0]->GetWidth() << " / " << physicsEngine.GetBlocks()[0]->GetHeight() << endl;

		}
	}

	if (!physicsEngine.GetFixedBlocks().empty())
	{
		for (int i = 0; i < physicsEngine.GetFixedBlocks().size(); i++)
		{
			physicsEngine.GetFixedBlocks()[i]->Update();
		}
	}

	if (!physicsEngine.GetEnemies().empty())
	{
		for (int i = 0; i < physicsEngine.GetEnemies().size(); i++)
		{
			physicsEngine.GetEnemies()[i]->Update();

		}
	}
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	GetKeyboardInput();

	if (!physicsEngine.GetProjectiles().empty())
	{
		// Checking if Projectile is clicked or not
		if (Util::Distance(EventManager::Instance().GetMousePosition(), physicsEngine.GetProjectiles().back()->GetTransform()->position)
			< physicsEngine.GetProjectiles().back()->GetWidth())

		{
			IsSelected = true;
		}
		else
		{
			IsSelected = false;
		}

		// Slingshot

		// Pressed mouse button
		if (EventManager::Instance().GetMouseButton(0) && !EventManager::Instance().MouseReleased(1) && IsSelected)
		{
			// Distance check between mouse_point and slingshot_point
			auto distance = Util::Distance(EventManager::Instance().GetMousePosition(), slingshotPoint);

			// Stop projectile movement when it is clicked
			physicsEngine.GetProjectiles().back()->SetVelocity(glm::vec2(0.0f, 0.0f));

			// Enough close mouse_position and projectile_position
			if (distance < 200)
			{
				physicsEngine.GetProjectiles().back()->GetTransform()->position = EventManager::Instance().GetMousePosition();
			}

		}
		// Released mouse button
		else if (EventManager::Instance().MouseReleased(1) && IsSelected)
		{
			speed = Util::Distance(physicsEngine.GetProjectiles().back()->GetTransform()->position, slingshotPoint);
			glm::vec2 direction = slingshotPoint - physicsEngine.GetProjectiles().back()->GetTransform()->position;
			glm::vec2 nomal_direction = Util::Normalize(direction);

			physicsEngine.GetProjectiles().back()->SetGravity(9.8f);
			physicsEngine.GetProjectiles().back()->SetAccelerationGravity();

			physicsEngine.GetProjectiles().back()->SetVelocity(nomal_direction * (speed * slingshotPower / physicsEngine.GetProjectiles().back()->GetMass()));

			speed = 0.0f;

		}
	}

	if (!physicsEngine.GetArrows().empty())
	{
		// Checking if Projectile is clicked or not
		if (Util::Distance(EventManager::Instance().GetMousePosition(), physicsEngine.GetArrows().back()->GetTransform()->position)
			< physicsEngine.GetArrows().back()->GetWidth())

		{
			IsSelected = true;
		}
		else
		{
			IsSelected = false;
		}

		// Slingshot

		// Pressed mouse button
		if (EventManager::Instance().GetMouseButton(0) && !EventManager::Instance().MouseReleased(1) && IsSelected)
		{
			// Distance check between mouse_point and slingshot_point
			auto distance = Util::Distance(EventManager::Instance().GetMousePosition(), slingshotPoint);

			// Stop projectile movement when it is clicked
			physicsEngine.GetArrows().back()->SetVelocity(glm::vec2(0.0f, 0.0f));

			// Enough close mouse_position and projectile_position
			if (distance < 200)
			{
				physicsEngine.GetArrows().back()->GetTransform()->position = EventManager::Instance().GetMousePosition();
			}

		}
		// Released mouse button
		else if (EventManager::Instance().MouseReleased(1) && IsSelected)
		{
			speed = Util::Distance(physicsEngine.GetArrows().back()->GetTransform()->position, slingshotPoint);
			glm::vec2 direction = slingshotPoint - physicsEngine.GetArrows().back()->GetTransform()->position;
			glm::vec2 nomal_direction = Util::Normalize(direction);

			physicsEngine.GetArrows().back()->SetGravity(9.8f);
			physicsEngine.GetArrows().back()->SetAccelerationGravity();

			physicsEngine.GetArrows().back()->SetVelocity(nomal_direction * (speed * slingshotPower / physicsEngine.GetArrows().back()->GetMass()));

			speed = 0.0f;

		}
	}
}

void PlayScene::ChangeProjectile()
{
	if (!physicsEngine.GetProjectiles().empty())
	{
		physicsEngine.RemoveProjectile();
		physicsEngine.AddObject(new Arrow(20.0f, angle, speed, gravity, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 15, GameObjectType::ARROW, MaterialType::STEEL));
		slingshotPower = 20.0f;
	}
	else if (!physicsEngine.GetArrows().empty())
	{
		physicsEngine.RemoveArrow();
		physicsEngine.AddObject(new Projectile(10.0f, angle, speed, gravity, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 32, GameObjectType::PROJECTILE, MaterialType::STEEL));
		slingshotPower = 10.0f;
	}

}

void PlayScene::ResetObject()
{

	if (!physicsEngine.GetProjectiles().empty())
	{
		physicsEngine.RemoveProjectile();
		//physicsEngine.RemoveProjectile();
	}
	physicsEngine.AddObject(new Projectile(10.0f, angle, speed, gravity, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 32, GameObjectType::PROJECTILE, MaterialType::STEEL));

	//physicsEngine.AddObject(new Projectile(20.0f, angle + 90.0f, speed, gravity, 0.5f, 600, 300, GameObjectType::PROJECTILE, MaterialType::WOOD));

	//if (!physicsEngine.GetArrows().empty())
	//{
	//	physicsEngine.RemoveArrow();
	//	RemoveAllChildren();
	//}
	//physicsEngine.AddObject(new Arrow(20.0f, angle, speed, gravity, 0.5f, 200, 300, GameObjectType::ARROW, MaterialType::STEEL));

	if (!physicsEngine.GetBlocks().empty())
	{
		physicsEngine.RemoveBlock();
		physicsEngine.RemoveBlock();
		physicsEngine.RemoveBlock();
		
	}
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 400, 400, GameObjectType::BLOCK, MaterialType::TUNGSTEN));
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 350, 250, GameObjectType::BLOCK, MaterialType::TUNGSTEN));
	physicsEngine.AddObject(new Block(blockMass, 0, 0, 9.8f, 0, 400, 100, GameObjectType::BLOCK, MaterialType::TUNGSTEN));


	//RemoveAllChildren();
	score = 0;
}

void PlayScene::GetKeyboardInput()
{
	// Timer to avoid double input.
	if (elapsedTime > 0.5)
	{
		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
		{
			Game::Instance().Quit();
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
		{
			Game::Instance().ChangeSceneState(SceneState::START);
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
		{
			Game::Instance().ChangeSceneState(SceneState::END);
		}
		// Reset a projectile
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_SPACE))
		{
			ResetObject();
			startTime = SDL_GetTicks();
		}
		// Change a projectile
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_C))
		{
			ChangeProjectile();
			startTime = SDL_GetTicks();
		}
	}
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Game2005_A4_Jeong", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	//ImGui::Text("Player Input");
	//ImGui::RadioButton("Keyboard / Mouse", &m_pCurrentInputType, static_cast<int>(InputType::KEYBOARD_MOUSE)); ImGui::SameLine();
	//ImGui::RadioButton("Game Controller", &m_pCurrentInputType, static_cast<int>(InputType::GAME_CONTROLLER)); ImGui::SameLine();
	//ImGui::RadioButton("Both", &m_pCurrentInputType, static_cast<int>(InputType::ALL));

	ImGui::Separator();

	if(ImGui::Button("Reset objects"))
	{
		ResetObject();
		/*std::cout << "My Button Pressed" << std::endl;*/
	}

	if (!physicsEngine.GetProjectiles().empty())
	{
		//ImGui::Text("Incline Angle");
		//float HPangle = physicsEngine.GetBoundaries().back()->GetAngle();
		//ImGui::SliderFloat("Angle", &HPangle, 0, 360.0);
		//physicsEngine.GetBoundaries().back()->SetAngle(HPangle);

		//ImGui::Separator();
		//ImGui::Text("RED CIRCLE");
		//float fricV = physicsEngine.GetProjectiles()[0]->GetDamping();
		//float massV = physicsEngine.GetProjectiles()[0]->GetMass();
		//ImGui::SliderFloat("Mass(RED)", &massV, 0.1, 20.0);
		//ImGui::SliderFloat("Friction(RED)", &fricV, 0.0, 2.0);
		//physicsEngine.GetProjectiles()[0]->SetDamping(fricV);
		//physicsEngine.GetProjectiles()[0]->SetMass(massV);
		//physicsEngine.GetProjectiles()[0]->SetAccelerationGravity();

		//ImGui::Separator();
		//ImGui::Text("GREEN CIRCLE");
		//float fricV2 = physicsEngine.GetProjectiles()[1]->GetDamping();
		//float massV2 = physicsEngine.GetProjectiles()[1]->GetMass();
		//ImGui::SliderFloat("Mass(GREEN)", &massV2, 0.1, 20.0);
		//ImGui::SliderFloat("Friction(GREEN)", &fricV2, 0.0, 2.0);
		//physicsEngine.GetProjectiles()[1]->SetDamping(fricV2);
		//physicsEngine.GetProjectiles()[1]->SetMass(massV2);
		//physicsEngine.GetProjectiles()[1]->SetAccelerationGravity();

		//ImGui::Separator();
		//ImGui::Text("BLUE CIRCLE");
		//float fricV3 = physicsEngine.GetProjectiles()[2]->GetDamping();
		//float massV3 = physicsEngine.GetProjectiles()[2]->GetMass();
		//ImGui::SliderFloat("Mass(BLUE)", &massV3, 0.1, 10.0);
		//ImGui::SliderFloat("Friction(BLUE)", &fricV3, 0.0, 10.0);
		//physicsEngine.GetProjectiles()[2]->SetDamping(fricV3);
		//physicsEngine.GetProjectiles()[2]->SetMass(massV3);
		//physicsEngine.GetProjectiles()[2]->SetAccelerationGravity();

		//ImGui::Separator();
		//ImGui::Text("YELLOW CIRCLE");
		//float fricV4 = physicsEngine.GetProjectiles()[3]->GetDamping();
		//float massV4 = physicsEngine.GetProjectiles()[3]->GetMass();
		//ImGui::SliderFloat("Mass(YELLOW)", &massV4, 0.1, 10.0);
		//ImGui::SliderFloat("Friction(YELLOW)", &fricV4, 0.0, 10.0);
		//physicsEngine.GetProjectiles()[3]->SetDamping(fricV4);
		//physicsEngine.GetProjectiles()[3]->SetMass(massV4);
		//physicsEngine.GetProjectiles()[3]->SetAccelerationGravity();

		ImGui::Separator();
		ImGui::LabelText("RED Velocity", "x:%f, y:%f", physicsEngine.GetProjectiles()[0]->GetVelocity().x, physicsEngine.GetProjectiles()[0]->GetVelocity().y);
		//ImGui::LabelText("RED Momentum", "x:%f, y:%f", physicsEngine.GetProjectiles()[0]->GetMomentum().x, physicsEngine.GetProjectiles()[0]->GetMomentum().y);
		//ImGui::Separator();
		//ImGui::LabelText("GREEN Velocity", "x:%f, y:%f", physicsEngine.GetProjectiles()[1]->GetVelocity().x, physicsEngine.GetProjectiles()[1]->GetVelocity().y);
		//ImGui::LabelText("GREEN Momentum", "x:%f, y:%f", physicsEngine.GetProjectiles()[1]->GetMomentum().x, physicsEngine.GetProjectiles()[1]->GetMomentum().y);
		//ImGui::Separator();
		//ImGui::LabelText("Total Momentum", "x:%f, y:%f", abs(physicsEngine.GetProjectiles()[1]->GetMomentum().x) + abs(physicsEngine.GetProjectiles()[0]->GetMomentum().x), abs(physicsEngine.GetProjectiles()[1]->GetMomentum().y) + abs(physicsEngine.GetProjectiles()[0]->GetMomentum().y));



	}
	ImGui::Separator();

	//static float float3[3] = { 0.0f, 1.0f, 1.5f };
	//if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	//{
	//	std::cout << float3[0] << std::endl;
	//	std::cout << float3[1] << std::endl;
	//	std::cout << float3[2] << std::endl;
	//	std::cout << "---------------------------\n";
	//}
	
	ImGui::End();
}

