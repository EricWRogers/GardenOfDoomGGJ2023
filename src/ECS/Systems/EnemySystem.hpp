#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include "../Components/EnemyComponent.hpp"
#include "../Components/PlayerHealthComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"
#include "ECS/ScriptableEntities/PlayerManager.hpp"
#include "ECS/ScriptableEntities/XP.hpp"

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
        auto view = _registry.view<EnemyComponent, EnemyHealthComponent, Canis::RectTransformComponent>();
        for (auto [entity, enemy, health, transform] : view.each())
        {
            enemy.step += _deltaTime;
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

            if (health.currentHealth <= 0)
            {
                auto e = scene->CreateEntity();

                auto& rectTransform = e.AddComponent<Canis::RectTransformComponent>();
                rectTransform.size = glm::vec2(32.0, 32.0);
                rectTransform.position = transform.position;

                auto& sprite = e.AddComponent<Canis::Sprite2DComponent>();
                sprite.texture = assetManager->Get<Canis::TextureAsset>( 
                    assetManager->LoadTexture("assets/textures/pickups/exp_orb_blue.png"))->GetTexture();

                auto& color = e.AddComponent<Canis::ColorComponent>();
                color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

                auto& collider = e.AddComponent<Canis::CircleColliderComponent>();
                collider.layer = Canis::BIT::THREE;   //double check this later
                collider.mask = Canis::BIT::ONE;
                collider.radius = 16.0;
                collider.center = glm::vec2(0.0);

                auto& xp = e.AddComponent<XP>();
                xp.SetXP(enemy.xpValue);

                _registry.destroy(entity);
            }
        }
    }
};