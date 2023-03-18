#pragma once

class SplashLoader2 : public Canis::ScriptableEntity
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
        timer += _dt;

        if (timer >= 3.0f)
        {
            ((Canis::SceneManager *)m_Entity.scene->sceneManager)->Load("main_menu");
        }
    }
};

bool DecodeSplashLoader2(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "SplashLoader2")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<SplashLoader2>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}