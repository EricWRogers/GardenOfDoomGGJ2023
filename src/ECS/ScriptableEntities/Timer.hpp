#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>

class Timer : public Canis::ScriptableEntity
{
private:
    bool enable = true;
    float time = 60.0f;

    bool isPlaying = false;

    
public:

    void OnCreate() //Awake
    {
        
    }

    void OnReady()//Start
    {
       GetAssetManager().Get<Canis::MusicAsset>(GetAssetManager().LoadMusic("assets/music/your_garden_variety_evil.mp3"))->Play(-1);
    }
    
    void OnDestroy()
    {
        GetAssetManager().Get<Canis::MusicAsset>(GetAssetManager().LoadMusic("assets/music/botanicore.mp3"))->Stop();
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
                
                if(seconds.length() <= 1)
                {
                     (*GetComponent<Canis::TextComponent>().text) = minutes + ":0" + seconds;
                }
                if(time <= 0.0f)
                {
                    enable = false;
                    ((Canis::SceneManager*)m_Entity.scene->sceneManager)->Load("win");
                }
                if(time <= 60.0f && !isPlaying)
                {
                    isPlaying = true;
                    GetAssetManager().Get<Canis::MusicAsset>(GetAssetManager().LoadMusic("assets/music/botanicore.mp3"))->Play(-1);
                }
            }
            else
            {
                return;
            }
        }
    }
};