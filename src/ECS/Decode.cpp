#include "Decode.hpp"

#include "Components/PlayerHealthComponent.hpp"
#include "Components/EnemyHealthComponent.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/SeedComponent.hpp"
#include "Components/BombComponent.hpp"

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

void DecodeBombComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager)
{
    if (auto bombComponent = _n["BombComponent"])
    {
        auto &bc = _entity.AddComponent<BombComponent>();
        bc.timeLeft = bombComponent["timeLeft"].as<float>();
        bc.damage = bombComponent["damage"].as<float>();
    }
}

void DecodeSeedComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager)
{
    if (auto seedComponent = _n["SeedComponent"])
    {
        auto &sc = _entity.AddComponent<BombComponent>();
        sc.timeLeft = seedComponent["temp"].as<int>();
    }
}