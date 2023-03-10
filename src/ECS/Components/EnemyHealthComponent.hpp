#pragma once

struct EnemyHealthComponent
{
    float currentHealth = 0.0f;
    float maxHealth = 0.0f;
    bool tookDamage = false;
};

namespace EnemyHealth
{
    inline void DamageEnemy(EnemyHealthComponent &health, const float &damage)
    {
        health.tookDamage = true;
        health.currentHealth -= damage;
    }
}