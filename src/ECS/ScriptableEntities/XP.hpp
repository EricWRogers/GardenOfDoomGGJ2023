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