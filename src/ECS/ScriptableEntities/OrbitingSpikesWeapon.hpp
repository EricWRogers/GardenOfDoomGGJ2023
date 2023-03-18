#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/Math.hpp>
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include "../Components/EnemyHealthComponent.hpp"

class OrbitingSpikesWeapon : public Weapon
{
private:
    float timer = 0.0f;
    bool canDamage = true;

public:
    void OnCreate()
    {
        Weapon::OnCreate();
    }

    void OnReady()
    {
        Weapon::OnReady();
        SetBaseStats(
            20.0f,             // damage
            glm::vec2(256.0f), // weapon effect size
            0.0f,              // speed
            0.0f,              // duration
            1,                 // amount
            0.3f               // cooldown
        );
    }

    void OnDestroy()
    {
        Weapon::OnDestroy();
    }

    void OnUpdate(float _dt)
    {
        if (GetComponent<Canis::RectTransformComponent>().active == false) // add to all weapons
            return;

        Weapon::OnUpdate(_dt);

        GetComponent<Canis::CircleColliderComponent>().radius = size.x / 2.0f;
        GetComponent<Canis::RectTransformComponent>().size = size;

        GetComponent<Canis::RectTransformComponent>().position = player.GetComponent<Canis::RectTransformComponent>().position;

        timer -= _dt;
        if (timer <= 0)
        {
            canDamage = true;
        }

        if (canDamage)
        {
            std::vector<entt::entity> hits = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
            Canis::Entity hitEntity;
            hitEntity.scene = m_Entity.scene;

            for (int i = 0; i < hits.size(); i++)
            {
                hitEntity.entityHandle = hits[i];

                // if (hitEntity.entityHandle != entt::null)
                //{
                if (glm::distance(GetComponent<Canis::RectTransformComponent>().position,
                                  hitEntity.GetComponent<Canis::RectTransformComponent>().position) >=
                    GetComponent<Canis::CircleColliderComponent>().radius)
                {
                    EnemyHealth::DamageEnemy(hitEntity.GetComponent<EnemyHealthComponent>(), damage);
                }
                //}
            }

            timer = cooldown;
            canDamage = false;
        }
    }
};

bool DecodeOrbitingSpikesWeapon(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "OrbitingSpikesWeapon")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<OrbitingSpikesWeapon>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}