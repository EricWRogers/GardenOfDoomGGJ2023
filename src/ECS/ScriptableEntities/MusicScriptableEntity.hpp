#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>

class MusicScriptableEntity : public Canis::ScriptableEntity
{
public:
    void OnCreate()
    {
    }

    void OnReady()
    {

        GetAssetManager().Get<Canis::MusicAsset>(GetAssetManager().LoadMusic("assets/music/your_garden_variety_evil.mp3"))->Play(-1);
    }

    void OnDestroy()
    {
        GetAssetManager().Get<Canis::MusicAsset>(GetAssetManager().LoadMusic("assets/music/your_garden_variety_evil.mp3"))->Stop();
    }

    void OnUpdate(float _dt)
    {
    }
};

bool DecodeMusicScriptableEntity(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "MusicScriptableEntity")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<MusicScriptableEntity>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}