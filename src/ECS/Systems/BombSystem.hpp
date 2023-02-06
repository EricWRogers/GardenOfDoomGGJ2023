#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include <Canis/Math.hpp>
#include "../Components/BombComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"
#include "../Components/SmokeComponent.hpp"

class BombSystem : public Canis::System
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
        auto view = _registry.view<BombComponent, Canis::SpriteAnimationComponent, Canis::RectTransformComponent>();
        for (auto [entity, bomb, animation, transform] : view.each())
        {
            //transform.position = glm::vec2(Canis::RandomFloat(camera.GetComponent<Canis::Camera2DComponent>().position.x - window->GetScreenWidth()/2.0f, camera.GetComponent<Canis::Camera2DComponent>().position.x - window->GetScreenWidth()/2.0f), Canis::RandomFloat(camera.GetComponent<Canis::Camera2DComponent>().position.y - window->GetScreenHeight()/2.0f, camera.GetComponent<Canis::Camera2DComponent>().position.y - window->GetScreenHeight()/2.0f));
            
            bomb.timeLeft -= _deltaTime;
            if (bomb.timeLeft < 0.0f)
            {
                if (animation.index == 0)
                {
                    auto e = scene->CreateEntity();

                    std::vector<entt::entity> hits = m_collisionSystem2D->GetHits(entity);
                    Canis::Entity hitEntity;
                    hitEntity.scene = scene;

                    for(entt::entity hit : hits)
                    {
                        hitEntity.entityHandle = hit;
                        if (hit != entt::tombstone && scene->entityRegistry.valid(hit))
                        {
                            // damage
                            hitEntity.GetComponent<EnemyHealthComponent>().currentHealth -= bomb.damage;
                        }
                    }

                    auto smoke = scene->CreateEntity();
                    
                    auto& s_transform = smoke.AddComponent<Canis::RectTransformComponent>();
                    s_transform.size = glm::vec2(64.0, 64.0);
                    s_transform.position = transform.position;

                    auto& sprite = smoke.AddComponent<Canis::Sprite2DComponent>();
                    sprite.texture = assetManager->Get<Canis::TextureAsset>( 
                        assetManager->LoadTexture("assets/textures/weapons/explosion.png"))->GetTexture();
                    
                    auto& color = smoke.AddComponent<Canis::ColorComponent>();
                    color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

                    auto& anim = smoke.AddComponent<Canis::SpriteAnimationComponent>();
                    anim.animationId = assetManager->LoadSpriteAnimation("assets/animations/explosion.anim");
                    anim.index = 0;

                    auto& s = smoke.AddComponent<SmokeComponent>();


                    // kill 
                    _registry.destroy(entity);

                    continue;
                }
            }
        }
    
    
        auto view2 = _registry.view<SmokeComponent>();
        for (auto [entity, smoke] : view2.each())
        {
            smoke.timeLeft -= _deltaTime;

            if (smoke.timeLeft < 0.0f)
                _registry.destroy(entity);
        }
    }   
};