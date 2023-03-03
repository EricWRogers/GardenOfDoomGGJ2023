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
            " drawTime : " + std::to_string(((Canis::SceneManager*)m_Entity.scene->sceneManager)->drawTime) + 
            " updateTime : " + std::to_string(((Canis::SceneManager*)m_Entity.scene->sceneManager)->updateTime);*/
        
        //(*GetComponent<Canis::TextComponent>().text) = "FPS : " + std::to_string(((Canis::SceneManager*)m_Entity.scene->sceneManager)->drawTime);
        //(*GetComponent<Canis::TextComponent>().text) = "FPS : " + std::to_string(((Canis::SceneManager*)m_Entity.scene->sceneManager)->updateTime);
    }
};