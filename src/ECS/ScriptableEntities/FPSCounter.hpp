#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>

class FPSCounter : public Canis::ScriptableEntity
{
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {

    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        (*GetComponent<Canis::TextComponent>().text) = "FPS : " + std::to_string((int)m_Entity.scene->window->fps);
    }
};