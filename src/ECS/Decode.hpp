#pragma once
#include <Canis/Entity.hpp>
#include <Canis/Yaml.hpp>
#include "Components/PlayerHealthComponent.hpp"
#include "Components/EnemyHealthComponent.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/BulletComponent.hpp"

void DecodePlayerHealthComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager)
{
    if (auto playerHealthComponent = _n["PlayerHealthComponent"])
    {
        auto &c2dc = _entity.AddComponent<PlayerHealthComponent>();
        c2dc.maxHealth = playerHealthComponent["maxHealth"].as<float>();
        c2dc.currentHealth = c2dc.maxHealth;
    }
}

void DecodeEnemyHealthComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager)
{
    if (auto enemyHealthComponent = _n["EnemyHealthComponent"])
    {
        auto &c2dc = _entity.AddComponent<EnemyHealthComponent>();
        c2dc.maxHealth = enemyHealthComponent["maxHealth"].as<float>();
        c2dc.currentHealth = c2dc.maxHealth;
    }
}

void DecodeBulletComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager)
{
    if (auto enemyHealthComponent = _n["BulletComponent"])
    {
        auto &bc = _entity.AddComponent<BulletComponent>();
        bc.direction = enemyHealthComponent["direction"].as<glm::vec2>();
        bc.speed = enemyHealthComponent["speed"].as<float>();
        bc.damage = enemyHealthComponent["damage"].as<float>();
        bc.timeLeft = enemyHealthComponent["timeLeft"].as<float>();
    }
}