#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>

class MapBuilder : public Canis::ScriptableEntity
{
private:
    int width = 100;
    int height = 100;
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {
        int halfWidth = width/2;
        int halfHeight = height/2;
        for(int x = -halfWidth; x < halfWidth; x++)
        {
            for(int y = -halfHeight; y < halfHeight; y++)
            {
                
            }
        }
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        
    }
};