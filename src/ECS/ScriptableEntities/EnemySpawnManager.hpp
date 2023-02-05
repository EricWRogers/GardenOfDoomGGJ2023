#pragma once
#include <vector>
#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/Camera2DComponent.hpp>
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include "../Components/EnemyHealthComponent.hpp"

#include "../Components/BoidComponent.hpp"
#include "../Components/EnemyComponent.hpp"

class EnemyMovement;

class EnemySpawnManager : public Canis::ScriptableEntity
{
    private:
    std::vector<glm::vec2> m_spawnPositions;
    Canis::Entity m_camera;
    int m_buffer = 5.0;

    glm::vec2 GetRandomPosition(std::vector<glm::vec2> _positions)
    {
        return _positions[rand() % _positions.size()];
    }

    public:
    void OnCreate()
    {
        m_camera = m_Entity.GetEntityWithTag("Camera");
    }

    void OnReady()
    {
        
    }

    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {

    }

    void SpawnEnemy(const std::string &_texPath, const std::string &_animPath, float _xpValue, float _maxHealth)
    {
        auto& m_cameraComponent = m_camera.GetComponent<Canis::Camera2DComponent>();
        Canis::Entity _entity = CreateEntity();

        for (int i = m_cameraComponent.position.x - GetWindow().GetScreenWidth()/2.0; i < GetWindow().GetScreenWidth() + m_cameraComponent.position.x; i++)
        {
            m_spawnPositions.push_back(glm::vec2(i, m_cameraComponent.position.y + GetWindow().GetScreenHeight()/2.0));
        }

        for (int i = m_cameraComponent.position.x - GetWindow().GetScreenWidth()/2.0; i < GetWindow().GetScreenWidth() + m_cameraComponent.position.x; i++)
        {
            m_spawnPositions.push_back(glm::vec2(i, m_cameraComponent.position.y - GetWindow().GetScreenHeight()/2.0));
        }

        for (int i = m_cameraComponent.position.y - GetWindow().GetScreenHeight()/2.0; i < GetWindow().GetScreenHeight() + m_cameraComponent.position.y; i++)
        {
            m_spawnPositions.push_back(glm::vec2(m_cameraComponent.position.x + GetWindow().GetScreenWidth()/2.0, i));
        }

        for (int i = m_cameraComponent.position.y - GetWindow().GetScreenHeight()/2.0; i < GetWindow().GetScreenHeight() + m_cameraComponent.position.y; i++)
        {
            m_spawnPositions.push_back(glm::vec2(m_cameraComponent.position.x - GetWindow().GetScreenWidth()/2.0, i));
        }

        auto& rectTransform = _entity.AddComponent<Canis::RectTransformComponent>();
        rectTransform.size = glm::vec2(32.0, 32.0);
        rectTransform.position = GetRandomPosition(m_spawnPositions);
            
        auto& sprite = _entity.AddComponent<Canis::Sprite2DComponent>();
        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>( 
            GetAssetManager().LoadTexture(_texPath))->GetTexture();
            
        auto& color = _entity.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        auto& boid = _entity.AddComponent<BoidComponent>();
        boid.drag = 0.95f;
        boid.speed = 2000.0f;
        boid.maxSpeed = 10000.0f;
        
        auto& collider = _entity.AddComponent<Canis::CircleColliderComponent>();
        collider.layer = Canis::BIT::TWO;
        collider.mask = Canis::BIT::ONE;
        collider.radius = 16.0;
        collider.center = glm::vec2(0.0);

        auto& health = _entity.AddComponent<EnemyHealthComponent>();
        health.maxHealth = _maxHealth;
        health.currentHealth = health.maxHealth;

        auto& enemy = _entity.AddComponent<EnemyComponent>();
        enemy.attackCooldown = 1.0;
        enemy.attackDamage = 1.0;
        enemy.xpValue = _xpValue;

        auto& anim = _entity.AddComponent<Canis::SpriteAnimationComponent>();
        anim.animationId = GetAssetManager().LoadSpriteAnimation(_animPath);
        anim.index = 0;
    }
};