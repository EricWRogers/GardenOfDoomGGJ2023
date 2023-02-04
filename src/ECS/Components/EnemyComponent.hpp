#pragma once

struct EnemyComponent
{
    float attackCooldown;
    float step = 0.0;
    float attackDamage;
};