#pragma once
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include "../Components/HandOfGodComponent.hpp"

class HandOfGodWeapon : public Weapon
{
    private:
    float timer = 0.0f;
    bool canDrop = true;
    int handId = 0;
    Canis::Camera2DComponent camera;

    public:
    void OnCreate()
    {
        handId = GetAssetManager().LoadSpriteAnimation("assets/animations/hand_of_god.anim");
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            30.0f,              //damage
            glm::vec2(64.0f),   //weapon effect size
            0.0f,               //speed
            0.0f,                  //duration
            1,                  //amount
            0.8f                //cooldown
        );
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        if (GetComponent<Canis::RectTransformComponent>().active == false) // add to all weapons
            return;
        
        Weapon::OnUpdate(_dt);

        camera = m_Entity.GetEntityWithTag("Camera").GetComponent<Canis::Camera2DComponent>();
        
        GetComponent<Canis::RectTransformComponent>().position = player.GetComponent<Canis::RectTransformComponent>().position;

        timer -= _dt;
        if(timer <= 0)
        {
            canDrop = true;
        }

        if (canDrop)
        {
            DropProjectile();

            timer = cooldown;
            canDrop = false;
        }
    }
    
    void DropProjectile()
    {
        Canis::Entity hitEntity = FindRandomEnemyOnScreen();

        if (hitEntity.entityHandle == entt::null)
            return;
        
        hitEntity.GetComponent<Canis::ColorComponent>().color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        
        auto e = CreateEntity();

        auto& rect = e.AddComponent<Canis::RectTransformComponent>();
        rect.position = hitEntity.GetComponent<Canis::RectTransformComponent>().position;
        rect.size = size;
        rect.depth = 0.2f;

        auto& sprite = e.AddComponent<Canis::Sprite2DComponent>();
        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(GetAssetManager().LoadTexture("assets/textures/enemies/let_it_bee.png"))->GetTexture();

        auto& color = e.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        auto& anim = e.AddComponent<Canis::SpriteAnimationComponent>();
        anim.animationId = handId;

        auto& circleCollider = e.AddComponent<Canis::CircleColliderComponent>();
        circleCollider.layer = Canis::BIT::ZERO;
        circleCollider.mask = Canis::BIT::TWO;
        circleCollider.radius = size.x / 2.0f;

        auto& hand = e.AddComponent<HandOfGodComponent>();
        hand.damage = damage;
        hand.timeLeft = 1.2f;
    }
};