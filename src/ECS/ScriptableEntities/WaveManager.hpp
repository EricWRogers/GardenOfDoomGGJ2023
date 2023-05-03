#pragma once

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
    float speed;
    float maxSpeed;
    float damage;
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
    float m_waveFrequency = 10.0f; //Time between enemy spawns
    float m_timeSinceLastWave = 20.0f;

    void Populate()
    {
        //Enemies               in waves
        //zombie_plant.anim     1, 2, 3        6
        //sword_plant.anim         2, 3, 4
        //bee.anim                    3, 4
        //tangleweed.anim                4, 5, 6
        //tumbleweed.anim                   5, 6
        //briar_plant.anim                     6

        //fire_plant.anim
        //turtle.anim
        //bomb_plant.anim

        //Wave 1
        {
            Wave wave;
            wave.delay = 7.0f; //Doesn't do anything
            wave.time.start = 0.0f;
            wave.time.end = 30.0f;

            {
                WaveEnemy enemy; //Zombie Plants
                enemy.amount = 4;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 25.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 30.0f;
                enemy.maxSpeed = 50.0f;
                enemy.damage = 8.0f;
                enemy.animPath = "assets/animations/zombie_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            m_waves.push_back(wave);
        }

        //Wave 2
        {
            Wave wave;
            wave.delay = 7.0f; //Doesn't do anything
            wave.time.start = 30.0f;
            wave.time.end = 60.0f;

            {
                WaveEnemy enemy; //Zombie Plants
                enemy.amount = 3;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 25.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 30.0f;
                enemy.maxSpeed = 50.0f;
                enemy.damage = 8.0f;
                enemy.animPath = "assets/animations/zombie_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Sword Plants
                enemy.amount = 2;
                enemy.xpAmount = 40.0f;
                enemy.maxHealth = 35.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 30.0f;
                enemy.maxSpeed = 50.0f;
                enemy.damage = 10.0f;
                enemy.animPath = "assets/animations/sword_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            m_waves.push_back(wave);
        }

        //Wave 3
        {
            Wave wave;
            wave.delay = 7.0f; //Doesn't do anything
            wave.time.start = 60.0f;
            wave.time.end = 90.0f;

            {
                WaveEnemy enemy; //Zombie Plants
                enemy.amount = 2;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 25.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 30.0f;
                enemy.maxSpeed = 50.0f;
                enemy.damage = 8.0f;
                enemy.animPath = "assets/animations/zombie_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Sword Plants
                enemy.amount = 3;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 35.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 30.0f;
                enemy.maxSpeed = 50.0f;
                enemy.damage = 10.0f;
                enemy.animPath = "assets/animations/sword_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Bee
                enemy.amount = 3;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 20.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 50.0f;
                enemy.maxSpeed = 70.0f;
                enemy.damage = 4.0f;
                enemy.animPath = "assets/animations/bee.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            m_waves.push_back(wave);
        }

        //Wave 4
        {
            Wave wave;
            wave.delay = 7.0f; //Doesn't do anything
            wave.time.start = 90.0f;
            wave.time.end = 120.0f;

            {
                WaveEnemy enemy; //Sword Plants
                enemy.amount = 4;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 35.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 30.0f;
                enemy.maxSpeed = 50.0f;
                enemy.damage = 10.0f;
                enemy.animPath = "assets/animations/sword_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Bee
                enemy.amount = 5;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 20.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 50.0f;
                enemy.maxSpeed = 70.0f;
                enemy.damage = 4.0f;
                enemy.animPath = "assets/animations/bee.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Tangleweed
                enemy.amount = 2;
                enemy.xpAmount = 60.0f;
                enemy.maxHealth = 50.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 40.0f;
                enemy.maxSpeed = 60.0f;
                enemy.damage = 13.0f;
                enemy.animPath = "assets/animations/tangleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            m_waves.push_back(wave);
        }

        //Wave 5
        {
            Wave wave;
            wave.delay = 7.0f; //Doesn't do anything
            wave.time.start = 120.0f;
            wave.time.end = 150.0f;

            {
                WaveEnemy enemy; //Tangleweed
                enemy.amount = 5;
                enemy.xpAmount = 60.0f;
                enemy.maxHealth = 50.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 40.0f;
                enemy.maxSpeed = 60.0f;
                enemy.damage = 13.0f;
                enemy.animPath = "assets/animations/tangleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Tumbleweed
                enemy.amount = 2;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 15.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 80.0f;
                enemy.maxSpeed = 100.0f;
                enemy.damage = 8.0f;
                enemy.animPath = "assets/animations/tumbleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            m_waves.push_back(wave);
        }

        //Wave 6
        {
            Wave wave;
            wave.delay = 7.0f; //Doesn't do anything
            wave.time.start = 150.0f;
            wave.time.end = 180.0f;

            {
                WaveEnemy enemy; //Zombie Plants
                enemy.amount = 6;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 25.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 30.0f;
                enemy.maxSpeed = 50.0f;
                enemy.damage = 8.0f;
                enemy.animPath = "assets/animations/zombie_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Tangleweed
                enemy.amount = 4;
                enemy.xpAmount = 60.0f;
                enemy.maxHealth = 50.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 40.0f;
                enemy.maxSpeed = 60.0f;
                enemy.damage = 13.0f;
                enemy.animPath = "assets/animations/tangleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Tumbleweed
                enemy.amount = 2;
                enemy.xpAmount = 30.0f;
                enemy.maxHealth = 15.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 80.0f;
                enemy.maxSpeed = 100.0f;
                enemy.damage = 8.0f;
                enemy.animPath = "assets/animations/tumbleweed.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            {
                WaveEnemy enemy; //Briar Plant
                enemy.amount = 2;
                enemy.xpAmount = 100.0f;
                enemy.maxHealth = 65.0f;
                enemy.seedDropChance = 10.0f;
                enemy.speed = 25.0f;
                enemy.maxSpeed = 45.0f;
                enemy.damage = 20.0f;
                enemy.animPath = "assets/animations/briar_plant.anim";
                enemy.texPath = "assets/textures/enemies/beehive.png";
                wave.enemies.push_back(enemy);
            }

            m_waves.push_back(wave);
        }
    }

    void SpawnWave()
    {
        for (Wave wave : m_waves)
        {
            if (((Timer*)m_timer.GetComponent<Canis::ScriptComponent>().Instance)->GetTime() >= wave.time.start && ((Timer*)m_timer.GetComponent<Canis::ScriptComponent>().Instance)->GetTime() < wave.time.end)
            {
                for (WaveEnemy enemy : wave.enemies)
                {
                    for (int i = 0; i < enemy.amount; i++)
                    {
                        currentEnemy = enemy;
                        ((EnemySpawnManager*)m_spawnManager.GetComponent<Canis::ScriptComponent>().Instance)->SpawnEnemy(enemy.texPath, enemy.animPath, enemy.xpAmount , enemy.maxHealth, enemy.speed, enemy.maxSpeed, enemy.damage);
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