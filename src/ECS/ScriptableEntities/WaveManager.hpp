#pragma once
#include <vector>
#include <string>
#include <map>
#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>

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
    std::string texPath;
    std::string animPath;
};

struct Wave
{
    float delay = 0;
    std::vector<WaveEnemy> enemies;
    std::vector<std::vector<WaveEnemy>> waves;
};

class WaveManager : public Canis::ScriptableEntity
{
    private:
    std::vector<WaveEnemy> m_enemies;
    std::vector<Wave> m_waves;
    Canis::Entity m_spawnManager;
    float m_waveFrequency = 20.0f;
    float m_timeSinceLastWave = 20.0f;

    void Populate()
    {
        {
            Wave wave;
            wave.delay = 10.0f;

            {
                WaveEnemy enemy;
                enemy.amount = 15;
                enemy.xpAmount = 50.0f;
                enemy.maxHealth = 15.0f;
                enemy.animPath = "assets/animations/tumbleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 5;
                enemy.xpAmount = 100.0f;
                enemy.maxHealth = 20.0f;
                enemy.animPath = "assets/animations/tangleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

             {
                WaveEnemy enemy;
                enemy.amount = 15;
                enemy.xpAmount = 50.0f;
                enemy.maxHealth = 15.0f;
                enemy.animPath = "assets/animations/bomb_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy;
                enemy.amount = 10;
                enemy.xpAmount = 100.0f;
                enemy.maxHealth = 20.0f;
                enemy.animPath = "assets/animations/fire_plant.anim";
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
            for (WaveEnemy enemy : wave.enemies)
            {
                for (int i = 0; i < enemy.amount; i++)
                {
                    ((EnemySpawnManager*)m_spawnManager.GetComponent<Canis::ScriptComponent>().Instance)->SpawnEnemy(enemy.texPath, enemy.animPath, enemy.xpAmount , enemy.maxHealth);
                }
            }
        }
    }

    public:
    void OnCreate()
    {

    }

    void OnReady()
    {
        m_spawnManager = m_Entity.GetEntityWithTag("EnemySpawnManager");
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