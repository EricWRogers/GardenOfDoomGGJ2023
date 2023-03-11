#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>
#include <Canis/ECS/Components/ButtonComponent.hpp>

class MainMenuButtons : public Canis::ScriptableEntity
{
    static void OnClickPlay(void *instance)
    {
        ((MainMenuButtons * )instance)->GetAssetManager().Get<Canis::SoundAsset>(((MainMenuButtons * )instance)->GetAssetManager().LoadSound("assets/sounds/click.wav"))->Play();
        ((Canis::SceneManager *)((MainMenuButtons * )instance)->m_Entity.scene->sceneManager)->Load("main");
    }

    static void OnClickQuit(void *instance)
    {
        exit(1);
    }
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {
        Canis::Entity playButton = CreateEntity();
        auto& playRect = playButton.AddComponent<Canis::RectTransformComponent>(
            true,                                                // active
            Canis::RectAnchor::CENTERLEFT,
            glm::vec2(200.0f, 50.0f), // position
            glm::vec2(150.0f, 40.0f),                               // size
            glm::vec2(0.0f, 0.0f),                                  // originOffset
            0.0f,                                                   // rotation
            1.0f,                                                // scale
            0.0f 
        );
        auto& playColor = playButton.AddComponent<Canis::ColorComponent>(
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
        );
        auto& playText = playButton.AddComponent<Canis::TextComponent>(
            GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
            new std::string("Play"), // text
            0u
        );
        auto& playB = playButton.AddComponent<Canis::ButtonComponent>(
            OnClickPlay,
            this,
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4(0.24f, 0.37f, 0.25f, 1.0f) //Green, #3c5e40
        );

        Canis::Entity quitButton = CreateEntity();
        auto& quitRect = quitButton.AddComponent<Canis::RectTransformComponent>(
            true,                                                // active
            Canis::RectAnchor::CENTERLEFT,
            glm::vec2(200.0f, -50.0f), // position
            glm::vec2(150.0f, 40.0f),                               // size
            glm::vec2(0.0f, 0.0f),                                  // originOffset
            0.0f,                                                   // rotation
            1.0f,                                                // scale
            0.0f 
        );
        auto& quitColor = quitButton.AddComponent<Canis::ColorComponent>(
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
        );
        auto& quitText = quitButton.AddComponent<Canis::TextComponent>(
            GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
            new std::string("Quit"), // text
            0u
        );
        auto& quitB = quitButton.AddComponent<Canis::ButtonComponent>(
            OnClickQuit,
            this,
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
        );
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        
    }


};