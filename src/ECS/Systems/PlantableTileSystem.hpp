#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/ScriptComponent.hpp>
#include <Canis/Scene.hpp>
#include <Canis/External/entt.hpp>
#include "../Components/PlantableGroundComponent.hpp"
#include "../ScriptableEntities/PlayerManager.hpp"

class PlantableTileSystem : public Canis::System
{
    private:
    Canis::Entity m_player;
    Canis::CollisionSystem2D* colSys = nullptr;

    public:
    void Create() {}

    void Ready() 
    {
        colSys = scene->GetSystem<Canis::CollisionSystem2D>();
        m_player.scene = scene;
        m_player = m_player.GetEntityWithTag("Player");
    }

    void Update(entt::registry &_registry, const float _deltaTime) 
    {
        auto view = _registry.view<Canis::RectTransformComponent, PlantableGroundComponent, Canis::CircleColliderComponent>();
        for (auto [entity, rect, tile, col] : view.each())
        {
            if (((PlayerManager*)m_player.GetComponent<Canis::ScriptComponent>().Instance)->holdingSeed)
            {
                std::vector<entt::entity> hits = colSys->GetHits(entity);
                Canis::Entity hitEntity;
                hitEntity.scene = scene;

                for(entt::entity hit : hits)
                {
                    hitEntity.entityHandle = hit;
                    
                    if (GetInputManager().GetKey(SDL_SCANCODE_F))
                    {
                        Canis::Log("Destroying");
                        ((PlayerManager*)m_player.GetComponent<Canis::ScriptComponent>().Instance)->DestroySeed();
                    }
                }
            }
        }
    }
};

bool DecodePlantableTileSystem(const std::string &_name, Canis::Scene *_scene) {
    if(_name == "PlantableTileSystem"){
        _scene->CreateSystem<PlantableTileSystem>();
        return true;
    }
    return false;
}