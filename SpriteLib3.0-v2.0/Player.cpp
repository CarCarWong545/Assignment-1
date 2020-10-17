#include "Player.h"
#include <iostream>

using namespace std;

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;

	if (hasPhys)
	{
		m_physBody = body;
	}

	//initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Load in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//IDLE ANIMATIONS\\

	//NEED TO ADD MORE ANIMATIONS TO crabRow.json

	//Idle Left
	m_animController->AddAnimation(animations["LStill"].get <Animation>());
	//Idle Right
	m_animController->AddAnimation(animations["RStill"].get<Animation>());
	
	//Idle Up
	m_animController->AddAnimation(animations["Fall"].get<Animation>());
	//Idle Down
	m_animController->AddAnimation(animations["Clamp"].get<Animation>());
	
	//WALK ANIMATIONS\\

	//Walk left
	m_animController->AddAnimation(animations["Move"].get<Animation>());
	//Walk Right
	m_animController->AddAnimation(animations["Move"].get<Animation>());
	
	//Walk Up
	m_animController->AddAnimation(animations["Jump"].get<Animation>());
	//Walk Down
	m_animController->AddAnimation(animations["Clamp"].get<Animation>());



	//Set Default Animation
	m_animController->SetActiveAnim(IDLELEFT);
}


void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();

	}
	
	AnimationUpdate();
}


float x = 1.f;

void Player::MovementUpdate()
{

	m_moving = false;
	float jumpPower = 4.0;
	
	

	if (m_hasPhysics)
	{
		float speed = 50.f;
		vec3 vel = vec3(0.f, 0.f,0.f);

		if (Input::GetKey(Key::Shift))
		{
			speed *= 7.f;
			cout << speed << endl;
		}
		
		if (Input::GetKey(Key::LeftControl))
		{
			if (speed < 100)
			{
				speed = pow(50, x);
				x += 0.01;
			}
			
			cout << speed << endl;

			/*while (x > 0 && x < 3 && speed <= 100)
			{
				
			}*/

		}
		else
		{
			x = 1.f;
		}

		if (Input::GetKey(Key::W))
		{
				vel = vel + vec3(0.f, 1.f * (jumpPower), 0.f);
				m_facing = UP;
				m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			vel = vel + vec3(0.f, 0.f,0.f); // No movement as you can't walk down in air
			m_facing = DOWN;
			m_moving = true; //True arms go up, false still plays
		}

		if (Input::GetKey(Key::A))
		{
			vel = vel + vec3(-1.f, 0.f,0.f);
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			vel = vel + vec3(1.f, 0.f,0.f);
			m_facing = RIGHT;
			m_moving = true;
		}

		m_physBody->SetVelocity(vel * speed);
	}
	else
	{
		//Regular Movement
		float speed = 50.f;

		if (Input::GetKey(Key::Shift))
		{
			speed *= 7.f;
		}


		if (Input::GetKey(Key::W))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() + (speed * Timer::deltaTime));
			m_facing = UP;
			m_moving = true;
		}

		if (Input::GetKey(Key::S)) // No movement as you can't walk down in air
		{
			m_transform->SetPositionY(m_transform->GetPositionY() - (speed * Timer::deltaTime));
			m_facing = DOWN;
			m_moving = true; //True arms go up, false still plays
		}

		if (Input::GetKey(Key::A))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() - (speed * Timer::deltaTime));
			m_facing = LEFT;
			m_moving = true;
		}

		if (Input::GetKey(Key::D))
		{
			m_transform->SetPositionX(m_transform -> GetPositionX() + (speed * Timer::deltaTime));
			m_facing = RIGHT;
			m_moving = true;
		}
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//Puts it into the WALK category
		activeAnimation = WALK;
	}

	else
	{
		activeAnimation = IDLE;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
	
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
