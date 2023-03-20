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
    std::vector<glm::vec2> m_spawnPositions = {};
    Canis::Entity m_camera;
    int m_buffer = 5.0;
    int m_borderTiles = 20;

    glm::vec2 GetRandomPosition(std::vector<glm::vec2> &_positions)
    {
        int i = rand() % _positions.size();
        glm::vec2 v = _positions[i];
        _positions.erase(_positions.begin() + i);
        return v;
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
        UpdateSpawnPositions();
    }

    void UpdateSpawnPositions()
    {
        m_spawnPositions.clear();
        auto& m_cameraComponent = m_camera.GetComponent<Canis::Camera2DComponent>();

        for (float x = m_cameraComponent.position.x - GetWindow().GetScreenWidth()/2.0f - (m_borderTiles * 32.0f); x < GetWindow().GetScreenWidth() + m_cameraComponent.position.x + (m_borderTiles * 32.0f); x+=32.0f)
            for (float y = m_cameraComponent.position.y - GetWindow().GetScreenHeight()/2.0f - (m_borderTiles * 32.0f); y < GetWindow().GetScreenHeight() + m_cameraComponent.position.y + (m_borderTiles * 32.0f); y+=32.0f)
                if((x > (m_cameraComponent.position.x + (GetWindow().GetScreenWidth()/2.0f)) ||
                   x < (m_cameraComponent.position.x - (GetWindow().GetScreenWidth()/2.0f))  ||
                   y < (m_cameraComponent.position.y - (GetWindow().GetScreenHeight()/2.0f)) ||
                   y > (m_cameraComponent.position.y + (GetWindow().GetScreenHeight()/2.0f))))
                    m_spawnPositions.push_back(glm::vec2(x,y));
    }

    void SpawnEnemy(const std::string &_texPath, const std::string &_animPath, float _xpValue, float _maxHealth)
    {
        Canis::Entity _entity = CreateEntity();

        if (m_spawnPositions.size() == 0)
        {
            UpdateSpawnPositions();
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
        boid.speed = 50.0f;
        boid.maxSpeed = 100.0f;
        
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
        enemy.xpValue = Canis::RandomFloat(0.0f, _xpValue);

        auto& anim = _entity.AddComponent<Canis::SpriteAnimationComponent>();
        anim.animationId = GetAssetManager().LoadSpriteAnimation(_animPath);
        anim.index = 0;
    }
};

bool DecodeEnemySpawnManager(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "EnemySpawnManager")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<EnemySpawnManager>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}