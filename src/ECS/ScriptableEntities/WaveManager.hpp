#pragma once
#include <vector>
#include <string>
#include <map>
#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>

enum EnemyType
{
    STRANGLE_WEED = 0,
    ZOMBIE = 2,
    FIRE = 3,
    BOMB = 4,
    BRIAR = 5,
    TUMBLEWEED = 6,
    SWORD = 7
};

struct WaveEnemy
{
    int amount;
    std::string texPath;
    std::string animPath;
};

class WaveManager : public Canis::ScriptableEntity
{
    private:
    std::vector<WaveEnemy> m_enemies;
    Canis::Entity m_spawnManager;
    float m_waveFrequency = 20.0f;
    float m_timeSinceLastWave = 20.0f;

    void Populate()
    {
        {
            WaveEnemy enemy;
            enemy.amount = 1;
            enemy.animPath = "";
            enemy.texPath = "assets/textures/enemies/beehive.png";
            m_enemies.push_back(enemy);
        }

        {
            WaveEnemy enemy;
            enemy.amount = 2;
            enemy.animPath = "";
            enemy.texPath = "assets/textures/environment/planters.png";
            m_enemies.push_back(enemy);
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

        for (WaveEnemy enemy : m_enemies)
        {
            for (int i = 0; i < enemy.amount; i++)
            {
                auto e = CreateEntity();
                ((EnemySpawnManager*)m_spawnManager.GetComponent<Canis::ScriptComponent>().Instance)->SpawnEnemy(e, enemy.texPath);
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