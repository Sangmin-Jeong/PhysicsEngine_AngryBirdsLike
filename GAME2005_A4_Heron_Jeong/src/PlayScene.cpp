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
	physicsEngine.AddObject(new Projectile(10.0f, angle, speed, gravity, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 32, GameObjectType::PROJECTILE));


	// Targets
	physicsEngine.AddObject(new Enemy(10, 0, 0, gravity, 0.5f, 450, groundPoint.y - 32, GameObjectType::ENEMY));
	physicsEngine.AddObject(new Enemy(10, 0, 0, gravity, 0.5f, 550, groundPoint.y - 32, GameObjectType::ENEMY));

	// Boundaries
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(350,300), 230.0f));
	physicsEngine.AddBoundary(new Boundary(groundPoint, 90.0f, 1000.0f));
	physicsEngine.AddBoundary(new Boundary(glm::vec2(700,400), 135.0f, 1000.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(slingshotPoint.x + 20, slingshotPoint.y + 50), 0.0f, 50.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(slingshotPoint.x, slingshotPoint.y), 90.0f, 10.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(800, 400), 0.0f, 1000.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(0, 400), 0.0f, 1000.0f));
	//physicsEngine.AddBoundary(new Boundary(glm::vec2(400, 000), 90.0f, 1000.0f));

	const SDL_Color red = { 255, 0, 0, 255 };
	m_pPointLabel = new Label("Score: " + to_string(score), "Consolas", 20, red, glm::vec2(50, 30));
	AddChild(m_pPointLabel);


	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });

}

void PlayScene::Draw()
{
	DrawDisplayList();

	// For Checking and drawing highest point 
	//if (!physicsEngine.GetProjectiles().empty())
	//{
	//	if (physicsEngine.GetProjectiles()[0]->GetVelocity().y >= 0 && check == true)
	//	{
	//		isHighestPoint = true;
	//		highestPoint = glm::vec2(physicsEngine.GetProjectiles()[0]->GetTransform()->position.x, physicsEngine.GetProjectiles()[0]->GetTransform()->position.y - (physicsEngine.GetProjectiles()[0]->GetHeight() / 2));
	//		const SDL_Color red = { 255, 0, 0, 255 };
	//		m_pPointLabel = new Label("X: " + to_string(highestPoint.x), "Consolas", 20, red, glm::vec2(highestPoint.x + 20, highestPoint.y - 40));
	//		m_pPointLabel->SetParent(this);
	//		AddChild(m_pPointLabel);
	//		m_pPointLabel2 = new Label("Y: " + to_string(highestPoint.y), "Consolas", 20, red, glm::vec2(highestPoint.x + 20, highestPoint.y - 20));
	//		m_pPointLabel2->SetParent(this);
	//		AddChild(m_pPointLabel2);
	//		check = false;
	//	}
	//}
	//if (isHighestPoint == true)
	//{
	//	Util::DrawFilledRect(highestPoint, 10, 10, glm::vec4(1, 0, 0, 1));
	//}


	/*const SDL_Color green = { 125, 255, 190, 255 };*/


	// Draw Physics Objects
	if (!physicsEngine.GetProjectiles().empty())
	{
		for (int i = 0; i < physicsEngine.GetProjectiles().size(); i++)
		{
			physicsEngine.GetProjectiles()[i]->Draw();
			
		}
	}

	if (!physicsEngine.GetTargets().empty())
	{
		for (int i = 0; i < physicsEngine.GetTargets().size(); i++)
		{
			physicsEngine.GetTargets()[i]->Draw();

		}
	}

	if (!physicsEngine.GetBoundaries().empty())
	{
		for (int i = 0; i < physicsEngine.GetBoundaries().size(); i++)
		{
			physicsEngine.GetBoundaries()[i]->Draw();
		}
	}

	Util::DrawFilledRect(slingshotPoint, 20, 100, RED);

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
	//Score
	m_pPointLabel->SetText("Score: " + to_string(score));

	//Time
	//currentTime = SDL_GetTicks();
	//elapsedTime = (currentTime - startTime) / 1000.0;
	//cout << elapsedTime << endl;

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

	if (!physicsEngine.GetTargets().empty())
	{
		for (int i = 0; i < physicsEngine.GetTargets().size(); i++)
		{
			physicsEngine.GetTargets()[i]->Update();
		}
	}

	if (!physicsEngine.GetBoundaries().empty())
	{
		for (int i = 0; i < physicsEngine.GetBoundaries().size(); i++)
		{
			physicsEngine.GetBoundaries()[i]->Update();
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
	else if(EventManager::Instance().MouseReleased(1) && IsSelected)
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

void PlayScene::ResetObject()
{

	if(!physicsEngine.GetProjectiles().empty())
	{
		physicsEngine.RemoveProjectile();
		//physicsEngine.RemoveProjectile();
		//physicsEngine.RemoveProjectile();
		//physicsEngine.RemoveProjectile();
		/*RemoveAllChildren();*/
		isHighestPoint = false;
		check = true;
	}
	//physicsEngine.AddObject(new Projectile(2.0f, angle, speed, gravity, 0.1f, physicsEngine.GetBoundaries().back()->GetStartPoint().x, physicsEngine.GetBoundaries().back()->GetStartPoint().y - 32.0f, GameObjectType::PROJECTILE));

	//physicsEngine.AddObject(new Projectile(20.0f, angle, speed, gravity, 0.5f, 300,  300, GameObjectType::PROJECTILE));
	//physicsEngine.GetProjectiles().back()->SetColor(RED);

	physicsEngine.AddObject(new Projectile(10.0f, angle, speed, gravity, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 32, GameObjectType::PROJECTILE));

	//physicsEngine.AddObject(new Projectile(8.0f, angle, speed, gravity, 0.1f, 600, 100, GameObjectType::PROJECTILE));
	//physicsEngine.GetProjectiles().back()->SetColor(BLUE);

	//physicsEngine.AddObject(new Projectile(8.0f, angle, speed, gravity, 0.8f, 700, 100, GameObjectType::PROJECTILE));
	//physicsEngine.GetProjectiles().back()->SetColor(YELLOW);

	if (!physicsEngine.GetTargets().empty())
	{
		for (unsigned i = 0; i < physicsEngine.GetTargets().size(); i++)
		{
			physicsEngine.RemoveTarget();
		}
		
	}

	physicsEngine.AddObject(new Enemy(10, 0, 0, 9.8f, 0.5f, 450, groundPoint.y - 32, GameObjectType::ENEMY));
	physicsEngine.AddObject(new Enemy(10, 0, 0, 9.8f, 0.5f, 550, groundPoint.y - 32, GameObjectType::ENEMY));


	score = 0;
}

void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}

	// Lunch a projectile
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_SPACE))
	{
		ResetObject();
	}
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Game2005_A3_Heron_Jeong", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

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

