#pragma once

#include <Canis/Yaml.hpp>
#include <Canis/Entity.hpp>

namespace Canis
{
    class SceneManager;
}

extern void DecodePlayerHealthComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager);

extern void DecodeEnemyHealthComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager);

extern void DecodeBulletComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager);

extern void DecodeBombComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager);

extern void DecodeSeedComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager);