#pragma once
#include <vector>
#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/Camera2DComponent.hpp>

#include "../Components/BoidComponent.hpp"

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

    }

    void OnReady()
    {
        m_camera = m_Entity.GetEntityWithTag("Camera");
    }

    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        if (GetInputManager().JustPressedKey(SDLK_h))
        {
            auto e = CreateEntity();
            SpawnEnemy(e);
        }
    }

    void SpawnEnemy(Canis::Entity _entity)
    {
        auto& m_cameraComponent = m_camera.GetComponent<Canis::Camera2DComponent>();

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
            GetAssetManager().LoadTexture("assets/textures/enemies/beehive.png"))->GetTexture();
            
        auto& color = _entity.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        auto& boid = _entity.AddComponent<BoidComponent>();
            boid.drag = 0.09f;
            boid.speed = 10.0f;
            boid.maxSpeed = 20.0f;

        //eric splain morrow
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<EnemyMovement>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
    }

    void SpawnEnemies(std::vector<Canis::Entity> _entities)
    {
        auto& m_cameraComponent = m_camera.GetComponent<Canis::Camera2DComponent>();

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

        for (auto e : _entities)
        {
            auto& rectTransform = e.AddComponent<Canis::RectTransformComponent>();
            rectTransform.size = glm::vec2(100.0, 100.0);
            rectTransform.position = GetRandomPosition(m_spawnPositions);
            
            auto& sprite = e.AddComponent<Canis::Sprite2DComponent>();
            sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(
                GetAssetManager().LoadTexture("assets/textures/enemies/beehive.png"))->GetTexture();
            
            auto& color = e.AddComponent<Canis::ColorComponent>();
            color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

            auto& boid = e.AddComponent<BoidComponent>();
            boid.drag = 0.09f;
            boid.speed = 10.0f;
            boid.maxSpeed = 20.0f;
        }

        
    }
};