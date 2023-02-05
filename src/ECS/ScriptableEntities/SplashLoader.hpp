#pragma once

class SplashLoader : public Canis::ScriptableEntity
{

private:
    float timer = 0.0f;
    bool hasCalledLoad = false;

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
        timer+=_dt;

        if (timer >= 3.0f)
        {
            ((Canis::SceneManager*)m_Entity.scene->sceneManager)->Load("main");
        }
    }
};