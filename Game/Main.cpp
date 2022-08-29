#include "Engine.h"
#include "AlexGame.h"
#include <iostream>

int main(){

	neu::InitializeMemory();
	neu::SetFilePath("../Assets");

	neu::g_inputSystem.Initialize();
	neu::g_renderer.Initialize();
	neu::g_audioSystem.Initialize();
	neu::g_resources.Initialize();
	neu::g_physicsSystem.Initialize();
	neu::g_eventManager.Initialize();

	neu::Engine::Instance().Register();

	neu::g_renderer.CreateWindow("Baked Beans", 800, 600);
	neu::g_renderer.SetClearColor(neu::Color{ 10, 10, 10, 255 });

	std::unique_ptr<AlexGame> game = std::make_unique<AlexGame>();
	
	game->Initialize();

	float angle = 0;

	bool quit = false;
	while (!quit){
		
		neu::g_time.Tick();
		neu::g_inputSystem.Update();
		neu::g_audioSystem.Update();
		neu::g_physicsSystem.Update();
		neu::g_eventManager.Update();

		if (neu::g_inputSystem.GetKeyDown(neu::key_escape)) quit = true;

		game->Update();

		neu::g_renderer.BeginFrame();

		angle += 360.0f * neu::g_time.deltaTime;

		game->Draw(neu::g_renderer);

		neu::g_renderer.EndFrame();
	}

	game->Shutdown();
	game.reset();

	neu::Factory::Instance().Shutdown();

	neu::g_eventManager.Shutdown();
	neu::g_physicsSystem.Shutdown();
	neu::g_resources.Shutdown();
	neu::g_inputSystem.Shutdown();
	neu::g_renderer.Shutdown();
	neu::g_audioSystem.Shutdown();
}