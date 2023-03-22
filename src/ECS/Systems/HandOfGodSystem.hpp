#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include <Canis/Math.hpp>
#include "../Components/HandOfGodComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"

class HandOfGodSystem : public Canis::System
{
    private:
    Canis::CollisionSystem2D *m_collisionSystem2D = nullptr;
    Canis::Entity m_player;
    Canis::Entity camera;
    float bombTimer = 0.0f;

    public:
    void Create() 
    {
        
    }

    void Ready() 
    {
        m_collisionSystem2D = scene->GetSystem<Canis::CollisionSystem2D>();
        m_player.scene = scene;
        m_player = m_player.GetEntityWithTag("Player");
        camera.scene = scene;
        camera = camera.GetEntityWithTag("Camera");
    }

    void Update(entt::registry &_registry, float _deltaTime)
    {
        auto view = _registry.view<HandOfGodComponent, Canis::SpriteAnimationComponent, Canis::RectTransformComponent>();
        for (auto [entity, hand, animation, transform] : view.each())
        {            
            hand.timeLeft -= _deltaTime;
            if (hand.timeLeft > 0.0f)
            {
                std::vector<entt::entity> hits = m_collisionSystem2D->GetHits(entity);
                Canis::Entity hitEntity;
                hitEntity.scene = scene;

                for(entt::entity hit : hits)
                {
                    hitEntity.entityHandle = hit;
                    if (hit != entt::tombstone && scene->entityRegistry.valid(hit))
                    {
                        EnemyHealth::DamageEnemy(hitEntity.GetComponent<EnemyHealthComponent>() ,hand.damage);
                    }
                }

                continue;
            }

            if (hand.timeLeft <= 0.0f)
            {
                // kill
                _registry.destroy(entity);

                continue;
            }
        }
    }   
};

bool DecodeHandOfGodSystem(const std::string &_name, Canis::Scene *_scene) {
    if(_name == "HandOfGodSystem"){
        _scene->CreateSystem<HandOfGodSystem>();
        return true;
    }
    return false;
}