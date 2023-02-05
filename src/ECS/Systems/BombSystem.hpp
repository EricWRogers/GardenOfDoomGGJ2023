#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include "../Components/BombComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"

class BombSystem : public Canis::System
{
    private:
    Canis::CollisionSystem2D *m_collisionSystem2D = nullptr;
    Canis::Entity m_player;

    public:
    void Create() 
    {
        
    }

    void Ready() 
    {
        m_collisionSystem2D = scene->GetSystem<Canis::CollisionSystem2D>();
        m_player.scene = scene;
        m_player = m_player.GetEntityWithTag("Player");
    }

    void Update(entt::registry &_registry, float _deltaTime)
    {
        auto view = _registry.view<BombComponent, Canis::SpriteAnimationComponent>();
        for (auto [entity, bomb, animation] : view.each())
        {
            if (!bomb.stillOnFirstZero)
            {
                if (animation.index == 0)
                {
                    // boom
                    auto e = scene->CreateEntity();
                    // make smoke
                    std::vector<entt::entity> hits = m_collisionSystem2D->GetHits(entity);
                    Canis::Entity hitEntity;
                    hitEntity.scene = scene;

                    for(entt::entity hit : hits)
                    {
                        hitEntity.entityHandle = hit;
                        if (hit != entt::tombstone && scene->entityRegistry.valid(hit))
                        {
                            // damage
                            m_player.GetComponent<PlayerHealthComponent>().currentHealth -= bomb.damage;
                        }
                    }
                    // kill 

                    continue;
                }
            }

            if (bomb.stillOnFirstZero)
            {
                if (animation.index != 0)
                {
                    bomb.stillOnFirstZero = false;
                }
            }
        }
    }
};