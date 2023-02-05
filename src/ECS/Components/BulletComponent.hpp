#pragma once
#include <glm/glm.hpp>

struct BulletComponent
{
    glm::vec2 direction = glm::vec2(0.0f);
    float speed = 0.0f;
    float damage = 0.0f;
    float timeLeft = 1.0f;
};