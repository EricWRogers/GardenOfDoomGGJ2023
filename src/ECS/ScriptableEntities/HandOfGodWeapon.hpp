#pragma once
#include <string>
#include <glm/glm.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/Math.hpp>
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include "../Components/HandOfGodComponent.hpp"

class HandOfGodWeapon : public WeaponClass
{

private:
    Canis::Entity target;
    float timer = 0.0f;
    bool canShoot = true;
    int handId = 0;
    bool firstStartFrame = false;
    bool canExplode = false;

    float dt;
    
    Canis::Entity closestEntity;

    Canis::Entity player;
    Canis::Camera2DComponent camera;

public:


    void OnCreate()
    {
        handId = GetAssetManager().LoadSpriteAnimation("assets/animations/hand_of_god.anim");
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
        camera = m_Entity.GetEntityWithTag("Camera").GetComponent<Canis::Camera2DComponent>();

        if (GetComponent<Canis::RectTransformComponent>().active == false) // add to all weapons
            return;
        
        GetComponent<Canis::RectTransformComponent>().position = player.GetComponent<Canis::RectTransformComponent>().position;

        dt = _dt;

        timer -= _dt;
        if(timer <= 0)
        {
            canShoot = true;
        }

        if (canShoot)
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
                    
                    if (hit[i] != entt::tombstone && m_Entity.scene->entityRegistry.valid(hit[i]))
                    {
                        float distance = glm::distance(hitEntity.GetComponent<Canis::RectTransformComponent>().position, GetComponent<Canis::RectTransformComponent>().position);

                        if(distance <= closestDistance)
                        {
                            closestDistance = distance;
                            closestEntity = hitEntity;
                        }
                    }
                }
            }
            else
            {
                return;
            }

            auto e = CreateEntity();
            Shoot(e);

            timer = 2.0f;
            canShoot = false;
        }
    }
    void Shoot(Canis::Entity _entity)
    {
        if(closestEntity.entityHandle == entt::null)
        {
            return;
        }
        
        auto& rect = _entity.AddComponent<Canis::RectTransformComponent>();
        rect.position = glm::vec2(
            Canis::RandomFloat(camera.position.x - GetWindow().GetScreenWidth()/2.0f, camera.position.x + GetWindow().GetScreenWidth()/2.0f),
            Canis::RandomFloat(camera.position.y - GetWindow().GetScreenHeight()/2.0f, camera.position.y + GetWindow().GetScreenHeight()/2.0f));
        rect.size = glm::vec2(64.0f);
        rect.depth = 0.2f;

        auto& sprite = _entity.AddComponent<Canis::Sprite2DComponent>();
        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(GetAssetManager().LoadTexture("assets/textures/enemies/let_it_bee.png"))->GetTexture();

        auto& color = _entity.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        auto& anim = _entity.AddComponent<Canis::SpriteAnimationComponent>();
        anim.animationId = handId;

        auto& circleCollider = _entity.AddComponent<Canis::CircleColliderComponent>();
        circleCollider.layer = Canis::BIT::ZERO;
        circleCollider.mask = Canis::BIT::TWO;
        circleCollider.radius = 128.0f;

        auto& hand = _entity.AddComponent<HandOfGodComponent>();
        hand.damage = 35.0f;
        hand.timeLeft = 0.5f;
    }

};