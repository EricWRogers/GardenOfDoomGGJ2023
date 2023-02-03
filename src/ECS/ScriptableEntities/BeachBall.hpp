#pragma once
#include <string>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>

class BeachBall : public Canis::ScriptableEntity
{
private:
    glm::vec2   m_direction;
    float       m_speed;
    float       m_timeBetweenAnimation = 3.0f;
    float       m_countDown = 0.0f;
    unsigned int m_animIndex = 0;
    std::vector<glm::vec2> m_spawnPoints = {};
public:
    void OnCreate()
    {
        m_countDown = m_timeBetweenAnimation;
    }

    void OnReady()
    {
        m_direction = glm::vec2(1.0f, 0.4f);
        m_speed = 150.0f;
        /*auto& sprite = GetComponent<Canis::Sprite2DComponent>();
            GetSpriteFromTextureAtlas(
                sprite,
                m_animIndex,
                0,
                48,
                50,
                true,
                false
            );*/
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {        
        /*m_countDown -= _dt;

        if (m_countDown < 0.0f)
        {
            m_countDown = m_timeBetweenAnimation;

            m_animIndex++;

            if (m_animIndex > 5)
                m_animIndex = 0;

            auto& sprite = GetComponent<Canis::Sprite2DComponent>();
            GetSpriteFromTextureAtlas(
                sprite,
                m_animIndex,
                0,
                48,
                50,
                true,
                true
            );
        }*/

        auto& rect = GetComponent<Canis::RectTransformComponent>();

        float halfSizeX = rect.size.x/2.0f;
        float halfSizeY = rect.size.y/2.0f;

        if (rect.position.x + halfSizeX >= GetWindow().GetScreenWidth()/2.0f ||
                rect.position.x - halfSizeX <= GetWindow().GetScreenWidth()/-2.0f)
            m_direction.x *= -1.0f;

        if (rect.position.y + halfSizeY >= GetWindow().GetScreenHeight()/2.0f ||
                rect.position.y - halfSizeY <= GetWindow().GetScreenHeight()/-2.0f)
            m_direction.y *= -1.0f;
        
        rect.position += (m_direction * (m_speed * _dt));

        if (GetInputManager().JustPressedKey(SDLK_p))
            m_speed += 50.0f;
        
        if (GetInputManager().JustPressedKey(SDLK_r))
            m_speed = 150.0f;
        
        if (GetInputManager().JustPressedKey(SDLK_d))
        {
            auto e = CreateEntity();
            auto& rectTransform = e.AddComponent<Canis::RectTransformComponent>();
            rectTransform = rect;
            auto& color = e.AddComponent<Canis::ColorComponent>();
            color = GetComponent<Canis::ColorComponent>();
            auto& sprite = e.AddComponent<Canis::Sprite2DComponent>();
            sprite = GetComponent<Canis::Sprite2DComponent>();
        }
    }
};