#pragma once

class HandOfGodWeapon : public Weapon
{
private:
    float timer = 0.0f;
    bool canDrop = true;
    int handId = 0;

public:
    void OnCreate()
    {
        handId = GetAssetManager().LoadSpriteAnimation("assets/animations/hand_of_god.anim");
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            10.0f,            // damage
            glm::vec2(64.0f), // weapon effect size
            0.0f,             // speed
            0.0f,             // duration
            1,                // amount
            1.2f              // cooldown
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
        if (timer <= 0)
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

        auto &rect = e.AddComponent<Canis::RectTransformComponent>();
        rect.position = hitEntity.GetComponent<Canis::RectTransformComponent>().position;
        rect.size = size;
        rect.depth = 0.2f;

        auto &sprite = e.AddComponent<Canis::Sprite2DComponent>();
        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(GetAssetManager().LoadTexture("assets/textures/enemies/let_it_bee.png"))->GetTexture();

        auto &color = e.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        auto &anim = e.AddComponent<Canis::SpriteAnimationComponent>();
        anim.animationId = handId;

        auto &circleCollider = e.AddComponent<Canis::CircleColliderComponent>();
        circleCollider.layer = Canis::BIT::ZERO;
        circleCollider.mask = Canis::BIT::TWO;
        circleCollider.radius = size.x / 2.0f;

        auto &hand = e.AddComponent<HandOfGodComponent>();
        hand.damage = damage;
        hand.timeLeft = 1.2f;
    }
};

bool DecodeHandOfGodWeapon(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "HandOfGodWeapon")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<HandOfGodWeapon>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}