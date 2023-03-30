#include "BoidSystem.hpp"
#include "../Components/BoidComponent.hpp"

#include <Canis/Scene.hpp>

#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include <Canis/ECS/Components/Camera2DComponent.hpp>

void BoidSystem::Ready()
{
    m_player.scene = scene;
    m_player = m_player.GetEntityWithTag("Player");
}

void BoidSystem::Update(entt::registry &registry, const float _deltaTime)
{
    glm::vec2 playerPosition = m_player.GetComponent<Canis::RectTransformComponent>().position;

    auto cam = registry.view<const Canis::Camera2DComponent>();

    float cameraSize;
    for (auto [entity, camera2D] : cam.each())
    {
        cameraPosition = camera2D.position;
        cameraSize = camera2D.scale;
    }

    float boidCount = 0;
    auto view = registry.view<Canis::RectTransformComponent, BoidComponent>();

    glm::vec2 seekTarget, alignmentTarget, cohesionTarget, separationTarget = glm::vec2(0.0f);

    glm::vec2 alignment = glm::vec2(0.0f);
    glm::vec2 cohesion = glm::vec2(0.0f);
    glm::vec2 separation = glm::vec2(0.0f);
    glm::vec2 acceleration;
    glm::vec4 weights = glm::vec4(USER_BEHAVIOR_WEIGHT, ALIGNMENT_WEIGHT, COHESION_WEIGHT, SEPARATION_WEIGHT);

    int alignNumNeighbors = 0;
    int cohNumNeighbors = 0;
    int sepNumNeighbors = 0;

    float distance = 0.0f;

    for (auto [entity, rect_transform, boid] : view.each())
    {
        alignment = glm::vec2(0.0f);
        cohesion = glm::vec2(0.0f);
        separation = glm::vec2(0.0f);
        alignNumNeighbors = 0;
        cohNumNeighbors = 0;

        for (auto [other_entity, other_rect_transform, other_boid] : view.each())
        {
            if (entity != other_entity)
            {
                distance = glm::distance(rect_transform.position, other_rect_transform.position);
                if (distance <= MAX_COHESION_DISTANCE)
                {
                    cohNumNeighbors++;
                    cohesion += other_rect_transform.position;

                    if (distance <= MAX_ALIGNMENT_DISTANCE)
                    {
                        alignNumNeighbors++;
                        alignment += other_boid.velocity;

                        if (distance <= MAX_SEPARATION_DISTANCE)
                        {
                            separation += (rect_transform.position - other_rect_transform.position);
                        }
                    }
                }
            }
        }

        // Seek
        seekTarget = glm::normalize(playerPosition - rect_transform.position);
        // Alignment
        alignmentTarget = (alignment != glm::vec2(0.0f)) ? glm::normalize(alignment / (alignNumNeighbors + 0.0f)) : glm::vec2(0.0f);
        // Cohesion
        cohesionTarget = (cohNumNeighbors > 0) ? glm::normalize((cohesion / static_cast<float>(cohNumNeighbors)) - rect_transform.position) : glm::vec2(0.0f);
        // Separation
        separationTarget = (separation != glm::vec2(0.0f)) ? glm::normalize(separation) : glm::vec2(0.0f);

        acceleration.x = glm::dot(glm::vec4(seekTarget.x, alignmentTarget.x, cohesionTarget.x, separationTarget.x), weights);
        acceleration.y = glm::dot(glm::vec4(seekTarget.y, alignmentTarget.y, cohesionTarget.y, separationTarget.y), weights);
        acceleration *= boid.speed;
        /*acceleration = ((seekTarget * USER_BEHAVIOR_WEIGHT) +
                        (alignmentTarget * ALIGNMENT_WEIGHT) +
                        (cohesionTarget * COHESION_WEIGHT) +
                        (separationTarget * SEPARATION_WEIGHT)) *
                       boid.speed;*/

        // update velocity
        boid.velocity += acceleration;

        // clamp velocity to maxSpeed
        if (glm::length(boid.velocity) > boid.maxSpeed)
        {
            boid.velocity = glm::normalize(boid.velocity) * boid.maxSpeed;
        }

        // apply drag
        boid.velocity *= boid.drag;

        // update position
        rect_transform.position += boid.velocity * _deltaTime;
    }
}

bool DecodeBoidSystem(const std::string &_name, Canis::Scene *_scene) {
    if(_name == "BoidSystem"){
        _scene->CreateSystem<BoidSystem>();
        return true;
    }
    return false;
}