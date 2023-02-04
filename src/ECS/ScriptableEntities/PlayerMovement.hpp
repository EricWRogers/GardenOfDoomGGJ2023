#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>

class PlayerMovement : public Canis::ScriptableEntity
{
private:
   float speed = 100;
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
        float horizontal = 0;
        float vertical = 0;

        auto& rect = GetComponent<Canis::RectTransformComponent>();
        
        if (GetInputManager().JustPressedKey(SDLK_a) && !GetWindow().GetMouseLock()) //Left
        {
            horizontal = -1.0f;
        }

        if (GetInputManager().JustPressedKey(SDLK_w) && !GetWindow().GetMouseLock()) //Forwards
        {
            vertical = 1.0f;
        }
        if (GetInputManager().JustPressedKey(SDLK_d) && !GetWindow().GetMouseLock()) //Right
        {
            horizontal = 1.0f;
        }
        if (GetInputManager().JustPressedKey(SDLK_s) && !GetWindow().GetMouseLock()) //back
        {
           vertical = -1.0f;
        }

        direction = glm::vec2(horizontal, vertical);
        rect.position = (direction * (speed * _dt));
    }

    
};