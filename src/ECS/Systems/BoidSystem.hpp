#pragma once

#include <Canis/Entity.hpp>
#include <Canis/ECS/Systems/System.hpp>

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
    Canis::Entity m_player;
public:
    glm::vec2 cameraPosition;
    std::vector<entt::entity> boidEntities = {};

    void Create() {}

    void Ready();

    void Update(entt::registry &registry, const float _deltaTime);
};

extern bool DecodeBoidSystem(const std::string &_name, Canis::Scene *_scene);