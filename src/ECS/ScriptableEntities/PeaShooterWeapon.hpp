#pragma once
#include <Canis/ECS/Components/CircleColliderComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>

class PeaShooterWeapon : public Weapon
{
    private:
    float timer = 0.0f;
    bool canShoot = true;
    int peaId = 0;
    Canis::Entity closestEntity;

    public:
    void OnCreate()
    {
        peaId = GetAssetManager().LoadSpriteAnimation("assets/animations/pea_shooter_projectile.anim");
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            10.0f,              //damage
            glm::vec2(16.0f),   //weapon effect size
            150.0f,             //speed
            6.0f,               //duration
            1,                  //amount
            0.3f                //cooldown
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

        closestEntity = Weapon::FindClosestEnemy();

        timer -= _dt;
        if(timer <= 0)
        {
            canShoot = true;
        }

        if (canShoot)
        {
            if(closestEntity.entityHandle != entt::null && closestEntity.entityHandle != entt::tombstone && m_Entity.scene->entityRegistry.valid(closestEntity.entityHandle))
            {
                Weapon::Shoot(
                    glm::normalize(closestEntity.GetComponent<Canis::RectTransformComponent>().position - 
                    GetComponent<Canis::RectTransformComponent>().position), 
                    player.GetComponent<Canis::RectTransformComponent>().position, 
                    peaId);
            }

            timer = cooldown;
            canShoot = false;
        }
    }
};

bool DecodePeaShooterWeapon(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "PeaShooterWeapon")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<PeaShooterWeapon>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}