#pragma once
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>

class SeedPickup : public Canis::ScriptableEntity
{
private:
    Canis::Entity player = m_Entity.GetEntityWithTag("Player");
    bool pickedUp = false;

public:
    bool hasSeed = false;
    bool seedOnGround = false;

    void OnCreate()
    {
    }

    void OnReady()
    {
    }

    void OnDestroy()
    {
    }

    void OnUpdate(float _dt)
    {
        if (!hasSeed && !seedOnGround)
        {
            std::vector<entt::entity> hits = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
            Canis::Entity hitEntity;
            hitEntity.scene = m_Entity.scene;

            for (int i = 0; i < hits.size(); i++)
            {
                hitEntity.entityHandle = hits[i];
                Pickup();
                pickedUp = true;
                hasSeed = true;
                break;
            }
        }
    }

    void Pickup()
    {
        GetComponent<Canis::RectTransformComponent>().position =
            glm::vec2(player.GetComponent<Canis::RectTransformComponent>().position.x,
                      player.GetComponent<Canis::RectTransformComponent>().position.y + 0.5f);
    }
};

bool DecodeSeedPickup(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "SeedPickup")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<SeedPickup>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}