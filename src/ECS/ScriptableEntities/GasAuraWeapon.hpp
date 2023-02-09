#pragma once
#include <string>
#include <glm/glm.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/Math.hpp>
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include "../Components/EnemyHealthComponent.hpp"

class GasAuraWeapon : public WeaponClass
{
    private:
        float timer = 0.0f;
        bool canDamage = true;
        
        Canis::Entity closestEntity;

        Canis::Entity player;
        Canis::Camera2DComponent camera;

    public:


        void OnCreate()
        {
            
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

            timer -= _dt;
            if(timer <= 0)
            {
                canDamage = true;
            }

            if (canDamage)
            {
                std::vector<entt::entity> hits = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
                Canis::Entity hitEntity;
                hitEntity.scene = m_Entity.scene;

                //float closestDistance = 100000000.0f;

                // if (hits.size() > 0)
                // {
                    for(int i = 0; i < hits.size(); i++)
                    {
                        hitEntity.entityHandle = hits[i];
                        
                        if (hits[i] != entt::tombstone && m_Entity.scene->entityRegistry.valid(hits[i]))
                        {
                            // float distance = glm::distance(hitEntity.GetComponent<Canis::RectTransformComponent>().position, GetComponent<Canis::RectTransformComponent>().position);

                            // if(distance <= closestDistance)
                            // {
                            //     closestDistance = distance;
                            //     closestEntity = hitEntity;
                            // }

                            hitEntity.GetComponent<EnemyHealthComponent>().currentHealth -= damage;
                        }
                    }

                    timer = 1.0f;
                    canDamage = false;
                //}
                // else
                // {
                //     return;
                // }

                //Damage();
            }
        }

        // void Damage()
        // {
        //     auto e = CreateEntity();

        //     // if(closestEntity.entityHandle == entt::null)
        //     // {
        //     //     return;
        //     // }
            
        //     auto& rect = _entity.AddComponent<Canis::RectTransformComponent>();
        //     rect.position = player.GetComponent<Canis::RectTransformComponent>().position;
        //     rect.size = glm::vec2(64.0f);
        //     rect.depth = 0.2f;

        //     auto& sprite = _entity.AddComponent<Canis::Sprite2DComponent>();
        //     sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(GetAssetManager().LoadTexture("assets/textures/enemies/let_it_bee.png"))->GetTexture();

        //     auto& color = _entity.AddComponent<Canis::ColorComponent>();
        //     color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        //     auto& anim = _entity.AddComponent<Canis::SpriteAnimationComponent>();
        //     anim.animationId = gasId;

        //     auto& circleCollider = _entity.AddComponent<Canis::CircleColliderComponent>();
        //     circleCollider.layer = Canis::BIT::ZERO;
        //     circleCollider.mask = Canis::BIT::TWO;
        //     circleCollider.radius = 128.0f;
        // }
};