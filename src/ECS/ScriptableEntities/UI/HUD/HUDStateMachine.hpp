#pragma once
#include <string>
#include <vector>

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
public:
    HUDPauseState(std::function<void(std::string _name)> _changeState, std::string _name) :
        State(_changeState, _name) {}
    
    ~HUDPauseState() {}
    
    void Enter(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Enter(_scriptableEntity);

        _scriptableEntity.scene->SetTimeScale(0.0);
    }

    void Update(Canis::ScriptableEntity &_scriptableEntity, float _deltaTime)
    {
        State::Update(_scriptableEntity, _deltaTime);
    }

    void Exit(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Exit(_scriptableEntity);

        _scriptableEntity.scene->SetTimeScale(1.0);
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