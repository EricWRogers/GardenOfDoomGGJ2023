#include "App.hpp"

#include <Canis/ECS/Components/ButtonComponent.hpp>

#include <Canis/ECS/Decode.hpp>

#include "ECS/Systems/BoidSystem.hpp"
#include "ECS/Systems/EnemySystem.hpp"
#include "ECS/Systems/BulletSystem.hpp"
#include "ECS/Systems/BombSystem.hpp"
#include "ECS/Systems/HandOfGodSystem.hpp"
#include "ECS/Systems/DamageTextSystem.hpp"
#include "ECS/Systems/PlantableTileSystem.hpp"

#include "ECS/ScriptableEntities/DebugCamera2D.hpp"
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

App::App()
{
	{ // decode systems
		sceneManager.decodeSystem.push_back(Canis::DecodeButtonSystem);
		sceneManager.decodeSystem.push_back(Canis::DecodeCollisionSystem2D);
		sceneManager.decodeSystem.push_back(Canis::DecodeSpriteAnimationSystem);
		sceneManager.decodeSystem.push_back(Canis::DecodeUISliderSystem);
		sceneManager.decodeSystem.push_back(DecodeBoidSystem);
		sceneManager.decodeSystem.push_back(DecodeBulletSystem);
		sceneManager.decodeSystem.push_back(DecodeEnemySystem);
		sceneManager.decodeSystem.push_back(DecodeBombSystem);		
		sceneManager.decodeSystem.push_back(DecodeHandOfGodSystem);
		sceneManager.decodeSystem.push_back(DecodePlantableTileSystem);
	}

	{ // decode render systems
		sceneManager.decodeRenderSystem.push_back(Canis::DecodeRenderHUDSystem);
		sceneManager.decodeRenderSystem.push_back(Canis::DecodeRenderTextSystem);
		sceneManager.decodeRenderSystem.push_back(Canis::DecodeSpriteRenderer2DSystem);
		sceneManager.decodeRenderSystem.push_back(DecodeDamageTextSystem);
	}
	
	{ // decode scriptable entities
		sceneManager.decodeScriptableEntity.push_back(DecodeDebugCamera2D);
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
		sceneManager.decodeEntity.push_back(DecodePlantableGroundComponent);
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
}
void App::Loop()
{
	while (appState == AppState::ON)
	{
		deltaTime = time.startFrame();
		sceneManager.SetDeltaTime(deltaTime);

		Update();
		Draw();
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
