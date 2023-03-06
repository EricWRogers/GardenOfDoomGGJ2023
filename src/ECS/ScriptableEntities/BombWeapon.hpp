#pragma once
#include <string>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/Math.hpp>
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include "Weapon.hpp"

class BombWeapon : public Weapon
{
    private:
    float timer = 0.0f;
    bool canDrop = true;
    int bombId = 0;

    public:
    void OnCreate()
    {
        Weapon::OnCreate();
        bombId = GetAssetManager().LoadSpriteAnimation("assets/animations/bomb_weapon.anim");
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            30.0f,              //damage
            glm::vec2(64.0f),   //weapon effect size
            0.0f,               //speed
            2.0f,               //duration
            1,                  //amount
            0.8f                //cooldown
        );
    }

    void OnDestroy()
    {
        Weapon::OnDestroy();
    }

    void OnUpdate(float _dt)
    {
        if (GetComponent<Canis::RectTransformComponent>().active == false) // add to all weapons
            return;
        
        Weapon::OnUpdate(_dt);
        
        GetComponent<Canis::RectTransformComponent>().position = player.GetComponent<Canis::RectTransformComponent>().position;

        timer -= _dt;
        if(timer <= 0)
        {
            canDrop = true;
        }

        if (canDrop)
        {
            for (int i = 0; i < amount; i++)
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
        
        auto e = CreateEntity();

        auto& rect = e.AddComponent<Canis::RectTransformComponent>();
        rect.position = hitEntity.GetComponent<Canis::RectTransformComponent>().position;
        rect.size = glm::vec2(32.0f);
        rect.depth = 0.2f;

        auto& sprite = e.AddComponent<Canis::Sprite2DComponent>();
        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(GetAssetManager().LoadTexture("assets/textures/enemies/let_it_bee.png"))->GetTexture();

        auto& color = e.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        auto& anim = e.AddComponent<Canis::SpriteAnimationComponent>();
        anim.animationId = bombId;

        auto& bomb = e.AddComponent<BombComponent>();
        bomb.damage = damage;
        bomb.timeLeft = 1.5f;
    }
};