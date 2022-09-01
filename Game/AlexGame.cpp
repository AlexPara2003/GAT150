#include "AlexGame.h"
#include "GameComponents/EnemyComponent.h"
#include "Engine.h"

void AlexGame::Initialize() {

	REGISTER_CLASS(EnemyComponent);

	m_scene = std::make_unique<neu::Scene>();
	rapidjson::Document document;

	std::vector<std::string> sceneNames = { "scenes/prefabs.txt", "scenes/tilemap.txt", "scenes/level.txt" };
	for (auto sceneName : sceneNames) {
		bool success = neu::json::Load(sceneName, document);
		if (!success) {
			LOG("Could not load scene %s", sceneName.c_str());
			continue;
		}
		m_scene->Read(document);
	}
	m_scene->Initialize();

	neu::g_eventManager.Subscribe("EVENT_ADD_POINTS", std::bind(&AlexGame::OnNotify, this, std::placeholders::_1));
	neu::g_eventManager.Subscribe("EVENT_PLAYER_DEAD", std::bind(&AlexGame::OnNotify, this, std::placeholders::_1));
}

void AlexGame::Shutdown() {

	m_scene->RemoveAll();

}

void AlexGame::Update() {

	switch (m_gameState){
	case gameState::titleScreen:
		m_scene->GetActorFromName("End")->SetActive(false);
		m_scene->GetActorFromName("Score")->SetActive(false);
		if (neu::g_inputSystem.GetKeyState(neu::key_space) == neu::InputSystem::State::Press) {
			m_scene->GetActorFromName("Title")->SetActive(false);
			m_scene->GetActorFromName("Instruction")->SetActive(false);
			m_scene->GetActorFromName("Begin")->SetActive(false);
			m_gameState = gameState::startLevel;
		}

		break;

	case gameState::startLevel:

	{
		auto actor = neu::Factory::Instance().Create<neu::Actor>("Player");
		actor->m_transform.position = { 400,200 };
		actor->Initialize();
		m_scene->Add(std::move(actor));
	}

	m_gameState = gameState::game;
	break;

	case gameState::game:

		m_enemySpawnTimer -= neu::g_time.deltaTime;
		if (m_enemySpawnTimer <= 0){
			auto actor = neu::Factory::Instance().Create<neu::Actor>("Ghost");
			actor->m_transform.position = { neu::randomf(0, 800), 100.0f };
			actor->Initialize();
			m_scene->Add(std::move(actor));

			m_enemySpawnTimer = 300;
		}

		m_coinSpawnTimer -= neu::g_time.deltaTime;
		if (m_coinSpawnTimer <= 0){
			auto actor = neu::Factory::Instance().Create<neu::Actor>("Coin");
			actor->m_transform.position = { neu::randomf(0, 800), 100.0f };
			actor->Initialize();
			m_scene->Add(std::move(actor));

			m_coinSpawnTimer = 400;
		}

	break;

	case gameState::gameOver:
	{
		
		m_scene->GetActorFromName("End")->SetActive(true);


	}

	break;

	case gameState::playerDead:
		m_startTimer -= neu::g_time.deltaTime;
		if (m_startTimer <= 0) {
			m_gameState = (m_lives > 0) ? gameState::startLevel : gameState::gameOver;
		}
		break;
	}

	m_scene->Update();
}

void AlexGame::Draw(neu::Renderer& renderer) {

	m_scene->Draw(renderer);

}

void AlexGame::OnAddPoints(const neu::Event& event) {

	AddPoints(std::get<int>(event.data));

	std::cout << event.name << std::endl;
	std::cout << GetScore() << std::endl;

}

void AlexGame::OnPlayerDead(const neu::Event& event) {

	m_gameState = gameState::playerDead;
	m_lives--;
	m_startTimer = 3;

}

void AlexGame::OnNotify(const neu::Event& event) {

	if (event.name == "EVENT_ENEMY_DEFEAT") {
		AddPoints(std::get<int>(event.data));
	}

	if (event.name == "EVENT_PLAYER_DEAD") {
		m_gameState = gameState::playerDead;
		m_lives--;
		m_startTimer = 3;
	}

	if (event.name == "EVENT_ADD_POINTS") {

		m_scene->GetActorFromName("Score")->SetActive(true);

		AddPoints(std::get<int>(event.data));
		
		auto score = m_scene->GetActorFromName("Score");
		auto component = score->GetComponent<neu::TextComponent>();
		if (component){
			component->SetText(std::to_string((int)(GetScore())));
		}
		std::cout << GetScore() << std::endl;

	}
}