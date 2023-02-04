#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/CircleColliderComponent.hpp>

class PeaShooterWeapon : public WeaponClass
{

private:
    Canis::Entity target;

public:


    void OnCreate()
    {
        damage = 10;
    }

    void OnReady()
    {

    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
       auto& col = GetComponent<Canis::CircleColliderComponent>();

        std::vector<entt::entity> hit = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
        Canis::Entity hitEntity;
        hitEntity.scene = m_Entity.scene;

        if (hit.size() > 0) {
            hitEntity.entityHandle = hit[0];
            
        }

    //    if (target.GetEntityWithTag("Enemy"))
    //    {
    //         target.GetComponent<EnemyMovement>().health -= damage * _dt;
    //    }

    


    }
};