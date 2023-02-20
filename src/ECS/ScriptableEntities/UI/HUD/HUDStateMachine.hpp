#pragma once
#include <string>
#include <vector>

#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/UIImageComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>

#include "../../StateMachine/StateMachine.hpp"

class HUDState : public State
{
private:
public:
    HUDState(std::function<void(std::string _name)> _changeState, std::string _name) :
        State(_changeState, _name) {}
    
    ~HUDState() {}
    
    void Enter(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Enter(_scriptableEntity);

        Canis::Log("Enter HUDState");
    }

    void Update(Canis::ScriptableEntity &_scriptableEntity, float _deltaTime)
    {
        State::Update(_scriptableEntity, _deltaTime);

        if (_scriptableEntity.GetInputManager().JustPressedKey(SDLK_ESCAPE)) {
            m_changeState("HUDPauseState");
        }
    }

    void Exit(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Exit(_scriptableEntity);
    }
};

class HUDPauseState : public State
{
private:
    Canis::Entity m_panel;
    Canis::Entity m_resumeButton;
    Canis::Entity m_mainMenuButton;
    Canis::Entity m_titleText;
public:
    HUDPauseState(std::function<void(std::string _name)> _changeState, std::string _name) :
        State(_changeState, _name) {}
    
    ~HUDPauseState() {}
    
    void Enter(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Enter(_scriptableEntity);

        _scriptableEntity.m_Entity.scene->SetTimeScale(0.0);

        if (!m_panel)
        {
            m_panel = _scriptableEntity.m_Entity.scene->CreateEntity();
            m_panel.AddComponent<Canis::RectTransformComponent>(
                true,
                Canis::RectAnchor::CENTER,
                glm::vec2(0.0f),
                glm::vec2(400.0f),
                glm::vec2(-200.0f),
                0.0f,
                1.0f,
                -0.9f
            );
            m_panel.AddComponent<Canis::ColorComponent>(
                glm::vec4(1.0f)
            );
            m_panel.AddComponent<Canis::UIImageComponent>();
            m_panel.GetComponent<Canis::UIImageComponent>().texture = _scriptableEntity.GetAssetManager().Get<Canis::TextureAsset>(
                _scriptableEntity.GetAssetManager().LoadTexture(
                    "assets/textures/UI/hud_panel.png"
                ))->GetTexture();
        }

        if (!m_resumeButton)
        {

        }

        if (!m_mainMenuButton)
        {

        }

        if (!m_titleText)
        {
            m_titleText = _scriptableEntity.m_Entity.scene->CreateEntity();
            m_titleText.AddComponent<Canis::RectTransformComponent>(
                true,
                Canis::RectAnchor::CENTER,
                glm::vec2(-50.0f,140.0f),
                glm::vec2(400.0f),
                glm::vec2(0.0f,0.0f),
                0.0f,
                1.0f,
                -1.0f
            );
            m_titleText.AddComponent<Canis::ColorComponent>(
                glm::vec4(1.0f)
            );
            m_titleText.AddComponent<Canis::TextComponent>(
                _scriptableEntity.GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
                new std::string("Pause"),
                0u
            );
        }

        m_panel.GetComponent<Canis::RectTransformComponent>().active = true;
        //m_resumeButton.GetComponent<Canis::RectTransformComponent>().active = true;
        //m_mainMenuButton.GetComponent<Canis::RectTransformComponent>().active = true;
        m_titleText.GetComponent<Canis::RectTransformComponent>().active = true;
    }

    void Update(Canis::ScriptableEntity &_scriptableEntity, float _deltaTime)
    {
        State::Update(_scriptableEntity, _deltaTime);

        if (_scriptableEntity.GetInputManager().JustPressedKey(SDLK_ESCAPE)) {
            m_changeState("HUDState");
        }
    }

    void Exit(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Exit(_scriptableEntity);

        _scriptableEntity.m_Entity.scene->SetTimeScale(1.0);

        m_panel.GetComponent<Canis::RectTransformComponent>().active = false;
        //m_resumeButton.GetComponent<Canis::RectTransformComponent>().active = false;
        //m_mainMenuButton.GetComponent<Canis::RectTransformComponent>().active = false;
        m_titleText.GetComponent<Canis::RectTransformComponent>().active = false;
    }
};

class HUDStateMachine : public StateMachine
{
private:
public:
    void OnCreate()
    {
        StateMachine::OnCreate();

        m_states.push_back(new HUDState([this] (std::string _name){ this->ChangeState(_name); }, "HUDState"));
        m_states.push_back(new HUDPauseState([this] (std::string _name){this->ChangeState(_name); }, "HUDPauseState"));

        SetState(m_states[0]);
    }

    void OnReady()
    {
        StateMachine::OnReady();
    }
    
    void OnDestroy()
    {
        StateMachine::OnDestroy();
    }

    void OnUpdate(float _dt)
    {        
        StateMachine::OnUpdate(_dt);
    }
};