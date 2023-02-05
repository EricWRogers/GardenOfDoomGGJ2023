#pragma once
#include <glm/glm.hpp>

struct Bullet
{
    glm::vec2 velocity = glm::vec2(0.0f);
    glm::vec2 acceleration = glm::vec2(0.0f);
    float drag = 0.0f;
    float speed = 0.0f;
    float maxSpeed = 0.0f;

    
};

