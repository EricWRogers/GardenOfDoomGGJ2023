#pragma once
#include <Canis/ECS/Systems/System.hpp>
#include "../Components/EnemyComponent.hpp"
#include "../Components/PlayerHealthComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"
#include "../Components/EnemyHealthComponent.hpp"
#include "../Components/SeedComponent.hpp"
#include "../ScriptableEntities/PlayerManager.hpp"
#include "../ScriptableEntities/WaveManager.hpp"
#include "../ScriptableEntities/SeedPickup.hpp"

class EnemySystem : public Canis::System
{
    private:
    Canis::Entity m_player;
    Canis::Entity waveManagerEntity;
    Canis::CollisionSystem2D *m_collisionSystem2D = nullptr;
    int blueXpIdleId = 0;
    int purpleXpIdleId = 0;
    int rainbowXpIdleId = 0;
    bool seedDropped = false;

    bool SpawnSeed(glm::vec2 position)
    {
        if (((WaveManager*)waveManagerEntity.GetComponent<Canis::ScriptComponent>().Instance)->currentEnemy.seedDropChance >= 0 && !seedDropped)
        {
            auto e = scene->CreateEntity();

            auto& transform = e.AddComponent<Canis::RectTransformComponent>();
            transform.size = glm::vec2(16.0f);
            transform.position = position;
            transform.anchor = 6;

            auto& collider = e.AddComponent<Canis::CircleColliderComponent>();
            collider.layer = Canis::BIT::THREE;
            collider.mask = Canis::BIT::ONE;
            collider.radius = 16.0f;

            auto& sprite = e.AddComponent<Canis::Sprite2DComponent>();
            sprite.texture = assetManager->Get<Canis::TextureAsset>( 
                assetManager->LoadTexture("assets/textures/weapons/explosion.png"))->GetTexture();
            
            auto& color = e.AddComponent<Canis::ColorComponent>();
            color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

            auto& anim = e.AddComponent<Canis::SpriteAnimationComponent>();
            anim.animationId = assetManager->LoadSpriteAnimation("assets/animations/apple_a_day.anim");
            anim.index = 0;

            auto& seed = e.AddComponent<SeedComponent>();

            seedDropped = true;

            return true;
        }

        else return false;
    }

    public:
    void Create() 
    {
        blueXpIdleId = assetManager->LoadSpriteAnimation("assets/animations/exp_orb_blue.anim");
        purpleXpIdleId = assetManager->LoadSpriteAnimation("assets/animations/exp_orb_purple.anim");
        rainbowXpIdleId = assetManager->LoadSpriteAnimation("assets/animations/exp_orb_rainbow.anim");
    }

    void Ready() 
    {
        m_player.scene = scene;
        m_player = m_player.GetEntityWithTag("Player");
        waveManagerEntity.scene = scene;
        waveManagerEntity = waveManagerEntity.GetEntityWithTag("WaveManager");
        m_collisionSystem2D = scene->GetSystem<Canis::CollisionSystem2D>();
    }

    void Update(entt::registry &_registry, float _deltaTime)
    {
        auto& playerRect = m_player.GetComponent<Canis::RectTransformComponent>();
        Canis::Entity entityEnemy;
        entityEnemy.scene = scene;
        
        auto view = _registry.view<EnemyComponent, EnemyHealthComponent, Canis::RectTransformComponent>();
        for (auto [entity, enemy, health, transform] : view.each())
        {
            // look at player
            bool playerIsLeft = ((transform.position.x - playerRect.position.x) < 0.0f);
            if(playerIsLeft != enemy.wasFlipX)
            {
                entityEnemy.entityHandle = entity;
                auto& anim = entityEnemy.GetComponent<Canis::SpriteAnimationComponent>();
                enemy.wasFlipX = playerIsLeft;
                anim.flipX = playerIsLeft;
                anim.redraw = true;
            }
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

            if (health.tookThisFrame > 0) {
                // spawn in damage text
                // TODO : Object Pool this later
                Canis::Entity damageTextEntity = GetScene().CreateEntity();
                Canis::RectTransformComponent& rtc = damageTextEntity.AddComponent<Canis::RectTransformComponent>();
                rtc = transform;
                Canis::ColorComponent& cc = damageTextEntity.AddComponent<Canis::ColorComponent>();

                if (health.tookThisFrame/health.maxHealth > 0.75f)
                    cc.color = glm::vec4(1.0f,0.0f,0.0f,1.0f);
                else
                    cc.color = glm::vec4(1.0f,0.64f,0.0f,1.0f);

                DamageTextComponent& dtc = damageTextEntity.AddComponent<DamageTextComponent>();
                DamageText::SetText(dtc, (unsigned short)health.tookThisFrame);

                enemy.hitCountDown = ENEMYHITTIME;
                health.tookThisFrame = 0;
            }

            if (enemy.hitCountDown > 0.0f)
            {
                enemy.hitCountDown -= _deltaTime;
                Canis::ColorComponent& c = _registry.get<Canis::ColorComponent>(entity);

                if (enemy.hitCountDown <= 0.0f) {
                    c.color = glm::vec4(1.0f);
                } else {
                    c.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                }
            }

            if (health.currentHealth <= 0)
            {
                bool success = SpawnSeed(transform.position);

                if (success)
                {
                    _registry.destroy(entity);
                    continue;
                }

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
                collider.radius = 32.0;
                collider.center = glm::vec2(0.0);

                auto& xp = e.AddComponent<XP>();
                xp.SetXP(enemy.xpValue);

                auto& anim = e.AddComponent<Canis::SpriteAnimationComponent>();
                anim.animationId = blueXpIdleId;
                if (xp.GetXP() >= 100)
                {
                    anim.animationId = rainbowXpIdleId;
                }
                if (xp.GetXP() >= 50 && xp.GetXP() <= 99)
                {
                    anim.animationId = purpleXpIdleId;
                }
                if (xp.GetXP() < 50)
                {
                    anim.animationId = blueXpIdleId;
                }

                anim.index = 0;

                _registry.destroy(entity);
            }
        }
    }
};

bool DecodeEnemySystem(const std::string &_name, Canis::Scene *_scene) {
    if(_name == "EnemySystem"){
        _scene->CreateSystem<EnemySystem>();
        return true;
    }
    return false;
}