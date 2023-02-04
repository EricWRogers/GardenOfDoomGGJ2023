#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>
#include <Canis/ECS/Components/ButtonComponent.hpp>

class MainMenuButton : public Canis::ScriptableEntity
{
    static void OnClickMainMenu(void *instance)
    {
        Canis::Log("Click");
        ((Canis::SceneManager *)((MainMenuButton * )instance)->m_Entity.scene->sceneManager)->Load("main_menu");
    }
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {
        Canis::Entity mainMenuButton = CreateEntity();
        auto& playRect = mainMenuButton.AddComponent<Canis::RectTransformComponent>(
            true,                                                // active
            Canis::RectAnchor::CENTER,
            glm::vec2(-100.0f, -50.0f), // position
            glm::vec2(150.0f, 40.0f),                               // size
            glm::vec2(0.0f, 0.0f),                                  // originOffset
            0.0f,                                                   // rotation
            1.0f,                                                // scale
            0.0f 
        );
        auto& playColor = mainMenuButton.AddComponent<Canis::ColorComponent>(
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
        );
        auto& playText = mainMenuButton.AddComponent<Canis::TextComponent>(
            GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
            new std::string("Main Menu"), // text
            0u
        );
        auto& playB = mainMenuButton.AddComponent<Canis::ButtonComponent>(
            OnClickMainMenu,
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