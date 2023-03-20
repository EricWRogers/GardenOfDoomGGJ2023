#include "App.hpp"
#include <Canis/ECS/Systems/RenderHUDSystem.hpp>
#include <Canis/ECS/Systems/RenderTextSystem.hpp>
#include <Canis/ECS/Systems/SpriteRenderer2DSystem.hpp>
#include <Canis/ECS/Systems/SpriteAnimationSystem.hpp>

#include <Canis/ECS/Systems/CollisionSystem2D.hpp>
#include <Canis/ECS/Systems/ButtonSystem.hpp>
#include <Canis/ECS/Systems/UISliderSystem.hpp>

#include <Canis/ECS/Decode.hpp>

#include "ECS/Systems/BoidSystem.hpp"
#include "ECS/Systems/EnemySystem.hpp"
#include "ECS/Systems/BulletSystem.hpp"
#include "ECS/Systems/BombSystem.hpp"
#include "ECS/Systems/HandOfGodSystem.hpp"
#include "ECS/Systems/DamageTextSystem.hpp"

#include "ECS/ScriptableEntities/DebugCamera2D.hpp"
#include "ECS/ScriptableEntities/BeachBall.hpp"
#include "ECS/ScriptableEntities/EnemySpawnManager.hpp"
#include "ECS/ScriptableEntities/FPSCounter.hpp"
#include "ECS/ScriptableEntities/PlayerManager.hpp"
#include "ECS/ScriptableEntities/Timer.hpp"
#include "ECS/ScriptableEntities/MainMenuButtons.hpp"
#include "ECS/ScriptableEntities/MainMenuButton.hpp"
#include "ECS/ScriptableEntities/MapBuilder.hpp"
#include "ECS/ScriptableEntities/Weapon.hpp"
#include "ECS/ScriptableEntities/PeaShooterWeapon.hpp"
#include "ECS/ScriptableEntities/WaveManager.hpp"
#include "ECS/ScriptableEntities/XP.hpp"
#include "ECS/ScriptableEntities/MusicScriptableEntity.hpp"
#include "ECS/ScriptableEntities/FireBallWeapon.hpp"
#include "ECS/ScriptableEntities/SwordWeapon.hpp"
#include "ECS/ScriptableEntities/BombWeapon.hpp"
#include "ECS/ScriptableEntities/HandOfGodWeapon.hpp"
#include "ECS/ScriptableEntities/SplashLoader.hpp"
#include "ECS/ScriptableEntities/SplashLoader2.hpp"
#include "ECS/ScriptableEntities/GasAuraWeapon.hpp"
#include "ECS/ScriptableEntities/OrbitingSpikesWeapon.hpp"
#include "ECS/ScriptableEntities/SeedPickup.hpp"
#include "ECS/ScriptableEntities/UI/HUD/HUDStateMachine.hpp"
#include "ECS/Decode.hpp"

