#pragma once
#include <Canis/External/entt.hpp>
#include <Canis/ECS/Systems/System.hpp>

#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include <Canis/ECS/Components/Camera2DComponent.hpp>

#include "../Components/BoidComponent.hpp"

const float MAX_ALIGNMENT_DISTANCE = 40.0f;
const float MAX_COHESION_DISTANCE = 50.0f;
const float MAX_SEPARATION_DISTANCE = 32.0f;

const float WANDER_CIRCLE_OFFSET = 50.0f;
const float WANDER_CIRCLE_RADIUS = 30.0f;
const float WANDER_ANGLE_DELTA_MAX = 2.0f;

const float USER_BEHAVIOR_WEIGHT = 0.3f;
const float SEPARATION_WEIGHT = 1.0f;
const float ALIGNMENT_WEIGHT = 0.0f;//0.3f;
const float COHESION_WEIGHT = 0.0f;//0.15f;

class BoidSystem : public Canis::System
{
private:
    glm::vec2 Seek(const glm::vec2 &agentPosition, const glm::vec2 &targetPosition)
    {
        glm::vec2 seek = targetPosition - agentPosition;
        return glm::normalize(seek);
    }

    glm::vec2 Flee(const glm::vec2 &agentPosition, const  glm::vec2 &targetPosition)
    {
        glm::vec2 seek = targetPosition - agentPosition;
        return glm::normalize(seek);
    }

    glm::vec2 Alignment(const entt::entity &agent, const glm::vec2 &agentPosition, entt::registry &_registry)
    {
        glm::vec2 alignment = glm::vec2(0.0f);
        int numNeighbors = 0;

        auto view = _registry.view<Canis::RectTransformComponent, Canis::ColorComponent, Canis::Sprite2DComponent, BoidComponent>();
        for (auto [entity, rect_transform, color, sprite, boid] : view.each())
        {
            if (agent != entity)
            {
                numNeighbors++;
                if (glm::distance(agentPosition, rect_transform.position) <= MAX_ALIGNMENT_DISTANCE)
                {
                    alignment += boid.velocity;
                }
            }
        }

        if (alignment != glm::vec2(0.0f))
        {
            return glm::normalize(alignment / (numNeighbors + 0.0f));
        }

        return glm::vec2(0.0f);
    }

    glm::vec2 Cohesion(const entt::entity &agent, const glm::vec2 &agentPosition, entt::registry &_registry)
    {
        glm::vec2 cohesion = glm::vec2(0.0f);
        int numNeighbors = 0;

        auto view = _registry.view<Canis::RectTransformComponent, Canis::ColorComponent, Canis::Sprite2DComponent, BoidComponent>();
        for (auto [entity, rect_transform, color, sprite, boid] : view.each())
        {
            if (agent != entity)
            {
                numNeighbors++;
                if (glm::distance(agentPosition, rect_transform.position) <= MAX_COHESION_DISTANCE)
                {
                    cohesion += rect_transform.position;
                }
            }
        }

        if (numNeighbors == 0)
            return glm::vec2(0.0f);

        cohesion /= numNeighbors;

        return glm::normalize((cohesion - agentPosition));
    }

    glm::vec2 Separation(const entt::entity &agent, const glm::vec2 &agentPosition, entt::registry &_registry)
    {
        glm::vec2 separation = glm::vec2(0.0f);
        int numNeighbors = 0;

        auto view = _registry.view<Canis::RectTransformComponent, Canis::ColorComponent, Canis::Sprite2DComponent, BoidComponent>();
        for (auto [entity, rect_transform, color, sprite, boid] : view.each())
        {
            if (agent != entity)
            {
                numNeighbors++;
                if (glm::distance(agentPosition, rect_transform.position) <= MAX_SEPARATION_DISTANCE)
                {
                    separation += (agentPosition - rect_transform.position);
                }
            }
        }

        if (separation != glm::vec2(0.0f))
        {
            return glm::normalize(separation);
        }

        return glm::vec2(0.0f);
    }

public:
    BoidSystem() : Canis::System() {}

    void Create() {}

    void Ready() {}

    void Update(entt::registry &_registry, float _deltaTime)
    {
        glm::vec2 seekTarget, alignmentTarget, cohesionTarget, separationTarget, cameraPosition;
        Canis::Entity player;
        player.scene = scene;
        player = player.GetEntityWithTag("Player");
        glm::vec2 playerPosition = player.GetComponent<Canis::RectTransformComponent>().position;

        auto cam = _registry.view<Canis::Camera2DComponent>();
        for (auto [entity, camera2D] : cam.each())
        {
            cameraPosition = camera2D.position;
        }

        auto view = _registry.view<Canis::RectTransformComponent, Canis::ColorComponent, Canis::Sprite2DComponent, BoidComponent>();
        for (auto [entity, rect_transform, color, sprite, boid] : view.each())
        {
            // SEEK
            seekTarget = Seek(rect_transform.position, playerPosition);
            // Alignment
            alignmentTarget = Alignment(entity, rect_transform.position, _registry);
            // Cohesion
            cohesionTarget = Cohesion(entity, rect_transform.position, _registry);
            // Separation
            separationTarget = Separation(entity, rect_transform.position, _registry);

            boid.acceleration = ((seekTarget * USER_BEHAVIOR_WEIGHT) +
                                (alignmentTarget * ALIGNMENT_WEIGHT) +
                                (cohesionTarget * COHESION_WEIGHT) +
                                (separationTarget * SEPARATION_WEIGHT)) * boid.speed;

            //rect_transform.rotation = atan2(boid.velocity.y, boid.velocity.x);

            // update velocity
            boid.velocity += boid.acceleration * _deltaTime;

            // clamp velocity to maxSpeed
            // if (glm::length(boid.velocity) > boid.maxSpeed)
            // {
            //     boid.velocity = glm::normalize(boid.velocity);
            //     boid.velocity *= boid.maxSpeed;
            // }

            // apply drag
            boid.velocity *= boid.drag;

            // update position
            rect_transform.position += boid.velocity * _deltaTime;
        }
    }
};