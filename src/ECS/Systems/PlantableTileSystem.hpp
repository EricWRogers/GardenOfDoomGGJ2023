#pragma once
#include <functional>
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>
#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/ScriptComponent.hpp>
#include <Canis/Scene.hpp>
#include <Canis/External/entt.hpp>
#include "../Components/PlantableGroundComponent.hpp"
#include "../ScriptableEntities/PlayerManager.hpp"
#include <Canis/ECS/Components/Sprite2DComponent.hpp>

class PlantableTileSystem : public Canis::System
{
    private:
    Canis::Entity m_player;
    Canis::CollisionSystem2D* colSys = nullptr;
    bool uiEnabled = false;
    float firstStageTime = 0.0f;
    float secondStageTime = 0.0f;
    float currentTime = 0.0f;
    bool doneWithFirstStage = false;
    bool seedGrown = false;

    bool Timer(float deltaTime, float timeToWait, std::function<void()> callOnComplete)
    {//Eric show me how to function pointers
        float currentTime = 0.0f;

        currentTime += deltaTime;

        if (currentTime >= timeToWait)
        {
            return true;
            callOnComplete();
        }
        else return false;
    }

    void GrowToFirst()
    {

    }

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
        uiEnabled = false;
        auto view = _registry.view<Canis::RectTransformComponent, PlantableGroundComponent, Canis::CircleColliderComponent, Canis::Sprite2DComponent>();
        for (auto [entity, rect, tile, col, sprite] : view.each())
        {
            if (seedGrown)
            {
                std::vector<entt::entity> hits = colSys->GetHits(entity);
                Canis::Entity hitEntity;
                hitEntity.scene = scene;

                if (hits.size() != 0)
                {
                    if (GetInputManager().GetKey(SDL_SCANCODE_F))
                    {
                        seedGrown = false;
                        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(
                            GetAssetManager().LoadTexture("assets/textures/environment/background_sprite_sheet.png"))->GetTexture();
                        Canis::GetSpriteFromTextureAtlas(sprite, 0, 0, 3, 1, 16, 16, false, false);

                        //Reward menu here
                    }
                }
            }

            if (((PlayerManager*)m_player.GetComponent<Canis::ScriptComponent>().Instance)->holdingSeed)
            {
                std::vector<entt::entity> hits = colSys->GetHits(entity);
                Canis::Entity hitEntity;
                hitEntity.scene = scene;

                if (hits.size() != 0)
                {
                    uiEnabled = true;

                    if (GetInputManager().GetKey(SDL_SCANCODE_F))
                    {
                        firstStageTime = ((PlayerManager*)m_player.GetComponent<Canis::ScriptComponent>().Instance)->seed.GetComponent<SeedComponent>().timeToFirstStage;
                        secondStageTime = ((PlayerManager*)m_player.GetComponent<Canis::ScriptComponent>().Instance)->seed.GetComponent<SeedComponent>().timeToSecondStage;
                        scene->GetSystem<EnemySystem>()->DestroySeed();
                        tile.occupied = true;
                        Canis::GetSpriteFromTextureAtlas(sprite, 0, 0, 3, 0, 16, 16, (0 == rand() % 2), (0 == rand() % 2));
                    }
                }
            }

            if (tile.occupied)
            {
                if (!doneWithFirstStage)
                {
                    currentTime += _deltaTime;

                    if (currentTime >= firstStageTime)
                    {
                        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(
                        GetAssetManager().LoadTexture("assets/textures/environment/border_environment_sprite_sheet.png"))->GetTexture();
                        Canis::GetSpriteFromTextureAtlas(sprite, 0, 0, 0, 0, 16, 16, false, false);
                        doneWithFirstStage = true;
                        currentTime = 0.0f;
                    }
                }

                if (doneWithFirstStage)
                {
                    currentTime += _deltaTime;

                    if (currentTime >= secondStageTime)
                    {
                        Canis::GetSpriteFromTextureAtlas(sprite, 0, 0, 1, 0, 16, 16, false, false);
                        currentTime = 0.0f;
                        doneWithFirstStage = false;
                        tile.occupied = false;
                        seedGrown = true;
                    }
                }
            }
        }

        if (uiEnabled)
        {
            //UI Here
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