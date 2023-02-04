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
    int idleId = 0;
    int runId = 0;
    bool wasMoving = false;
public:

    void OnCreate() //Awake
    {
        idleId = GetAssetManager().LoadSpriteAnimation("assets/animations/player_idle.anim");
        runId = GetAssetManager().LoadSpriteAnimation("assets/animations/player_run.anim");
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
        auto& anim = GetComponent<Canis::SpriteAnimationComponent>();

        bool moving = false;
        
        
        if (keystate[SDL_SCANCODE_A] && !GetWindow().GetMouseLock()) //Left
        {
            moving = true;
            horizontal = -1.0f;
            if(anim.flipX != true)
            {
                anim.flipX = true;
                anim.redraw = true;
            }
        }

        if (keystate[SDL_SCANCODE_W] && !GetWindow().GetMouseLock()) //Forwards
        {
            moving = true;
            vertical = 1.0f;
        }
        if (keystate[SDL_SCANCODE_D] && !GetWindow().GetMouseLock()) //Right
        {
            moving = true;
            horizontal = 1.0f;
            if (anim.flipX != false)
            {
                anim.flipX = false;
                anim.redraw = true;
            }
        }
        if (keystate[SDL_SCANCODE_S] && !GetWindow().GetMouseLock()) //back
        {
            moving = true;
            vertical = -1.0f;
        }

        direction = (glm::vec2(horizontal, vertical) == glm::vec2(0.0f)) ? glm::vec2(0.0f) : glm::normalize(glm::vec2(horizontal, vertical));
        rect.position += (direction * (speed * _dt));


        if (moving && !wasMoving) // change to run
        {
            anim.redraw = true;
            anim.index = 0;
            anim.animationId = runId;
        }

        if (!moving && wasMoving) // idle
        {
            anim.redraw = true;
            anim.index = 0;
            anim.animationId = idleId;
        }

        wasMoving = moving;

        if (!m_Entity.HasComponent<PlayerHealthComponent>())
        {
            Canis::FatalError("You ded for real");
        }
        auto& playerHealth = GetComponent<PlayerHealthComponent>();
        (*m_healthText.GetComponent<Canis::TextComponent>().text) = "Health: " + std::to_string((int)playerHealth.currentHealth);
        (*m_healthText.GetComponent<Canis::TextComponent>().text) += ".";
        (*m_healthText.GetComponent<Canis::TextComponent>().text) += std::to_string((int)((playerHealth.currentHealth - (int)playerHealth.currentHealth) * 100.0f));
        // Canis::Log("Test" + *m_healthText.GetComponent<Canis::TextComponent>().text);
    }
};