#include "ECS/ScriptableEntities/UI/HUD/HUDStateMachine.hpp"

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

		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "Canis::SpriteAnimationSystem"){
					scene->CreateSystem<Canis::SpriteAnimationSystem>();
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "Canis::UISliderSystem"){
					scene->CreateSystem<Canis::UISliderSystem>();
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "BoidSystem"){
					scene->CreateSystem<BoidSystem>();
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "BulletSystem"){
					scene->CreateSystem<BulletSystem>();
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "EnemySystem"){
					scene->CreateSystem<EnemySystem>();
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "BombSystem"){
					scene->CreateSystem<BombSystem>();
					return true;
				}
				return false;
			}
		);
		
		sceneManager.decodeSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "HandOfGodSystem"){
					scene->CreateSystem<HandOfGodSystem>();
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

		sceneManager.decodeRenderSystem.push_back(
			[](YAML::Node _n, int _index, Canis::Scene *scene) {
				if(_n[_index].as<std::string>() == "DamageTextSystem"){
					scene->CreateRenderSystem<DamageTextSystem>();
					return true;
				}
				return false;
			}
		);
	}
	
	{ // decode scriptable entities
		sceneManager.decodeScriptableEntity.push_back(DecodeDebugCamera2D);
		sceneManager.decodeScriptableEntity.push_back(DecodeBeachBall);
		sceneManager.decodeScriptableEntity.push_back(DecodeEnemySpawnManager);
		sceneManager.decodeScriptableEntity.push_back(DecodePlayerManager);
		sceneManager.decodeScriptableEntity.push_back(DecodeFPSCounter);
		sceneManager.decodeScriptableEntity.push_back(DecodeTimer);
		sceneManager.decodeScriptableEntity.push_back(DecodeMainMenuButtons);
		sceneManager.decodeScriptableEntity.push_back(DecodeMainMenuButton);
		sceneManager.decodeScriptableEntity.push_back(DecodeMapBuilder);		
		sceneManager.decodeScriptableEntity.push_back(DecodePeaShooterWeapon);
		sceneManager.decodeScriptableEntity.push_back(DecodeWeapon);
		sceneManager.decodeScriptableEntity.push_back(DecodeWaveManager);
		sceneManager.decodeScriptableEntity.push_back(DecodeXP);
		sceneManager.decodeScriptableEntity.push_back(DecodeMusicScriptableEntity);
		sceneManager.decodeScriptableEntity.push_back(DecodeFireBallWeapon);
		sceneManager.decodeScriptableEntity.push_back(DecodeSwordWeapon);
		sceneManager.decodeScriptableEntity.push_back(DecodeBombWeapon);
		sceneManager.decodeScriptableEntity.push_back(DecodeHandOfGodWeapon);
		sceneManager.decodeScriptableEntity.push_back(DecodeGasAuraWeapon);
		sceneManager.decodeScriptableEntity.push_back(DecodeOrbitingSpikesWeapon);
		sceneManager.decodeScriptableEntity.push_back(DecodeSplashLoader);
		sceneManager.decodeScriptableEntity.push_back(DecodeSplashLoader2);
		sceneManager.decodeScriptableEntity.push_back(DecodeSeedPickup);
		sceneManager.decodeScriptableEntity.push_back(DecodeHUDStateMachine);
	}

	{ // decode component
		sceneManager.decodeEntity.push_back(Canis::DecodeTagComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeCamera2DComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeRectTransformComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeColorComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeTextComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeSprite2DComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeUIImageComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeUISliderComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeSpriteAnimationComponent);
		sceneManager.decodeEntity.push_back(Canis::DecodeCircleColliderComponent);
		sceneManager.decodeEntity.push_back(DecodePlayerHealthComponent);
		sceneManager.decodeEntity.push_back(DecodeEnemyHealthComponent);
		sceneManager.decodeEntity.push_back(DecodeBombComponent);
	}
}

App::~App() {}

void App::Run()
{
	if (appState == AppState::ON)
		Canis::FatalError("App already running.");

	Canis::Init();

	unsigned int windowFlags = 0;

	window.Create("Canis", 1280, 800, windowFlags);

	time.init(10000);

	camera.override_camera = false;


	seed = std::time(NULL);
	srand(seed);
	Canis::Log("seed : " + std::to_string(seed));

	sceneManager.Add(new Canis::Scene("SpriteDemo", "assets/scenes/SpriteDemo.scene"));
	sceneManager.Add(new Canis::Scene("main_menu", "assets/scenes/main_menu.scene"));
	sceneManager.Add(new Canis::Scene("main", "assets/scenes/main.scene"));
	sceneManager.Add(new Canis::Scene("lose", "assets/scenes/lose.scene"));
	sceneManager.Add(new Canis::Scene("win", "assets/scenes/win.scene"));
	sceneManager.Add(new Canis::Scene("engine_splash", "assets/scenes/engine_splash.scene"));
	sceneManager.Add(new Canis::Scene("ggj_splash", "assets/scenes/ggj_splash.scene"));

	sceneManager.PreLoad(
		&window,
		&inputManager,
		&time,
		&camera,
		&assetManager,
		seed
	);

	Load();

	appState = AppState::ON;

	Loop();
}
void App::Load()
{
	sceneManager.ForceLoad("main");

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
	if(!inputManager.Update(window.GetScreenWidth(), window.GetScreenHeight())) {
		appState = AppState::OFF;
	}

	

	sceneManager.InputUpdate();
}
