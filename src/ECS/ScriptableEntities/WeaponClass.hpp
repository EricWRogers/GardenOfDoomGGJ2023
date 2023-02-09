#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>


class WeaponClass : public Canis::ScriptableEntity
{
public:
    bool enabled;
    float damage;
    float rarity;
    float speed;


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
       
    }
};