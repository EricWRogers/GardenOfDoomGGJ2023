#pragma once
#include <Canis/Entity.hpp>
#include <Canis/Yaml.hpp>
#include "Components/PlayerHealthComponent.hpp"

void DecodePlayerHealthComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager)
{
    if (auto playerHealthComponent = _n["PlayerHealthComponent"])
    {
        auto &c2dc = _entity.AddComponent<PlayerHealthComponent>();
        c2dc.maxHealth = playerHealthComponent["maxHealth"].as<float>();
        c2dc.currentHealth = c2dc.maxHealth;
    }
}