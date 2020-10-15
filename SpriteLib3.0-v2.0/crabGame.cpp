/*
//////////Crab Game: a simple platformer//////////

Objective: Get to your kin

W - Jump(fly)
A - Move left
S - Squat (No functionality)
D - Move Right
Shift - Run

T - Clear terminal


Caroyln Wong() - Scene, player movement, background sprite, foreground sprite, 3/4 bounding box 
Jaden Hepburn() - Win objective, player sprites, sprite animations, 1/4 bounding box  
*/

#include "crabGame.h"
#include "Utilities.h"
#include <iostream> 


using namespace std; // Tired of doing :: for everthing printing related

crabGame::crabGame(std::string name)
	:Scene(name)
{
	//no gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -2000.f); // Default is 0.f,-1000.f
	m_physicsWorld->SetGravity(m_gravity);
}

void crabGame::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = (windowWidth / windowHeight);

	//Setup main camera entity
	{
		//Creates camera
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		float camSize = 75.f; //Changes "Zoom" of camera, 75 is default. [N]

		vec4 temp = vec4(-camSize, camSize, -camSize, camSize);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Set up win objective crabs
	{
		//Kin 1
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "spritesheets/None_128.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
			//	ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(120.f, 8.f, 10.f));
		}
		//Kin 2
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "spritesheets/None_128.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
			//	ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(135.f, 8.f, 10.f));
		}
		//Kin 3
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "spritesheets/None_128.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
			//	ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(150.f, 8.f, 10.f));
		}
		//Kin 4
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "spritesheets/None_128.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
			//	ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(165.f, 8.f, 10.f));
		}
		//Kin 5
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "spritesheets/None_128.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
			//	ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(180.f, 8.f, 10.f));
		}
	}

	// Set up dynamic box
	/* 
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components

		int boxSize = 40;//Change for box size [N]
		std::string fileName = "box.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, boxSize, boxSize); //String filelocation, x, y
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-90.f), float32(20.f)); // Changes box position default is 0, 20 [N]

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
	}*/

	/////////////////////////Level Creation/////////////////////////
	{
		//Set up background
		{
			//Create entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up component
			float backgroundScale = 1; // Scales background, default 1 [N]
			std::string fileName = "Background.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 426 * backgroundScale, 240 * backgroundScale);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 40.f, 0.f));
		}

		//Set up barrier on left
		{
			auto entity = ECS::CreateEntity();

			//Add compnents
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Set up components
			std::string fileName = "floor.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 426);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-175.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}

		//Set up barrier on right
		{
			auto entity = ECS::CreateEntity();

			//Add compnents
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Set up components
			std::string fileName = "floor.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 426);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(200.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}

		//Set up static box for floor
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "floor.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 840, 20);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, -10.f, 2.f));



			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 5.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-50.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}

		//Set up static box for roof
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "roof.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 840, 100);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 5.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-50.f), float32(210.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}
	}
	/////////////////////////////////////////////////////////////////

	


	//Set up player crab
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Set up components
		std::string fileName = "spritesheets/128Crab.png";
		std::string animations = "Crab128.json"; 
		float plrSize = 40; //40 default, 32 & 64 nice numbers

		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, plrSize, plrSize, &ECS::GetComponent<Sprite>(entity), &ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity), true, &ECS::GetComponent<PhysicsBody>(entity));

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-100.f), float32(20.f)); //SETS PLAYER POSITION on initialisation

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetFixedRotation(true);

	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent <VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

}

bool winCondition = false;

void crabGame::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto& plr = ECS::GetComponent<Transform>(MainEntities::MainPlayer());

	Scene::AdjustScrollOffset();
	player.Update();

	//WIN CONDITION
	if (plr.GetPositionX() >= 100 && plr.GetPositionY() <= 35)
	{
		cout << "WIN CONDITION" << endl;	
	}

}


void crabGame::KeyboardHold()
{
	auto& plr = ECS::GetComponent<Transform>(MainEntities::MainPlayer());

	if (Input::GetKey(Key::E))
	{
		
		cout << "("<<plr.GetPositionX()<<","<<plr.GetPositionY()<<")"<<endl;
	}

	if (Input::GetKey(Key::T))
	{

		system("cls");
	}
}

void crabGame::KeyboardDown()
{
	




}

void crabGame::KeyboardUp()
{

}
