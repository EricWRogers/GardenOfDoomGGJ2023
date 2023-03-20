#pragma once
#include <Canis/ScriptableEntity.hpp>

class XP : public Canis::ScriptableEntity
{
    private:
    float xpValue = 0.0f;

    public:
    void OnCreate() {}
    void OnReady() {}
    void OnDestroy() {}
    void OnUpdate() {}

    void SetXP(float _value)
    {
        xpValue = _value;
    }

    float GetXP()
    {
        return xpValue;
    }
};

bool DecodeXP(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "XP")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<XP>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}