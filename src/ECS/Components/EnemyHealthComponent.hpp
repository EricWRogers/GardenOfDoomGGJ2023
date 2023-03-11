#pragma once

struct EnemyHealthComponent
{
    float currentHealth = 0.0f;
    float maxHealth = 0.0f;
    unsigned short tookThisFrame = 0;
};

namespace EnemyHealth
{
    inline void DamageEnemy(EnemyHealthComponent &_health, const float &_damage)
    {
        _health.tookThisFrame += _damage;
        _health.currentHealth -= _damage;
    }
}