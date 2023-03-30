#pragma once

class CactusWeapon : public WeaponClass
{
public:

    float damage = 10.0f;

    void OnCreate()
    {
        
    }

    void OnReady()
    {

    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
       std::vector<entt::entity> hit = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);
        Canis::Entity hitEntity;

        for(int i; i <= hit.size(); i++)
        {
            hitEntity.entityHandle = hit[i];
            hitEntity.GetComponent<EnemySpawnManager>().health -= damage;

        }
    }
};