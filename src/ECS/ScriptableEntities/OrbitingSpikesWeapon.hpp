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

class OrbitingSpikesWeapon : public WeaponClass
{
    private:
        float timer = 0.0f;
        bool canDamage = true;
        
        Canis::Entity closestEntity;

        Canis::Entity player;

    public:
    
        void OnCreate()
        {
            
        }

        void OnReady()
        {
            player = m_Entity.GetEntityWithTag("Player");
            damage = 50.0f;
        }
        
        void OnDestroy()
        {

        }

        void OnUpdate(float _dt)
        {
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

                for(int i = 0; i < hits.size(); i++)
                {
                    hitEntity.entityHandle = hits[i];
                    
                    if (hits[i] != entt::tombstone && m_Entity.scene->entityRegistry.valid(hits[i]))
                    {
                        if (glm::distance(GetComponent<Canis::RectTransformComponent>().position, 
                        hitEntity.GetComponent<Canis::RectTransformComponent>().position) >= 
                        GetComponent<Canis::CircleColliderComponent>().radius)
                        {
                            hitEntity.GetComponent<EnemyHealthComponent>().currentHealth -= damage;
                        }
                    }
                }

                timer = 1.0f;
                canDamage = false;
            }
        }

};