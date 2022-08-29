#pragma once

#include "Framework/Game.h"
#include "Framework/Event.h"

class AlexGame : public neu::Game {

public:
	enum class gameState {

		titleScreen,
		startLevel,
		game, 
		playerDead, 
		gameOver


	};

	virtual void Initialize() override;
	virtual void Shutdown() override;
	virtual void Update() override;
	virtual void Draw(neu::Renderer& renderer) override;

	void OnAddPoints(const neu::Event& event);
	void OnPlayerDead(const neu::Event& event);

	gameState m_gameState = gameState::titleScreen;
	float m_startTimer = 0;
	int m_lives = 3;

};