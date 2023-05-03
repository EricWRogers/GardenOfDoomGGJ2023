#pragma once

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
            10.0f,            // damage
            glm::vec2(16.0f), // weapon effect size
            100.0f,           // speed
            3.0f,             // duration
            1,                // amount
            1.4f              // cooldown
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
        if (timer <= 0)
        {
            canShoot = true;
        }

        if (canShoot)
        {
            // if(closestEnemy.entityHandle != entt::null)
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

bool DecodeFireBallWeapon(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "FireBallWeapon")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<FireBallWeapon>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}