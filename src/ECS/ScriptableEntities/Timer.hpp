#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>

class Timer : public Canis::ScriptableEntity
{
private:
    bool enable;
    float time;

    
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
            if(time != 0)
            {
                time -= time * _dt;
                std::string minutes = std::to_string((int)time / 60);
                //std::string seconds = std::to_string(time % 60);

                //(*GetComponent<Canis::TextComponent>().text) = "Time Left- " + minutes + ":" + seconds);
            }
            else
            {
                return;
            }
            
        }
    }

    
};