#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>

class Timer : public Canis::ScriptableEntity
{
private:
    bool enable = true;
    float time = 10.0f;

    
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
         
       
        if(enable == true)
        {
            if(time != 0.0f)
            {
                time -= _dt;
                std::string minutes = std::to_string((int)time / 60);
                std::string seconds = std::to_string((int)time % 60);

                (*GetComponent<Canis::TextComponent>().text) = minutes + ":" + seconds;
                
                if(time <= 0.0f)
                {
                    enable = false;
                    ((Canis::SceneManager*)m_Entity.scene->sceneManager)->Load("win");
                }
            }
            else
            {
                return;
            }
            
        }
    }

    
};