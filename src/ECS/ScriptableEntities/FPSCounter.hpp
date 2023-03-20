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
        (*GetComponent<Canis::TextComponent>().text) = "FPS : " + std::to_string((int)m_Entity.scene->window->fps);/* + 
            "DT: " + std::to_string(((Canis::SceneManager*)m_Entity.scene->sceneManager)->drawTime) + 
            " UT: " + std::to_string(((Canis::SceneManager*)m_Entity.scene->sceneManager)->updateTime);*/
        
        //(*GetComponent<Canis::TextComponent>().text) = "FPS : " + std::to_string(((Canis::SceneManager*)m_Entity.scene->sceneManager)->drawTime);
        //(*GetComponent<Canis::TextComponent>().text) = "FPS : " + std::to_string(((Canis::SceneManager*)m_Entity.scene->sceneManager)->updateTime);
    }
};

bool DecodeFPSCounter(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "FPSCounter")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<FPSCounter>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}