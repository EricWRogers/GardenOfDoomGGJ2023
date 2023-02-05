#pragma once
#include <string>
#include <glm/glm.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>

#include "../Components/Bullet.hpp"

class PeaShooterWeapon : public WeaponClass
{

private:
    Canis::Entity target;
    float timer = 10;
    bool canShoot = false;

    float dt;
    
    Canis::Entity closestEntity;

    Canis::Entity player;

public:


    void OnCreate()
    {
        damage = 10;
    }

    void OnReady()
    {
        player = m_Entity.GetEntityWithTag("Player");
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        
        GetComponent<Canis::RectTransformComponent>().position = player.GetComponent<Canis::RectTransformComponent>().position;

         if (GetInputManager().JustPressedKey(SDLK_z))
        {
            
        }

        dt = _dt;
         if (GetInputManager().JustPressedKey(SDLK_z))
        {
        std::vector<entt::entity> hit = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
        Canis::Entity hitEntity;
        hitEntity.scene = m_Entity.scene;

        float closestDistance = 100000000.0f;

        if (hit.size() > 0)
        {
            for(int i = 0; i < hit.size(); i++)
            {
                //Canis::Log("id: " + std::to_string((unsigned int)hit[i].entityHandle.));
                hitEntity.entityHandle = hit[i];
                float distance = glm::distance(hitEntity.GetComponent<Canis::RectTransformComponent>().position, GetComponent<Canis::RectTransformComponent>().position);

                if(distance <= closestDistance)
                {
                    closestDistance = distance;
                    closestEntity = hitEntity;
                }
            }
        }

        auto e = CreateEntity();
            Shoot(e);
            
            
            
            timer -= _dt;
            if(timer <= 0)
            {
                timer = 10;
                canShoot = false;
            }
            if(timer >= 10)
            {
                canShoot = true;
            }

        }
    }
    void Shoot(Canis::Entity _entity)
    {
        if(closestEntity.entityHandle == entt::null)
        {
            return;
        }
        Canis::Log("Test");
        auto& rectTransform = _entity.AddComponent<Bullet>();
        
        auto& rect = _entity.AddComponent<Canis::RectTransformComponent>();
        rect.position = player.GetComponent<Canis::RectTransformComponent>().position;
        rect.size = glm::vec2(8.0f);
        rect.depth = 0.2f;

        auto& sprite = _entity.AddComponent<Canis::Sprite2DComponent>();
        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(GetAssetManager().LoadTexture("assets/textures/enemies/let_it_bee.png"))->GetTexture();

        auto& color = _entity.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    }

};