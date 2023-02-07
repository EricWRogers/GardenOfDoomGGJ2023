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
        int border = 20;
        Canis::GLTexture texture = GetAssetManager().Get<Canis::TextureAsset>(
                GetAssetManager().LoadTexture("assets/textures/environment/background_sprite_sheet.png"))->GetTexture();
        Canis::GLTexture borderTexture = GetAssetManager().Get<Canis::TextureAsset>(
                GetAssetManager().LoadTexture("assets/textures/environment/border_environment_sprite_sheet.png"))->GetTexture();
        
        for(int x = -halfWidth; x < halfWidth; x++)
        {
            for(int y = -halfHeight; y < halfHeight; y++)
            {
                Canis::Entity tile = CreateEntity();
                auto& tileRect = tile.AddComponent<Canis::RectTransformComponent>();
                tileRect.position.x = x*32;
                tileRect.position.y = y*32;
                tileRect.size.x = 32;
                tileRect.size.y = 32;
                tileRect.depth = 2.0f;
                auto& tileColor = tile.AddComponent<Canis::ColorComponent>();
                tileColor.color = glm::vec4(1.0f);
                auto& sprite = tile.AddComponent<Canis::Sprite2DComponent>();

                if(x > ((GetWindow().GetScreenWidth()/64.0f) - halfWidth) && 
                   x < (halfWidth - (GetWindow().GetScreenWidth()/64.0f)) && 
                   y < (halfHeight - (GetWindow().GetScreenHeight()/64.0f)) &&
                   y > ((GetWindow().GetScreenHeight()/64.0f) - halfHeight))
                {
                    sprite.texture = texture;
                    GetSpriteFromTextureAtlas(
                        sprite,
                        0,
                        0,
                        rand() % 3,
                        rand() % 3,
                        16,
                        16,
                        (0 == rand() % 2),
                        (0 == rand() % 2)
                    );
                }else{
                    sprite.texture = borderTexture;
                    GetSpriteFromTextureAtlas(
                        sprite,
                        0,
                        0,
                        3,
                        0,
                        16,
                        16,
                        false,
                        false
                    );
                }
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