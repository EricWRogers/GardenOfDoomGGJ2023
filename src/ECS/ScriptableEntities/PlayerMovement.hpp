#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>

class PlayerMovement : public Canis::ScriptableEntity
{
private:
   float speed = 100.0f;
    glm::vec2 direction;
public:

    void OnCreate() //Awake
    {
        
    }

    void OnReady()//Start
    {
       
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
    }

    
};