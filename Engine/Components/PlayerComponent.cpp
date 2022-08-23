#include "PlayerComponent.h"
#include "Engine.h"
#include <iostream>
void neu::PlayerComponent::Update()
{
	Vector2 direction = Vector2::zero;
	//update transform with input
	if (g_inputSystem.GetKeyState(key_a) == InputSystem::State::Held){

		direction = Vector2::left;
	}

	if (g_inputSystem.GetKeyState(key_d) == InputSystem::State::Held){
		direction = Vector2::right;
	}

	float  thrust = 0;
	if (g_inputSystem.GetKeyState(key_w) == InputSystem::State::Held){

		thrust = speed;
	}
	
	auto component = m_owner->GetComponent<PhysicsComponent>();
	if (component){
		Vector2 force = Vector2::Rotate({ 1, 0 }, neu::DegToRad(m_owner->m_transform.rotation)) * thrust;
		component->ApplyForce(direction * speed);

	}

	m_owner->m_transform.position += direction * 300 * g_time.deltaTime;

	if (g_inputSystem.GetKeyState(key_space) == InputSystem::State::Press){

		auto component = m_owner->GetComponent<PhysicsComponent>();
		if (component) {
			Vector2 force = Vector2::Rotate({ 1, 0 }, neu::DegToRad(m_owner->m_transform.rotation)) * thrust;
			component->ApplyForce(Vector2::up * 30);

		}
	}
}

bool neu::PlayerComponent::Write(const rapidjson::Value& value) const{

	return true;
}

bool neu::PlayerComponent::Read(const rapidjson::Value& value){

	READ_DATA(value, speed);

	return true;
}