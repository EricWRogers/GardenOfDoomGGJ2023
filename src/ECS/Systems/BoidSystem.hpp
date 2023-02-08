#pragma once
#include <execution>

#include <Canis/DataStucture/QuadTree.hpp>
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

struct BoidThreadInfo
{
    void *boidSystem;
    unsigned int startIndex = 0;
    unsigned int endIndex = 0;
};

class BoidSystem : public Canis::System
{
private:

public:

    //BoidSystem() : Canis::System() {}

    glm::vec2 cameraPosition;
    std::vector<entt::entity> boidEntities = {};

    void Create()
    {

    }

    void Ready()
    {

    }

    void Update(entt::registry &registry, const float _deltaTime)
    {

        Canis::Entity player;
        player.scene = scene;
        player = player.GetEntityWithTag("Player");
        glm::vec2 playerPosition = player.GetComponent<Canis::RectTransformComponent>().position;

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

        int alignNumNeighbors = 0;
        int cohNumNeighbors = 0;
        int sepNumNeighbors = 0;

        float distance = 0.0f;
        std::vector<Canis::QuadPoint> quadPoints = {};

        for (auto [entity, rect_transform, boid] : view.each())
        {
            alignment = glm::vec2(0.0f);
            cohesion = glm::vec2(0.0f);
            separation = glm::vec2(0.0f);
            alignNumNeighbors = 0;
            cohNumNeighbors = 0;

            for (auto [other_entity, other_rect_transform, other_boid] : view.each())
            {
                if(entity != other_entity)
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

            acceleration = ((seekTarget * USER_BEHAVIOR_WEIGHT) +
                            (alignmentTarget * ALIGNMENT_WEIGHT) +
                            (cohesionTarget * COHESION_WEIGHT) +
                            (separationTarget * SEPARATION_WEIGHT)) *
                           boid.speed;

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
};
