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

	// Enemies
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 550, groundPoint.y - 36, GameObjectType::ENEMY, MaterialType::STEEL));
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 650, groundPoint.y - 36, GameObjectType::ENEMY, MaterialType::STEEL));
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 600, 140, GameObjectType::ENEMY, MaterialType::STEEL));

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
	physicsEngine.GetBlocks().back()->SetMass(blockMass + 10.0f);
	physicsEngine.GetBlocks().back()->SetAccelerationGravity();
	physicsEngine.GetBlocks().back()->SetWidth(300);
	physicsEngine.GetBlocks().back()->SetHeight(50);

	const SDL_Color red = { 255, 0, 0, 255 };
	m_pPointLabel = new Label("Score: " + to_string(score), "Consolas", 20, red, glm::vec2(50, 30));
	AddChild(m_pPointLabel);

	m_pInstructionLabel = new Label("[C] Switch Weapon , [SPACE] Reset Object ", "Consolas", 20, red, glm::vec2(500, 30));
	AddChild(m_pInstructionLabel);

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

	//cout << physicsEngine.GetEnemies().back()->GetToughness().x << " / " << physicsEngine.GetEnemies().back()->GetToughness().y << endl;
	//cout << physicsEngine.GetProjectiles().back()->GetMomentum().x << " / " << physicsEngine.GetProjectiles().back()->GetMomentum().y << endl;
	//cout << physicsEngine.GetBlocks().back()->GetMomentum().x << " / " << physicsEngine.GetBlocks().back()->GetMomentum().y << endl
	//cout << physicsEngine.GetEnemies().back()->GetMomentum().x << " / " << physicsEngine.GetEnemies().back()->GetMomentum().y << endl;
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
		physicsEngine.AddObject(new Arrow(15.0f, angle, speed, gravity, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 15, GameObjectType::ARROW, MaterialType::STEEL));
		//slingshotPower = 20.0f;
	}
	else if (!physicsEngine.GetArrows().empty())
	{
		physicsEngine.RemoveArrow();
		physicsEngine.AddObject(new Projectile(10.0f, angle, speed, gravity, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 32, GameObjectType::PROJECTILE, MaterialType::STEEL));
		//slingshotPower = 10.0f;
	}

}

