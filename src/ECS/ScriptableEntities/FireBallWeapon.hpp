#pragma once
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>

class FireBallWeapon : public Weapon
{
    private:
    float timer = 0.0f;
    bool canShoot = true;
    int fireballId = 0;
    Canis::Entity closestEnemy;

    public:
    void OnCreate()
    {
        Weapon::OnCreate();
        fireballId = GetAssetManager().LoadSpriteAnimation("assets/animations/fireball_weapon.anim");
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            15.0f,              //damage
            glm::vec2(16.0f),   //weapon effect size
            120.0f,             //speed
            6.0f,               //duration
            1,                  //amount
            0.5f                //cooldown
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

        closestEnemy = FindClosestEnemy();

        timer -= _dt;
        if(timer <= 0)
        {
            canShoot = true;
        }

        if (canShoot)
        {
            //if(closestEnemy.entityHandle != entt::null)
            //{
                Weapon::Shoot(
                    glm::normalize(closestEnemy.GetComponent<Canis::RectTransformComponent>().position - 
                    GetComponent<Canis::RectTransformComponent>().position), 
                    player.GetComponent<Canis::RectTransformComponent>().position, 
                    fireballId);
            //}

            timer = cooldown;
            canShoot = false;
        }
    }
};