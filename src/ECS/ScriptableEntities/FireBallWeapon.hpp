#pragma once
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>

class FireBallWeapon : public Weapon
{
    private:
    float timer = 0.0f;
    bool canShoot = true;
    int fireballId = 0;

    public:
    void OnCreate()
    {
        fireballId = GetAssetManager().LoadSpriteAnimation("assets/animations/fireball_weapon.anim");
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            15.0f,              //damage
            glm::vec2(16.0f),   //weapon effect size
            120.0f,             //speed
            6.0f,               //duration
            1,                  //amount
            0.5f                //cooldown
        );
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        if (GetComponent<Canis::RectTransformComponent>().active == false) // add to all weapons
            return;
            
        Weapon::OnUpdate(_dt);
        
        GetComponent<Canis::RectTransformComponent>().position = player.GetComponent<Canis::RectTransformComponent>().position;

        Canis::Entity closestEnemy = FindClosestEnemy(GetComponent<Canis::RectTransformComponent>().position);

        timer -= _dt;
        if(timer <= 0)
        {
            canShoot = true;
        }

        if (canShoot)
        {
            if(closestEnemy.entityHandle != entt::null && closestEnemy.entityHandle != entt::tombstone && m_Entity.scene->entityRegistry.valid(closestEnemy.entityHandle))
            {
                Weapon::Shoot(
                    glm::normalize(closestEnemy.GetComponent<Canis::RectTransformComponent>().position - 
                    GetComponent<Canis::RectTransformComponent>().position), 
                    player.GetComponent<Canis::RectTransformComponent>().position, 
                    fireballId);
            }

            timer = cooldown;
            canShoot = false;
        }
    }

    /*Canis::Entity FindClosestEnemy()
    {
        std::vector<entt::entity> hits = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
        Canis::Entity closestEntity;
        Canis::Entity hitEntity;
        hitEntity.scene = m_Entity.scene;

        float closestDistance = 100000000.0f;

        for(int i = 0; i < hits.size(); i++)
        {
            hitEntity.entityHandle = hits[i];
            if (hits[i] != entt::tombstone && m_Entity.scene->entityRegistry.valid(hits[i]))
            {
                float distance = glm::distance(hitEntity.GetComponent<Canis::RectTransformComponent>().position, GetComponent<Canis::RectTransformComponent>().position);

                if(distance <= closestDistance)
                {
                    closestDistance = distance;
                    closestEntity = hitEntity;
                    return closestEntity;
                }
            }
        }

        return closestEntity;
    }*/
};