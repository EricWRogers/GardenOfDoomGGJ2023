#pragma once
class SwordWeapon : public Weapon
{
private:
    float timer = 0.0f;
    bool canShoot = true;
    int swordId = 0;

public:
    void OnCreate()
    {
        Weapon::OnCreate();
        swordId = GetAssetManager().LoadSpriteAnimation("assets/animations/sword_weapon.anim");
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            8.0f,            // damage
            glm::vec2(16.0f), // weapon effect size
            150.0f,           // speed
            3.0f,             // duration
            1,                // amount
            0.8f              // cooldown
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
        if (timer <= 0)
        {
            canShoot = true;
        }

        if (canShoot)
        {
            Shoot(
                ((PlayerManager *)player.GetComponent<Canis::ScriptComponent>().Instance)->GetInputDirection(),
                player.GetComponent<Canis::RectTransformComponent>().position,
                swordId);

            timer = cooldown;
            canShoot = false;
        }
    }
};

bool DecodeSwordWeapon(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "SwordWeapon")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<SwordWeapon>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}