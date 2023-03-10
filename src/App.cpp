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
#include "ECS/ScriptableEntities/Music.hpp"
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

		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "EnemySpawnManager"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<EnemySpawnManager>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "PlayerManager"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<PlayerManager>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "FPSCounter"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<FPSCounter>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "Timer"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<Timer>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "MainMenuButtons"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<MainMenuButtons>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "MainMenuButton"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<MainMenuButton>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "MapBuilder"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<MapBuilder>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);
		
		sceneManager.decodeScriptableEntity.push_back(
			[](const std::string &_name, Canis::Entity &_entity) {
				if(_name == "PeaShooterWeapon"){
					Canis::ScriptComponent scriptComponent = {};
            		scriptComponent.Bind<PeaShooterWeapon>();
					_entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
					return true;
				}
				return false;
			}
		);

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "Weapon"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<Weapon>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "WaveManager"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<WaveManager>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "XP"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<XP>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "Music"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<Music>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "FireBallWeapon"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<FireBallWeapon>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "SwordWeapon"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<SwordWeapon>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "BombWeapon"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<BombWeapon>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "HandOfGodWeapon"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<HandOfGodWeapon>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "GasAuraWeapon"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<GasAuraWeapon>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "OrbitingSpikesWeapon"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<OrbitingSpikesWeapon>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "SplashLoader"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<SplashLoader>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "SplashLoader2"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<SplashLoader2>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "SeedPickup"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<SeedPickup>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );

		sceneManager.decodeScriptableEntity.push_back(
            [](const std::string &_name, Canis::Entity &_entity) {
                if(_name == "HUDStateMachine"){
                    Canis::ScriptComponent scriptComponent = {};
                    scriptComponent.Bind<HUDStateMachine>();
                    _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
                    return true;
                }
                return false;
            }
        );
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
