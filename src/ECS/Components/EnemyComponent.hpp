#pragma once

struct EnemyComponent
{
    float attackCooldown;
    float step = 0.0;
    float attackDamage;
    float xpValue = 0.0f;
};