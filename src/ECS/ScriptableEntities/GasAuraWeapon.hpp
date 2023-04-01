#pragma once

class GasAuraWeapon : public Weapon
{
private:
    float timer = 0.0f;
    bool canDamage = true;

public:
    void OnCreate()
    {
        Weapon::OnCreate();
    }

    void OnReady()
    {
        Weapon::OnReady();
        Weapon::SetBaseStats(
            20.0f,             // damage
            glm::vec2(128.0f), // weapon effect size
            0.0f,              // speed
            0.0f,              // duration
            1,                 // amount
            0.3f               // cooldown
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

        GetComponent<Canis::CircleColliderComponent>().radius = size.x / 2.0f;
        GetComponent<Canis::RectTransformComponent>().size = size;

        GetComponent<Canis::RectTransformComponent>().position = player.GetComponent<Canis::RectTransformComponent>().position;

        timer -= _dt;
        if (timer <= 0)
        {
            canDamage = true;
        }

        if (canDamage)
        {
            std::vector<entt::entity> hits = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
            Canis::Entity hitEntity;
            hitEntity.scene = m_Entity.scene;

            for (int i = 0; i < hits.size(); i++)
            {
                hitEntity.entityHandle = hits[i];

                EnemyHealth::DamageEnemy(hitEntity.GetComponent<EnemyHealthComponent>(), damage);
            }

            timer = cooldown;
            canDamage = false;
        }
    }
};

bool DecodeGasAuraWeapon(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "GasAuraWeapon")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<GasAuraWeapon>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}