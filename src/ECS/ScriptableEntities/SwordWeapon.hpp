#pragma once
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>

class SwordWeapon : public Weapon
{
    private:
    float timer = 0.0f;
    bool canShoot = true;
    int swordId = 0;

    public:
    void OnCreate()
    {
        swordId = GetAssetManager().LoadSpriteAnimation("assets/animations/sword_weapon.anim");
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            15.0f,              //damage
            glm::vec2(16.0f),   //weapon effect size
            160.0f,             //speed
            6.0f,               //duration
            1,                  //amount
            0.4f                //cooldown
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
        
        GetComponent<Canis::RectTransformComponent>().position = player.GetComponent<Canis::RectTransformComponent>().position;

        timer -= _dt;
        if(timer <= 0)
        {
            canShoot = true;
        }

        if (canShoot)
        {
            if (!player.GetComponent<Canis::SpriteAnimationComponent>().flipX)
            {
                Weapon::Shoot(
                    glm::vec2(1.0f, 0.0f), 
                    player.GetComponent<Canis::RectTransformComponent>().position, 
                    swordId);
            }

            if (player.GetComponent<Canis::SpriteAnimationComponent>().flipX)
            {
                Weapon::Shoot(
                    glm::vec2(-1.0f, 0.0f), 
                    player.GetComponent<Canis::RectTransformComponent>().position, 
                    swordId);
            }

            timer = cooldown;
            canShoot = false;
        }
    }
};