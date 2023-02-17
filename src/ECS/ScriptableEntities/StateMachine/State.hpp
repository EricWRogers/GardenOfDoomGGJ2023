#pragma once
#include <string>
#include <vector>
#include <functional>

class State
{
private:
    bool m_hasBeenEntered = false;
    std::function<void(std::string _name)> m_changeState = nullptr;
public:
    std::string name = "";

    State(std::function<void(std::string _name)> _changeState, std::string _name) {
        m_changeState = _changeState;
        name = _name;
    }

    virtual void Enter() {
        m_hasBeenEntered = true;
        Canis::Log("Enter State");
    }
    
    virtual void Update(Canis::ScriptableEntity &_scriptableEntity, float _deltaTime) {

    }
    
    virtual void Exit() {

    }
};