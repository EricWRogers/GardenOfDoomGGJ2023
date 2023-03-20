#pragma once

#include <Canis/ECS/Systems/System.hpp>

class DamageTextSystem : public Canis::System
{
private:
    Canis::RenderTextSystem *m_renderTextSystem = nullptr;
    int m_fontId = 0;
public:
    void Create() {}

    void Ready()
    {
        m_renderTextSystem = GetScene().GetSystem<Canis::RenderTextSystem>();

        if (m_renderTextSystem == nullptr)
            Canis::FatalError("Damage text system was not able to find render text system");
        
        m_fontId = GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf",18);
    }

    void Update(entt::registry &registry, const float _deltaTime)
    {
        std::string sText = "";
        glm::vec2 positionAnchor = glm::vec2(0.0f);
        auto view = registry.view<DamageTextComponent>();

        auto cam = registry.view<const Canis::Camera2DComponent>();

        float cameraSize;
        glm::vec2 cameraPosition;
        for (auto [entity, camera2D] : cam.each())
        {
            cameraPosition = camera2D.position;
            cameraSize = camera2D.scale;
        }

        glDepthFunc(GL_ALWAYS);
        m_renderTextSystem->textShader.Use();
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::ortho(0.0f, static_cast<float>(window->GetScreenWidth()), 0.0f, static_cast<float>(window->GetScreenHeight()));
        glUniformMatrix4fv(glGetUniformLocation(m_renderTextSystem->textShader.GetProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        for(auto [entity, text] : view.each())
        {
            text.countDown -= _deltaTime;

            if (text.countDown <= 0.0f)
            {
                registry.destroy(entity);
                continue;
            }

            /*for(int i = 0; i < DamageText::CHARTEXTLENGTH; i++)
            {
                if (text.text[i] == '\0')
                    continue;*/
                
                sText = std::string(text.text);
            //}

            auto& cc = registry.get<Canis::ColorComponent>(entity);
            auto& rect = registry.get<Canis::RectTransformComponent>(entity);

            positionAnchor = Canis::GetAnchor((Canis::RectAnchor)6,
                        (float)window->GetScreenWidth(),
                        (float)window->GetScreenHeight());

            m_renderTextSystem->RenderText(m_renderTextSystem->textShader,
                sText,
                rect.position.x + ((float)window->GetScreenWidth()/2) - cameraPosition.x,
                rect.position.y + ((float)window->GetScreenHeight()/2) - cameraPosition.y,
                rect.scale,
                cc.color,
                m_fontId,
                0);
        }
        m_renderTextSystem->textShader.UnUse();
    }
};

bool DecodeDamageTextSystem(const std::string &_name, Canis::Scene *_scene) {
    if(_name == "DamageTextSystem"){
        _scene->CreateRenderSystem<DamageTextSystem>();
        return true;
    }
    return false;
}