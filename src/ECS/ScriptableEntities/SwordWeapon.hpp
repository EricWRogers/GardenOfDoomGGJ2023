#pragma once
#include <string>
#include <glm/glm.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>

#include "../Components/BulletComponent.hpp"

class SwordWeapon : public WeaponClass
{

private:
    Canis::Entity target;
    float timer = 0.0f;
    bool canShoot = true;
    int swordId = 0;

    float dt;
    
    Canis::Entity closestEntity;

    Canis::Entity player;

public:


    void OnCreate()
    {
        damage = 10;
        swordId = GetAssetManager().LoadSpriteAnimation("assets/animations/sword_weapon.anim");
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

            timer = 0.5f;
            canShoot = false;
        }
    }
    void Shoot(Canis::Entity _entity)
    {
        if(closestEntity.entityHandle == entt::null)
        {
            return;
        }
        auto& bullet = _entity.AddComponent<BulletComponent>();

        if (!player.GetComponent<Canis::SpriteAnimationComponent>().flipX)
        {
            bullet.direction = glm::vec2(1.0f, 0.0f);
        }

        if (player.GetComponent<Canis::SpriteAnimationComponent>().flipX)
        {
            bullet.direction = glm::vec2(-1.0f, 0.0f);
        }
        
        bullet.speed = 200.0f;
        bullet.damage = 100.0f;
        bullet.timeLeft = 6.0f;
        
        auto& rect = _entity.AddComponent<Canis::RectTransformComponent>();
        rect.position = player.GetComponent<Canis::RectTransformComponent>().position;
        rect.size = glm::vec2(16.0f);
        rect.depth = 0.2f;

        auto& sprite = _entity.AddComponent<Canis::Sprite2DComponent>();
        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(GetAssetManager().LoadTexture("assets/textures/enemies/let_it_bee.png"))->GetTexture();

        auto& color = _entity.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        auto& anim = _entity.AddComponent<Canis::SpriteAnimationComponent>();
        anim.animationId = swordId;
        anim.flipX = !player.GetComponent<Canis::SpriteAnimationComponent>().flipX;

        auto& circleCollider = _entity.AddComponent<Canis::CircleColliderComponent>();
        circleCollider.layer = Canis::BIT::ZERO;
        circleCollider.mask = Canis::BIT::TWO;
        circleCollider.radius = 16.0f;
    }

};