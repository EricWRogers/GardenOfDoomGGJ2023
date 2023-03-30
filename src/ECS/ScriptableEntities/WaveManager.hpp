#pragma once
#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include "Timer.hpp"
#include "EnemySpawnManager.hpp"

/*enum EnemyType
{
    STRANGLE_WEED = 0,
    ZOMBIE = 2,
    FIRE = 3,
    BOMB = 4,
    BRIAR = 5,
    TUMBLEWEED = 6,
    SWORD = 7
};*/

struct WaveEnemy
{
    int amount;
    float xpAmount;
    float maxHealth = 0.0f;
    float seedDropChance; //in percent
    std::string texPath;
    std::string animPath;
};

struct TimeBracket
{
    float start = 0.0f;
    float end = 0.0f;
};

struct Wave
{
    float delay = 0;
    std::vector<WaveEnemy> enemies;
    std::vector<std::vector<WaveEnemy>> waves;
    TimeBracket time;
};

class WaveManager : public Canis::ScriptableEntity
{
    private:
    std::vector<WaveEnemy> m_enemies;
    std::vector<Wave> m_waves;
    Canis::Entity m_spawnManager;
    Canis::Entity m_timer;
    float m_waveFrequency = 10.0f;
    float m_timeSinceLastWave = 20.0f;

    void Populate()
    {
        // {
        //     Wave wave;
        //     wave.delay = 5.0f;
        //     wave.time.start = 0.0f;
        //     wave.time.end = 10.0f;

        //     {
        //         WaveEnemy enemy;
        //         enemy.amount = 2;
        //         enemy.xpAmount = 50.0f;
        //         enemy.maxHealth = 50.0f;
        //         enemy.animPath = "assets/animations/tumbleweed.anim";
        //         enemy.texPath = "assets/textures/enemies/beehive.png";
        //         wave.enemies.push_back(enemy);
        //     }

        //     m_waves.push_back(wave);
        // }

        // {
        //     Wave wave;
        //     wave.delay = 5.0f;
        //     wave.time.start = 10.0f;
        //     wave.time.end = 20.0f;

        //     {
        //         WaveEnemy enemy;
        //         enemy.amount = 2;
        //         enemy.xpAmount = 50.0f;
        //         enemy.maxHealth = 50.0f;
        //         enemy.animPath = "assets/animations/bomb_plant.anim";
        //         enemy.texPath = "assets/textures/enemies/beehive.png";
        //         wave.enemies.push_back(enemy);
        //     }

        //     m_waves.push_back(wave);
        // }

        {
            Wave wave;
            wave.delay = 5.0f;
            wave.time.start = 0.0f;
            wave.time.end = 180.0f;

            {
                WaveEnemy enemy;
                enemy.amount = 2;
                enemy.xpAmount = 50.0f;
                enemy.maxHealth = 25.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/tumbleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 2;
                enemy.xpAmount = 100.0f;
                enemy.maxHealth = 30.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/tangleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 2;
                enemy.xpAmount = 50.0f;
                enemy.maxHealth = 25.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/bomb_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 2;
                enemy.xpAmount = 100.0f;
                enemy.maxHealth = 30.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/fire_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 2;
                enemy.xpAmount = 50.0f;
                enemy.maxHealth = 25.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/bee.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 2;
                enemy.xpAmount = 100.0f;
                enemy.maxHealth = 30.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/briar_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 2;
                enemy.xpAmount = 50.0f;
                enemy.maxHealth = 25.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/zombie_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 2;
                enemy.xpAmount = 100.0f;
                enemy.maxHealth = 30.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/sword_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 1;
                enemy.xpAmount = 200.0f;
                enemy.maxHealth = 150.0f;
                enemy.seedDropChance = 10.0f;
                enemy.animPath = "assets/animations/turtle.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }


            m_waves.push_back(wave);
        }
    }

    void SpawnWave()
    {
        // int totalAmount = 0;
        // for (WaveEnemy enemy : m_enemies)
        // {
        //     totalAmount += enemy.amount;
        // }
        // int amountForIteration = ceil(totalAmount / m_waveFrequency);
        // for (int i = 0; i < amountForIteration; i++)
        // {
        //     auto e = CreateEntity();
        //     m_spawnManager.GetComponent<EnemySpawnManager>().SpawnEnemy(e, m_enemies[i].texPath);
        // }

        for (Wave wave : m_waves)
        {
            if (((Timer*)m_timer.GetComponent<Canis::ScriptComponent>().Instance)->GetTime() >= wave.time.start && ((Timer*)m_timer.GetComponent<Canis::ScriptComponent>().Instance)->GetTime() < wave.time.end)
            {
                for (WaveEnemy enemy : wave.enemies)
                {
                    for (int i = 0; i < enemy.amount; i++)
                    {
                        currentEnemy = enemy;
                        ((EnemySpawnManager*)m_spawnManager.GetComponent<Canis::ScriptComponent>().Instance)->SpawnEnemy(enemy.texPath, enemy.animPath, enemy.xpAmount , enemy.maxHealth);
                    }
                }
            }
        }
    }

    public:

    WaveEnemy currentEnemy;

    void OnCreate()
    {

    }

    void OnReady()
    {
        m_spawnManager = m_Entity.GetEntityWithTag("EnemySpawnManager");
        m_timer = m_Entity.GetEntityWithTag("Timer");
        Populate();
    }

    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        if (m_timeSinceLastWave >= m_waveFrequency)
        {
            SpawnWave();
            m_timeSinceLastWave = 0.0f;
        }

        m_timeSinceLastWave += _dt;
    }
};

bool DecodeWaveManager(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "WaveManager")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<WaveManager>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}