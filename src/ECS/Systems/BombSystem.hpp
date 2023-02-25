#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include "../Components/BombComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"
#include "../Components/SmokeComponent.hpp"
#include "../ScriptableEntities/Weapon.hpp"

class BombSystem : public Canis::System
{
    private:
    Canis::CollisionSystem2D *m_collisionSystem2D = nullptr;
    Canis::Entity m_player;
    Canis::Entity camera;
    float bombTimer = 0.0f;
    Canis::Entity weaponEntity;
    

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
        weaponEntity.scene = scene;
        weaponEntity = weaponEntity.GetEntityWithTag("Bomb");
    }

    void Update(entt::registry &_registry, float _deltaTime)
    {
        Weapon &weapon = (*((Weapon*)weaponEntity.GetComponent<Canis::ScriptComponent>().Instance));

        auto view = _registry.view<BombComponent, Canis::SpriteAnimationComponent, Canis::RectTransformComponent>();
        for (auto [entity, bomb, animation, transform] : view.each())
        {            
            bomb.timeLeft -= _deltaTime;
            if (bomb.timeLeft < 0.2f)
            {
                auto smoke = scene->CreateEntity();
                
                auto& s_transform = smoke.AddComponent<Canis::RectTransformComponent>();
                s_transform.size = weapon.size;
                s_transform.position = transform.position;

                auto& s_collider = smoke.AddComponent<Canis::CircleColliderComponent>();
                s_collider.layer = Canis::BIT::ZERO;
                s_collider.mask = Canis::BIT::TWO;
                s_collider.radius = weapon.size.x / 2.0f;

                auto& sprite = smoke.AddComponent<Canis::Sprite2DComponent>();
                sprite.texture = assetManager->Get<Canis::TextureAsset>( 
                    assetManager->LoadTexture("assets/textures/weapons/explosion.png"))->GetTexture();
                
                auto& color = smoke.AddComponent<Canis::ColorComponent>();
                color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

                auto& anim = smoke.AddComponent<Canis::SpriteAnimationComponent>();
                anim.animationId = assetManager->LoadSpriteAnimation("assets/animations/explosion.anim");
                anim.index = 0;

                auto& s = smoke.AddComponent<SmokeComponent>();
                s.timeLeft = weapon.duration;
                s.damage = weapon.damage;

                _registry.destroy(entity);

                continue;
            }
        }
    
    
        auto view2 = _registry.view<SmokeComponent>();
        for (auto [entity, smoke] : view2.each())
        {
            smoke.timeLeft -= _deltaTime;

            if (smoke.timeLeft > 0.0f)
            {
                std::vector<entt::entity> hits = m_collisionSystem2D->GetHits(entity);
                Canis::Entity hitEntity;
                hitEntity.scene = scene;

                for(entt::entity hit : hits)
                {
                    hitEntity.entityHandle = hit;
                    hitEntity.GetComponent<EnemyHealthComponent>().currentHealth -= smoke.damage;
                }
            }

            if (smoke.timeLeft <= 0.0f)
                _registry.destroy(entity);
        }
    }   
};