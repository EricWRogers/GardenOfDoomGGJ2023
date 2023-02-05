#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include "../Components/BulletComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"

class BulletSystem : public Canis::System
{
    private:
    Canis::CollisionSystem2D *m_collisionSystem2D = nullptr;

    public:
    void Create() 
    {
        
    }

    void Ready() 
    {
        m_collisionSystem2D = scene->GetSystem<Canis::CollisionSystem2D>();
    }

    void Update(entt::registry &_registry, float _deltaTime)
    {
        auto view = _registry.view<Canis::RectTransformComponent, BulletComponent>();
        for (auto [entity, rect, bullet] : view.each())
        {
            rect.position += ((bullet.direction * bullet.speed) * _deltaTime);
            std::vector<entt::entity> hits = m_collisionSystem2D->GetHits(entity);

            Canis::Entity hitEntity;
            hitEntity.scene = scene;

            for(entt::entity hit : hits)
            {
                hitEntity.entityHandle = hit;

                auto& enemyHealthComponent = hitEntity.GetComponent<EnemyHealthComponent>();
                if (enemyHealthComponent.currentHealth > 0)
                {
                    enemyHealthComponent.currentHealth -= bullet.damage;
                    break;
                }
            }

            bullet.timeLeft -= _deltaTime;

            if (bullet.timeLeft < 0.0f)
                _registry.destroy(entity);
        }
    }
};