void PlayScene::ResetObject()
{
	while (!physicsEngine.GetProjectiles().empty())
	{
		physicsEngine.RemoveProjectile();
	}
	while (!physicsEngine.GetArrows().empty())
	{
		physicsEngine.RemoveArrow();
	}
	while(!physicsEngine.GetBlocks().empty())
	{
		physicsEngine.RemoveBlock();
	}
	while (!physicsEngine.GetEnemies().empty())
	{
		physicsEngine.RemoveEnemy();
	}
	// Projectile
	physicsEngine.AddObject(new Projectile(10.0f, 0, 0, 0, 0.8f, slingshotPoint.x + 16, slingshotPoint.y - 32, GameObjectType::PROJECTILE, MaterialType::STEEL));

	// Enemies
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 550, groundPoint.y - 36, GameObjectType::ENEMY, MaterialType::STEEL));
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 650, groundPoint.y - 36, GameObjectType::ENEMY, MaterialType::STEEL));
	physicsEngine.AddObject(new Enemy(10.0f, 0, 0, 9.8f, 0.5f, 600, 140, GameObjectType::ENEMY, MaterialType::STEEL));

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
	physicsEngine.GetBlocks().back()->SetMass(blockMass + 10.0f);
	physicsEngine.GetBlocks().back()->SetAccelerationGravity();
	physicsEngine.GetBlocks().back()->SetWidth(300);
	physicsEngine.GetBlocks().back()->SetHeight(50);

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

	if(ImGui::Button("Reset objects"))
	{
		ResetObject();
		/*std::cout << "My Button Pressed" << std::endl;*/
	}

	ImGui::Separator();
	ImGui::SliderFloat("Slingshot Power: ", &slingshotPower, 1, 20.0);

	if (!physicsEngine.GetProjectiles().empty())
	{
		ImGui::Separator();
		ImGui::Text("Projectile(Circle)");
		float mass1 = physicsEngine.GetProjectiles().back()->GetMass();
		ImGui::SliderFloat("MassP: ", &mass1, 0.1, 20.0);
		physicsEngine.GetProjectiles().back()->SetMass(mass1);
		physicsEngine.GetProjectiles().back()->SetAccelerationGravity();

		ImGui::Separator();
		ImGui::LabelText("VelocityP", "x:%f, y:%f", physicsEngine.GetProjectiles().back()->GetVelocity().x, physicsEngine.GetProjectiles().back()->GetVelocity().y);
		ImGui::LabelText("EnergyP", "x:%f, y:%f", physicsEngine.GetProjectiles().back()->GetMomentum().x * physicsEngine.GetProjectiles().back()->GetMaterialCOR(), physicsEngine.GetProjectiles().back()->GetMomentum().y * physicsEngine.GetProjectiles().back()->GetMaterialCOR());

		if (ImGui::Button("STEEL"))
		{
			physicsEngine.GetProjectiles().back()->SetMaterial(MaterialType::STEEL);
		}
		ImGui::SameLine();
		if (ImGui::Button("RUBBER"))
		{
			physicsEngine.GetProjectiles().back()->SetMaterial(MaterialType::RUBBER);
		}
		ImGui::SameLine();
		ImGui::LabelText("", "Material: %s, COR:%f", physicsEngine.GetProjectiles().back()->GetMaterial(), physicsEngine.GetProjectiles().back()->GetMaterialCOR());
	}
	ImGui::Separator();

	if (!physicsEngine.GetArrows().empty())
	{
		ImGui::Separator();
		ImGui::Text("Arrow(Rectangle)");
		float mass2 = physicsEngine.GetArrows().back()->GetMass();
		ImGui::SliderFloat("MassA: ", &mass2, 0.1, 20.0);
		physicsEngine.GetArrows().back()->SetMass(mass2);
		physicsEngine.GetArrows().back()->SetAccelerationGravity();

		ImGui::Separator();
		ImGui::LabelText("VelocityA", "x:%f, y:%f", physicsEngine.GetArrows().back()->GetVelocity().x, physicsEngine.GetArrows().back()->GetVelocity().y);
		ImGui::LabelText("EnergyA", "x:%f, y:%f", physicsEngine.GetArrows().back()->GetMomentum().x * physicsEngine.GetArrows().back()->GetMaterialCOR(), physicsEngine.GetArrows().back()->GetMomentum().y * physicsEngine.GetArrows().back()->GetMaterialCOR());
		if (ImGui::Button("STEEL"))
		{
			physicsEngine.GetArrows().back()->SetMaterial(MaterialType::STEEL);
		}
		ImGui::SameLine();
		if (ImGui::Button("RUBBER"))
		{
			physicsEngine.GetArrows().back()->SetMaterial(MaterialType::RUBBER);
		}
		ImGui::SameLine();
		ImGui::LabelText("", "Material: %s, COR:%f", physicsEngine.GetArrows().back()->GetMaterial(), physicsEngine.GetArrows().back()->GetMaterialCOR());

	}
	ImGui::Separator();
	ImGui::Separator();

	if (!physicsEngine.GetEnemies().empty())
	{
		ImGui::Separator();
		ImGui::Text("Enemies");
		for(int i = 0; i < physicsEngine.GetEnemies().size(); i++)
		{
			float mass3 = physicsEngine.GetEnemies().back()->GetMass();
			ImGui::SliderFloat("Mass1: ", &mass3, 0.1, 20.0);
			physicsEngine.GetEnemies().back()->SetMass(mass3);
			physicsEngine.GetEnemies().back()->SetAccelerationGravity();
			ImGui::LabelText("Energy1", "x:%f, y:%f", physicsEngine.GetEnemies().back()->GetMomentum().x * physicsEngine.GetEnemies().back()->GetMaterialCOR(), physicsEngine.GetEnemies().back()->GetMomentum().y * physicsEngine.GetEnemies().back()->GetMaterialCOR());
			ImGui::LabelText("Toughness1", "x:%f, y:%f", physicsEngine.GetEnemies().back()->GetToughness().x, physicsEngine.GetEnemies().back()->GetToughness().y);
			ImGui::LabelText("", "Material: %s, COR:%f", physicsEngine.GetEnemies().back()->GetMaterial(), physicsEngine.GetEnemies().back()->GetMaterialCOR());
		}
	}

	
	ImGui::End();
}

