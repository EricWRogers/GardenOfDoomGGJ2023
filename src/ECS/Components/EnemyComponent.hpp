#pragma once

const float ENEMYHITTIME = 0.1f;

struct EnemyComponent
{
    float attackCooldown;
    float step = 0.0;
    float attackDamage;
    float xpValue = 0.0f;
    bool wasFlipX = false;
    float hitCountDown = 0.0f;
};