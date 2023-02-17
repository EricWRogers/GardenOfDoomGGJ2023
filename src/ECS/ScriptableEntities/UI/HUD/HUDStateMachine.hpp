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
    
    void Enter()
    {
        State::Enter();

        Canis::Log("Enter HUDState");
    }

    void Update(Canis::ScriptableEntity &_scriptableEntity, float _deltaTime)
    {
        State::Update(_scriptableEntity, _deltaTime);
    }

    void Exit()
    {
        State::Exit();
    }
};

class HUDStateMachine : public StateMachine
{
private:
public:
    void OnCreate()
    {
        StateMachine::OnCreate();

        Canis::Log("OnCreate HUDStateMachine");

        m_states.push_back(new HUDState([this] (std::string _name){ this->ChangeState(_name); }, "HUDState"));

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