#include "App.hpp"
#include <Canis/ECS/Systems/RenderHUDSystem.hpp>
#include <Canis/ECS/Systems/RenderTextSystem.hpp>
#include <Canis/ECS/Systems/SpriteRenderer2DSystem.hpp>

#include <Canis/ECS/Systems/CollisionSystem2D.hpp>
#include <Canis/ECS/Systems/ButtonSystem.hpp>

#include "ECS/ScriptableEntities/DebugCamera2D.hpp"
#include "ECS/ScriptableEntities/BeachBall.hpp"

App::App()
{
	{ // decode systems
		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "Canis::ButtonSystem"){
					scene->CreateSystem<Canis::ButtonSystem>();
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "Canis::CollisionSystem2D"){
					scene->CreateSystem<Canis::CollisionSystem2D>();
					return true;
				}
				return false;
			}
		);
	}

	{ // decode render systems
		sceneManager.decodeRenderSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "Canis::RenderHUDSystem"){
					scene->CreateRenderSystem<Canis::RenderHUDSystem>();
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeRenderSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "Canis::RenderTextSystem"){
					scene->CreateRenderSystem<Canis::RenderTextSystem>();
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeRenderSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "Canis::SpriteRenderer2DSystem"){
					scene->CreateRenderSystem<Canis::SpriteRenderer2DSystem>();
					return true;
				}
				return false;
			}
		);
	}
	
	{ // decode scriptable entities
		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "DebugCamera2D"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<DebugCamera2D>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "BeachBall"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<BeachBall>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);
	}

	{ // decode component

	}
}
App::~App()
{
	
}

void App::Run()
{
	if (appState == AppState::ON)
		Canis::FatalError("App already running.");

	Canis::Init();

	unsigned int windowFlags = 0;

	window.Create("Canis", 1280, 720, windowFlags);

	time.init(30);

	camera.override_camera = false;


	seed = std::time(NULL);
	srand(seed);
	Canis::Log("seed : " + std::to_string(seed));

	sceneManager.Add(new Canis::Scene("SpriteDemo", "assets/scenes/SpriteDemo.scene"));

	sceneManager.PreLoad(
		&window,
		&inputManager,
		&time,
		&camera,
		&assetManager,
		seed
	);

	Canis::Log("Q App 0");

	Load();

	appState = AppState::ON;

	Loop();
}
void App::Load()
{
	sceneManager.ForceLoad("SpriteDemo");

	// start timer
	previousTime = high_resolution_clock::now();
}
void App::Loop()
{
	while (appState == AppState::ON)
	{
		deltaTime = time.startFrame();
		sceneManager.SetDeltaTime(deltaTime);

		Update();
		Draw();
		// Get SDL to swap our buffer
		window.SwapBuffer();
		LateUpdate();
		InputUpdate();

		window.fps = time.endFrame(); 
	}
}
void App::Update()
{
	sceneManager.Update();
}
void App::Draw()
{
	sceneManager.Draw();
}
void App::LateUpdate()
{
	sceneManager.LateUpdate();
}
void App::InputUpdate()
{
	inputManager.SwapMaps();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			appState = AppState::OFF;
			break;
		case SDL_MOUSEMOTION:
				inputManager.mouse.x = event.motion.x;
				inputManager.mouse.y = event.motion.y;
				camera.ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		case SDL_KEYUP:
			inputManager.ReleasedKey(event.key.keysym.sym);
			//Canis::Log("UP" + std::to_string(event.key.keysym.sym));
			break;
		case SDL_KEYDOWN:
			inputManager.PressKey(event.key.keysym.sym);
			//Canis::Log("DOWN");
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
				inputManager.leftClick = true;
			if(event.button.button == SDL_BUTTON_RIGHT)
				inputManager.rightClick = true;
			break;
		}
	}

	sceneManager.InputUpdate();
}