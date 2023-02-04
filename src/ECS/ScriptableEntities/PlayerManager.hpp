#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include "../Components/PlayerHealthComponent.hpp"

class PlayerManager : public Canis::ScriptableEntity
{
private:
   float speed = 100.0f;
    glm::vec2 direction;
    Canis::Entity m_healthText;
public:

    void OnCreate() //Awake
    {
        
    }

    void OnReady()//Start
    {
       m_healthText = m_Entity.GetEntityWithTag("HealthText");

       if (m_healthText.entityHandle == entt::null)
       {
            Canis::FatalError("You ded");
       }
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt) //Update
    {
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        
        float horizontal = 0.0f;
        float vertical = 0.0f;

        auto& rect = GetComponent<Canis::RectTransformComponent>();
        
        if (keystate[SDL_SCANCODE_A] && !GetWindow().GetMouseLock()) //Left
        {
            horizontal = -1.0f;
        }

        if (keystate[SDL_SCANCODE_W] && !GetWindow().GetMouseLock()) //Forwards
        {
            vertical = 1.0f;
        }
        if (keystate[SDL_SCANCODE_D] && !GetWindow().GetMouseLock()) //Right
        {
            horizontal = 1.0f;
        }
        if (keystate[SDL_SCANCODE_S] && !GetWindow().GetMouseLock()) //back
        {
           vertical = -1.0f;
        }

        direction = glm::vec2(horizontal, vertical);
        rect.position += (direction * (speed * _dt));

        if (!m_Entity.HasComponent<PlayerHealthComponent>())
        {
            Canis::FatalError("You ded for real");
        }
        auto& playerHealth = GetComponent<PlayerHealthComponent>();
        (*m_healthText.GetComponent<Canis::TextComponent>().text) = std::to_string((int)playerHealth.currentHealth);
        (*m_healthText.GetComponent<Canis::TextComponent>().text) += ".";
        (*m_healthText.GetComponent<Canis::TextComponent>().text) += std::to_string((playerHealth.currentHealth - (int)playerHealth.currentHealth) * 100.0f);
    }
};