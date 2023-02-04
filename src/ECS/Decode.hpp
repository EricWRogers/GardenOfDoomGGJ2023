#pragma once
#include <Canis/Entity.hpp>
#include <Canis/Yaml.hpp>
#include "Components/PlayerHealth.hpp"

void DecodePlayerHealthComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager)
{
    if (auto playerHealthComponent = _n["PlayerHealth"])
    {
        auto &c2dc = _entity.AddComponent<PlayerHealth>();
        c2dc.maxHealth = playerHealthComponent["maxHealth"].as<float>();
        c2dc.currentHealth = c2dc.maxHealth;
    }
}