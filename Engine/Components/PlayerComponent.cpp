#include "PlayerComponent.h"
#include "Engine.h"
#include <iostream>

void neu::PlayerComponent::Initialize(){
	auto component = m_owner->GetComponent<CollisionComponent>();

	if (component) {
		component->SetCollisionEnter(std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1));
		component->SetCollisionExit(std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1));
	}
}

void neu::PlayerComponent::Update(){
	Vector2 direction = Vector2::zero;
	
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
			component->ApplyForce(Vector2::up * 200);

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

void neu::PlayerComponent::OnCollisionEnter(Actor* other){

	if (other->GetName() == "Coin") {

		Event event;
		event.name = "EVENT_ADD_POINTS";
		event.data = 100;

		g_eventManager.Notify(event);

		other->SetDestroy();
	}

	std::cout << "Player Enter\n";

}

void neu::PlayerComponent::OnCollisionExit(Actor* other){

	std::cout << "Player Exit\n";

}
