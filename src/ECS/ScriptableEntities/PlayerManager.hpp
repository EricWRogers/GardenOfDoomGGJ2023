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
    float cooldown = 1.0;
    float step = 0.0;

    void HandleHealth(float _dt)
    {
        // std::vector<entt::entity> hit = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
        // Canis::Entity hitEntity;
        // hitEntity.scene = m_Entity.scene;

        // if (hit.size() > 0) 
        // {
        //     hitEntity.entityHandle = hit[0];

        //     if (step >= cooldown)
        //     {
        //         GetComponent<PlayerHealthComponent>().currentHealth -= 1.0;
        //         step = 0;
        //     }

        //     if (GetComponent<PlayerHealthComponent>().currentHealth <= 0)
        //     {
        //         ((Canis::SceneManager*)m_Entity.scene->sceneManager)->Load("lose");
        //     }
        // }

        //step += _dt;

        auto& playerHealth = GetComponent<PlayerHealthComponent>();
        (*m_healthText.GetComponent<Canis::TextComponent>().text) = "Health: " + std::to_string((int)playerHealth.currentHealth);
        (*m_healthText.GetComponent<Canis::TextComponent>().text) += ".";
        (*m_healthText.GetComponent<Canis::TextComponent>().text) += std::to_string((int)((playerHealth.currentHealth - (int)playerHealth.currentHealth) * 100.0f));

    }
public:

    void OnCreate() //Awake
    {
        
    }

    void OnReady()//Start
    {
       m_healthText = m_Entity.GetEntityWithTag("HealthText");
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

        HandleHealth(_dt);


    }
};