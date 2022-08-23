#include "Engine.h"
#include <iostream>

int main(){

	neu::InitializeMemory();
	neu::SetFilePath("../Assets");

	neu::g_inputSystem.Initialize();
	neu::g_renderer.Initialize();
	neu::g_audioSystem.Initialize();
	neu::g_resources.Initialize();
	neu::g_physicsSystem.Initialize();

	neu::Engine::Instance().Register();

	neu::g_renderer.CreateWindow("Baked Beans", 800, 600);
	neu::g_renderer.SetClearColor(neu::Color{ 10, 10, 10, 255 });

	neu::Scene scene;
	
	rapidjson::Document document;
	bool success = neu::json::Load("level.txt", document);

	scene.Read(document);
	scene.Initialize();

	float angle = 0;

	bool quit = false;
	while (!quit){
		//update (engine)
		neu::g_time.Tick();
		neu::g_inputSystem.Update();
		neu::g_audioSystem.Update();
		neu::g_physicsSystem.Update();

		if (neu::g_inputSystem.GetKeyDown(neu::key_escape)) quit = true;

		//update game objects
		scene.Update();

		//render
		neu::g_renderer.BeginFrame();

		angle += 360.0f * neu::g_time.deltaTime;

		scene.Draw(neu::g_renderer);

		neu::g_renderer.EndFrame();
	}

	neu::g_physicsSystem.Shutdown();
	neu::g_resources.Shutdown();
	neu::g_inputSystem.Shutdown();
	neu::g_renderer.Shutdown();
	neu::g_audioSystem.Shutdown();
}