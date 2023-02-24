#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>
#include "../Components/BulletComponent.hpp"

class Weapon : public Canis::ScriptableEntity
{
    protected:
    float baseDamage;
    glm::vec2 baseSize;
    float baseSpeed;
    float baseDuration;
    int baseAmount;
    float baseCooldown;

    protected:
    void SetBaseStats(float _bDamge, glm::vec2 _bSize, float _bSpeed, float _bDuration, int _bAmount, float _bCooldown)
    {
        baseDamage = _bDamge;
        baseSize = _bSize;
        baseSpeed = _bSpeed;
        baseDuration = _bDuration;
        baseAmount = _bAmount;
        baseCooldown = _bCooldown;
    }

    public:
    bool enabled;
    float rarity;
    Canis::Entity player;
    Canis::Entity camera;
    float damage;
    glm::vec2 size;
    float speed;
    float duration;
    int amount;
    float cooldown;

    void OnCreate()
    {
        
    }

    void OnReady()
    {
        camera = m_Entity.GetEntityWithTag("Camera");
        player = m_Entity.GetEntityWithTag("Player");
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
       damage = baseDamage * PlayerStats.might.value;
       size = baseSize * PlayerStats.area.value;
       speed = baseSpeed * PlayerStats.weaponSpeed.value;
       duration = baseDuration * PlayerStats.duration.value;
       amount = baseAmount + PlayerStats.amount.value;
       cooldown = baseCooldown * PlayerStats.cooldown.value;
    }

    void Shoot(glm::vec2 _direction, glm::vec2 _startingPos, int _animId)
    {
        for (int i = 0; i < amount; i++)
        {
            auto e = CreateEntity();

            auto& bullet = e.AddComponent<BulletComponent>();
            bullet.direction = _direction;
            bullet.speed = speed;
            bullet.damage = damage;
            bullet.timeLeft = duration;
            
            auto& rect = e.AddComponent<Canis::RectTransformComponent>();
            rect.position = glm::vec2(_startingPos.x, _startingPos.y + (i * 10.0f));
            rect.size = size;
            rect.depth = 0.2f;

            auto& sprite = e.AddComponent<Canis::Sprite2DComponent>();
            sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(GetAssetManager().LoadTexture("assets/textures/enemies/let_it_bee.png"))->GetTexture();

            auto& color = e.AddComponent<Canis::ColorComponent>();
            color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

            auto& anim = e.AddComponent<Canis::SpriteAnimationComponent>();
            anim.animationId = _animId;

            auto& circleCollider = e.AddComponent<Canis::CircleColliderComponent>();
            circleCollider.layer = Canis::BIT::ZERO;
            circleCollider.mask = Canis::BIT::TWO;
            circleCollider.radius = size.x / 2.0f;
        }
    }

    Canis::Entity FindRandomEnemyOnScreen()
    {
        entt::entity enttSelection;
        Canis::Entity canisSelection;
        Canis::Camera2DComponent cameraComponent = camera.GetComponent<Canis::Camera2DComponent>();

        canisSelection.scene = m_Entity.scene;

        Canis::CollisionSystem2D* collisionSystem = GetSystem<Canis::CollisionSystem2D>();
        std::vector<entt::entity> results = collisionSystem->BoxCast(glm::vec2(cameraComponent.position.x, cameraComponent.position.y), glm::vec2(GetWindow().GetScreenWidth(), GetWindow().GetScreenHeight()), glm::vec2(0.0f), 0.0f, Canis::BIT::TWO);
        
        if (results.size() > 0)
        {
            enttSelection = results[rand() % results.size()];
            if (enttSelection != entt::tombstone && m_Entity.scene->entityRegistry.valid(enttSelection))
            {
                canisSelection.entityHandle = enttSelection;
                return canisSelection;
            }
        }

        return canisSelection;
    }

    Canis::Entity FindClosestEnemy()
    {
        std::vector<entt::entity> hits = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
        Canis::Entity closestEntity;
        Canis::Entity hitEntity;
        hitEntity.scene = m_Entity.scene;
        closestEntity.scene = m_Entity.scene;
        glm::vec2 playerPostion = GetComponent<Canis::RectTransformComponent>().position;

        float closestDistance = 100000000.0f;

        for(int i = 0; i < hits.size(); i++)
        {
            hitEntity.entityHandle = hits[i];
            if (hits[i] != entt::tombstone && m_Entity.scene->entityRegistry.valid(hits[i]))
            {
                float distance = glm::distance(hitEntity.GetComponent<Canis::RectTransformComponent>().position, playerPostion);

                if(distance <= closestDistance)
                {
                    closestDistance = distance;
                    closestEntity = hitEntity;
                }
            }
        }

        return closestEntity;
    }
};