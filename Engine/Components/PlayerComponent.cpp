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
		float multiplier = (m_groundCount > 0) ? 1 : 0.2f;

		component->ApplyForce(direction * speed * multiplier);
		velocity = component->velocity;
	}

	m_owner->m_transform.position += direction * 300 * g_time.deltaTime;

	if (m_groundCount > 0 && g_inputSystem.GetKeyState(key_space) == InputSystem::State::Press){

		auto component = m_owner->GetComponent<PhysicsComponent>();
		if (component) {
			Vector2 force = Vector2::Rotate({ 1, 0 }, neu::DegToRad(m_owner->m_transform.rotation)) * thrust;
			component->ApplyForce(Vector2::up * jump);
		}
	}

	auto animComponent = m_owner->GetComponent<SpriteAnimComponent>();
	if (animComponent){
		if (velocity.x != 0) animComponent->SetFlipHorizontal(velocity.x < 0);
		if (std::fabs(velocity.x) > 0){
			animComponent->SetSequence("run");
		} else {
			animComponent->SetSequence("idle");
		}
	}

	auto camera = m_owner->GetScene()->GetActorFromName("Camera");
	if (camera){
		camera->m_transform.position = neu::Lerp(camera -> m_transform.position, m_owner->m_transform.position, 2 * g_time.deltaTime);
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
			m_owner->SetDestroy();

			Event event;
			event.name = "EVENT_PLAYER_DEAD";

			g_eventManager.Notify(event);
		}
	}
}

void neu::PlayerComponent::OnCollisionEnter(Actor* other){

	if (other->GetName() == "Coin") {

		Event event;
		event.name = "EVENT_ADD_POINTS";
		event.data = 100;

		health += 1;

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

	if (other->GetTag() == "Ground") {
		m_groundCount++;
	}
}

void neu::PlayerComponent::OnCollisionExit(Actor* other){
	if (other->GetTag() == "Ground"){
		m_groundCount--;
	}
}
