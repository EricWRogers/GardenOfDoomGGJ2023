#pragma once
class EnemyMovement : public Canis::ScriptableEntity
{
    private:
    Canis::Entity m_player;
    Canis::Entity m_spawnManager;

    public:
    int health = 100;

    void OnCreate()
    {
        
    }

    void OnReady()
    {
        m_player = m_Entity.GetEntityWithTag("Player");
        m_spawnManager = m_Entity.GetEntityWithTag("EnemySpawnManager");
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        auto& m_transform = GetComponent<Canis::RectTransformComponent>();
        //m_transform.position += glm::normalize(m_player.GetComponent<Canis::RectTransformComponent>().position - m_transform.position) * 10.0f * _dt;
    }
};