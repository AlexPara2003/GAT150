#include "PlayerComponent.h"
#include "Engine.h"
#include <iostream>

void neu::PlayerComponent::Initialize(){
	
	CharacterComponent::Initialize();

	
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

	Vector2 velocity;
	auto component = m_owner->GetComponent<PhysicsComponent>();
	if (component){
		
		component->ApplyForce(direction * speed);
		velocity = component->velocity;

	}

	m_owner->m_transform.position += direction * 300 * g_time.deltaTime;

	if (g_inputSystem.GetKeyState(key_space) == InputSystem::State::Press){

		auto component = m_owner->GetComponent<PhysicsComponent>();
		if (component) {
			Vector2 force = Vector2::Rotate({ 1, 0 }, neu::DegToRad(m_owner->m_transform.rotation)) * thrust;
			component->ApplyForce(Vector2::up * 200);

		}
	}

	auto renderComponent = m_owner->GetComponent<RenderComponent>();
	if (renderComponent){
		if (velocity.x != 0) renderComponent->SetFlipHorizontal(velocity.x < 0);
	}
}

bool neu::PlayerComponent::Write(const rapidjson::Value& value) const{

	return true;
}

bool neu::PlayerComponent::Read(const rapidjson::Value& value){

	CharacterComponent::Read(value);
	READ_DATA(value, jump);

	return true;
}

void neu::PlayerComponent::OnNotify(const Event& event){

	if (event.name == "EVENT_DAMAGE") {
		health -= std::get<float>(event.data);
		std::cout << health << std::endl;
		if (health <= 0) {
			//Player dead
		}
	}
}

void neu::PlayerComponent::OnCollisionEnter(Actor* other){

	if (other->GetName() == "Coin") {

		Event event;
		event.name = "EVENT_ADD_POINTS";
		event.data = 100;

		g_eventManager.Notify(event);

		other->SetDestroy();
	}

	if (other->GetTag() == "Enemy") {

		Event event;
		event.name = "EVENT_DAMAGE";
		event.data = damage;
		event.receiver = other;

		g_eventManager.Notify(event);

		other->SetDestroy();

	}
}

void neu::PlayerComponent::OnCollisionExit(Actor* other){

}
