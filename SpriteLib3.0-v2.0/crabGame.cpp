/*
//////////Crab Game: a simple platformer//////////

Objective: Get to your kin

W - Jump(fly)
A - Move left
S - Squat (No functionality)
D - Move Right
Shift - Run

L - Clear terminal
P - Toggles boxes (Prebuilt???)
O - Shows time & position (Terminal) 


////TODO LIST////
- Speed graph 2
- Gui toggle for both speed graphs (Were we taught guis???)
[optionals]
-Moving platforms
-Moving enemy
[Finalize]
-Video explanation
-Create .exe
-Provide files & github/gitkraken repo

Caroyln Wong(100781520) - Player movement, background & foreground sprites, bounding box, scene,
Jaden Hepburn(100791169) - Win objective, player sprites & animations, cleaning/formatting, Timer & Position show,    
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


	/////////////////////////Visual information/////////////////////////
	//Setup main camera entity
	{
		//Creates camera
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);
		float m_offset = 0.5f;

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

	//User interface attempt (Failed, i'm completely clueless how this works)
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<UI>(entity);
		ECS::GetComponent<UI>(entity).InitImGUI();
		ECS::GetComponent<UI>(entity).Start(windowWidth,windowHeight);
		ECS::GetComponent<UI>(entity).m_isInit = true;
		ECS::GetComponent<UI>(entity).End();
	}

	/////////////////////////Objects Implementation/////////////////////////
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

		//Set up start instructions
		{
			
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "start.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 20);
			//ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-400.f, 30.f, 10.f));
		}

		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "won.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 20);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(150.f, 30.f, 10.f));
		}
	}

	

	//Box
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
			float backgroundScale = 1.30; // Scales background, default 1 [N]
			std::string fileName = "Background2.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 700 * backgroundScale, 400 * backgroundScale);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-150.f, 40.f, 0.f));
		}

		//Set up barrier on left
		{
			auto entity = ECS::CreateEntity();

			//Add compnents
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Set up components
			std::string fileName = "barrier.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 426);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-500.f), float32(-10.f));

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
			std::string fileName = "barrier.png";
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
			std::string fileName = "barrier.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1250, 20);
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
			std::string fileName = "barrier.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 840, 100);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 5.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-50.f), float32(290.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}

		//Barrier
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			std::string fileName = "planktall.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 100);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(90.f), float32(50));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}

		//Platform 1
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			std::string fileName = "plankwide.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-110.f), float32(20));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}


		//Platform 2
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			std::string fileName = "plankwide.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(-50.f), float32(50));

			tempBody = m_physicsWorld->CreateBody(&tempDef);
			
			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}

		//Platform 3
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			std::string fileName = "plankwide.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, -10.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(25.f), float32(80));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		}
	}
	

	/////////////////////////Player Implementation/////////////////////////
	{
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
			tempDef.position.Set(float32(-450.f), float32(20.f)); //SETS PLAYER POSITION on initialisation

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

			ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetFixedRotation(true);

		}
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent <VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

}

bool winCondition = false;

void crabGame::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto& plr = ECS::GetComponent<Transform>(MainEntities::MainPlayer());

	//Scene::AdjustScrollOffset();
	player.Update();

	//WIN CONDITION area (need a better way to display)
	if (plr.GetPositionX() >= 100 && plr.GetPositionY() <= 35 && plr.GetPositionX() <= 200 && plr.GetPositionY() >= -35)
	{
		cout << "WIN CONDITION REACHED" << endl;
	}

}


void crabGame::KeyboardHold()
{
	auto& plr = ECS::GetComponent<Transform>(MainEntities::MainPlayer());

	/*if (Input::GetKey(Key::P)) // Shows position
	{
		
		cout << "("<<plr.GetPositionX()<<","<<plr.GetPositionY()<<")"<<endl;
	}*/

	if (Input::GetKey(Key::L)) // Clears Terminal
	{

		system("cls");
	}

	if (Input::GetKey(Key::O)) //Displays timer and position
	{
		cout << "Time: " << Timer::time << "\n"
			<<"Position: " << "(" << plr.GetPositionX() << "," << plr.GetPositionY() << ")" 
			<<"\n//////////////////////////////" <<endl;
	}

}

void crabGame::KeyboardDown()
{
	
}

void crabGame::KeyboardUp()
{

}
