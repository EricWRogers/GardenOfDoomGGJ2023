#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include "../Components/EnemyComponent.hpp"
#include "../Components/PlayerHealthComponent.hpp"

class EnemySystem : public Canis::System
{
    private:
    Canis::Entity m_player;
    Canis::CollisionSystem2D *m_collisionSystem2D = nullptr;

    public:
    void Create() 
    {
        
    }

    void Ready() 
    {
        m_player.scene = scene;
        m_player = m_player.GetEntityWithTag("Player");
        m_collisionSystem2D = scene->GetSystem<Canis::CollisionSystem2D>();
    }

    void Update(entt::registry &_registry, float _deltaTime)
    {
        auto view = _registry.view<EnemyComponent>();
        for (auto [entity, enemy] : view.each())
        {
            std::vector<entt::entity> hit = m_collisionSystem2D->GetHits(entity);
            Canis::Entity hitEntity;
            hitEntity.scene = scene;

            if (hit.size() > 0) 
            {
                hitEntity.entityHandle = hit[0];
                if (enemy.step >= enemy.attackCooldown)
                {
                    m_player.GetComponent<PlayerHealthComponent>().currentHealth -= enemy.attackDamage;
                    enemy.step = 0;
                }

                if (m_player.GetComponent<PlayerHealthComponent>().currentHealth <= 0)
                {
                    ((Canis::SceneManager*)scene->sceneManager)->Load("lose");
                }
            }

            enemy.step += _deltaTime;
        }
    }
};