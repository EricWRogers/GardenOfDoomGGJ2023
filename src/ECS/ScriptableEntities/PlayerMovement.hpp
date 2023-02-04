#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>

class PlayerMovement : public Canis::ScriptableEntity
{
private:
   float speed;

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
        
    }
    
};