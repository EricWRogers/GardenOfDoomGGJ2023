#pragma once
#include <string>
#include <vector>
#include <SDL_keycode.h>

#include <Canis/ECS/Components/ButtonComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/UIImageComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>

#include "../../StateMachine/StateMachine.hpp"

struct ButtonInfo{
    void* classObject = nullptr;
    void* data = nullptr;
};

static void OnClickPausePanelToMainMenu(void *instance)
{
    ((Canis::ScriptableEntity * )instance)->GetAssetManager().Get<Canis::SoundAsset>(((Canis::ScriptableEntity * )instance)->GetAssetManager().LoadSound("assets/sounds/click.wav"))->Play();

    ((Canis::SceneManager *)((Canis::ScriptableEntity * )instance)->m_Entity.scene->sceneManager)->Load("main_menu");

    ((Canis::ScriptableEntity * )instance)->m_Entity.scene->SetTimeScale(1.0f);
}

static void OnClickPausePanelToResume(void *instance)
{
    ((StateMachine* )instance)->ChangeState("HUDState");
}

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

        if (_scriptableEntity.GetInputManager().JustPressedKey(SDLK_ESCAPE) ||
            _scriptableEntity.GetInputManager().GetButton(0, Canis::ControllerButton::START)) {
            ChangeState("HUDPauseState");
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
            m_resumeButton = _scriptableEntity.CreateEntity();
            auto& playRect = m_resumeButton.AddComponent<Canis::RectTransformComponent>(
                true,
                Canis::RectAnchor::CENTER,
                glm::vec2(-70.0f,60.0f),
                glm::vec2(150.0f, 40.0f),
                glm::vec2(0.0f,0.0f),
                0.0f,
                1.0f,
                -1.0f
            );
            auto& playColor = m_resumeButton.AddComponent<Canis::ColorComponent>(
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            auto& playText = m_resumeButton.AddComponent<Canis::TextComponent>(
                _scriptableEntity.GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
                new std::string("Resume"), // text
                0u
            );
            auto& playB = m_resumeButton.AddComponent<Canis::ButtonComponent>(
                OnClickPausePanelToResume,
                (void*)&_scriptableEntity,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                glm::vec4(0.24f, 0.37f, 0.25f, 1.0f) //Green, #3c5e40
            );
        }

        if (!m_mainMenuButton)
        {
            m_mainMenuButton = _scriptableEntity.CreateEntity();
            auto& playRect = m_mainMenuButton.AddComponent<Canis::RectTransformComponent>(
                true,
                Canis::RectAnchor::CENTER,
                glm::vec2(-100.0f,-40.0f),
                glm::vec2(150.0f, 40.0f),
                glm::vec2(0.0f,0.0f),
                0.0f,
                1.0f,
                -1.0f
            );
            auto& playColor = m_mainMenuButton.AddComponent<Canis::ColorComponent>(
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            auto& playText = m_mainMenuButton.AddComponent<Canis::TextComponent>(
                _scriptableEntity.GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
                new std::string("Main Menu"), // text
                0u
            );
            auto& playB = m_mainMenuButton.AddComponent<Canis::ButtonComponent>(
                OnClickPausePanelToMainMenu,
                (void*)&_scriptableEntity,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                glm::vec4(0.24f, 0.37f, 0.25f, 1.0f) //Green, #3c5e40
            );
        }

        if (!m_titleText)
        {
            m_titleText = _scriptableEntity.m_Entity.scene->CreateEntity();
            m_titleText.AddComponent<Canis::RectTransformComponent>(
                true,
                Canis::RectAnchor::CENTER,
                glm::vec2(-60.0f,140.0f),
                glm::vec2(400.0f),
                glm::vec2(0.0f,0.0f),
                0.0f,
                1.0f,
                -1.0f
            );
            m_titleText.AddComponent<Canis::ColorComponent>(
                glm::vec4(0.0f)
            );
            m_titleText.AddComponent<Canis::TextComponent>(
                _scriptableEntity.GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
                new std::string("Pause"),
                0u
            );
        }

        m_panel.GetComponent<Canis::RectTransformComponent>().active = true;
        m_resumeButton.GetComponent<Canis::RectTransformComponent>().active = true;
        m_mainMenuButton.GetComponent<Canis::RectTransformComponent>().active = true;
        m_titleText.GetComponent<Canis::RectTransformComponent>().active = true;
    }

    void Update(Canis::ScriptableEntity &_scriptableEntity, float _deltaTime)
    {
        State::Update(_scriptableEntity, _deltaTime);

        if (_scriptableEntity.GetInputManager().JustPressedKey(SDLK_ESCAPE)) {
            ChangeState("HUDState");
        }
    }

    void Exit(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Exit(_scriptableEntity);

        _scriptableEntity.m_Entity.scene->SetTimeScale(1.0);

        m_panel.GetComponent<Canis::RectTransformComponent>().active = false;
        m_resumeButton.GetComponent<Canis::RectTransformComponent>().active = false;
        m_mainMenuButton.GetComponent<Canis::RectTransformComponent>().active = false;
        m_titleText.GetComponent<Canis::RectTransformComponent>().active = false;
    }
};

class HUDLevelUpState : public State
{
private:
    Canis::Entity m_panel;
    Canis::Entity m_LeftButton;
    Canis::Entity m_RightButton;
    Canis::Entity m_titleText;
    ButtonInfo buttonInfoZero = {};
    ButtonInfo buttonInfoOne = {};
    int slotNumberZero = 0;
    int slotNumberOne = 1;
public:
    int chosenIndex = -1;
    HUDLevelUpState(std::function<void(std::string _name)> _changeState, std::string _name) :
        State(_changeState, _name) {
        buttonInfoZero.classObject = (void*)this;
        buttonInfoZero.data = &slotNumberZero;

        buttonInfoOne.classObject = (void*)this;
        buttonInfoOne.data = &slotNumberOne;
    }
    
    ~HUDLevelUpState() {

    }

    static void OnClickAward(void *instance)
    {
        ButtonInfo buttonInfo = *(ButtonInfo*)instance;
        HUDLevelUpState& levelUpState = *(HUDLevelUpState*)buttonInfo.classObject;
        int data = *(int*)buttonInfo.data;

        levelUpState.chosenIndex = data;
    }
    
    void Enter(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Enter(_scriptableEntity);

        _scriptableEntity.m_Entity.scene->SetTimeScale(0.0);

        Canis::Entity player = _scriptableEntity.m_Entity.GetEntityWithTag("Player");
        PlayerManager& playerManager = *((PlayerManager*)player.GetComponent<Canis::ScriptComponent>().Instance);

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

        if (!m_LeftButton)
        {
            m_LeftButton = _scriptableEntity.CreateEntity();
            auto& buttonRect = m_LeftButton.AddComponent<Canis::RectTransformComponent>(
                true,
                Canis::RectAnchor::CENTER,
                glm::vec2(0.0f,-50.0f),
                glm::vec2(128.0f, 128.0f),
                glm::vec2(0.0f,0.0f),
                0.0f,
                1.0f,
                -1.0f
            );
            auto& buttonColor = m_LeftButton.AddComponent<Canis::ColorComponent>(
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            auto& buttonImage = m_LeftButton.AddComponent<Canis::UIImageComponent>();
            buttonImage.texture = _scriptableEntity.GetAssetManager().Get<Canis::TextureAsset>(
            _scriptableEntity.GetAssetManager().LoadTexture("assets/textures/UI/level_up_icons_sprite_sheet.png"))->GetTexture();

            GetUIImageFromTextureAtlas(
                buttonImage,
                0,
                0,
                playerManager.weaponIDoNotHave[0],
                0,
                64,
                64,
                false,
                false
            );

            auto& buttonB = m_LeftButton.AddComponent<Canis::ButtonComponent>(
                OnClickAward,
                (void*)&buttonInfoZero,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                glm::vec4(0.24f, 0.37f, 0.25f, 1.0f) //Green, #3c5e40
            );
        }

        if (!m_RightButton && (playerManager.weaponIDoNotHave.size() > 1))
        {
            m_RightButton = _scriptableEntity.CreateEntity();
            auto& buttonRect = m_RightButton.AddComponent<Canis::RectTransformComponent>(
                true,
                Canis::RectAnchor::CENTER,
                glm::vec2(-130.0f,-50.0f),
                glm::vec2(128.0f, 128.0f),
                glm::vec2(0.0f,0.0f),
                0.0f,
                1.0f,
                -1.0f
            );
            auto& buttonColor = m_RightButton.AddComponent<Canis::ColorComponent>(
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            auto& buttonImage = m_RightButton.AddComponent<Canis::UIImageComponent>();
            buttonImage.texture = _scriptableEntity.GetAssetManager().Get<Canis::TextureAsset>(
            _scriptableEntity.GetAssetManager().LoadTexture("assets/textures/UI/level_up_icons_sprite_sheet.png"))->GetTexture();

            GetUIImageFromTextureAtlas(
                buttonImage,
                0,
                0,
                playerManager.weaponIDoNotHave[1],
                0,
                64,
                64,
                false,
                false
            );

            auto& buttonB = m_RightButton.AddComponent<Canis::ButtonComponent>(
                OnClickAward,
                (void*)&buttonInfoOne,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                glm::vec4(0.24f, 0.37f, 0.25f, 1.0f) //Green, #3c5e40
            );
        }

        if (!m_titleText)
        {
            m_titleText = _scriptableEntity.m_Entity.scene->CreateEntity();
            m_titleText.AddComponent<Canis::RectTransformComponent>(
                true,
                Canis::RectAnchor::CENTER,
                glm::vec2(-60.0f,140.0f),
                glm::vec2(400.0f),
                glm::vec2(0.0f,0.0f),
                0.0f,
                1.0f,
                -1.0f
            );
            m_titleText.AddComponent<Canis::ColorComponent>(
                glm::vec4(0.0f)
            );
            m_titleText.AddComponent<Canis::TextComponent>(
                _scriptableEntity.GetAssetManager().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
                new std::string("Level Up"),
                0u
            );
        }

        auto& buttonImage = m_LeftButton.GetComponent<Canis::UIImageComponent>();
        buttonImage.texture = _scriptableEntity.GetAssetManager().Get<Canis::TextureAsset>(
        _scriptableEntity.GetAssetManager().LoadTexture("assets/textures/UI/level_up_icons_sprite_sheet.png"))->GetTexture();

        GetUIImageFromTextureAtlas(
            buttonImage,
            0,
            0,
            playerManager.weaponIDoNotHave[0],
            0,
            64,
            64,
            false,
            false
        );

        

        m_panel.GetComponent<Canis::RectTransformComponent>().active = true;
        m_LeftButton.GetComponent<Canis::RectTransformComponent>().active = true;
        if (playerManager.weaponIDoNotHave.size() > 1)
        {
            m_RightButton.GetComponent<Canis::RectTransformComponent>().active = true;

            auto& buttonImage2 = m_RightButton.GetComponent<Canis::UIImageComponent>();
            buttonImage2.texture = _scriptableEntity.GetAssetManager().Get<Canis::TextureAsset>(
            _scriptableEntity.GetAssetManager().LoadTexture("assets/textures/UI/level_up_icons_sprite_sheet.png"))->GetTexture();

            GetUIImageFromTextureAtlas(
                buttonImage2,
                0,
                0,
                playerManager.weaponIDoNotHave[1],
                0,
                64,
                64,
                false,
                false
            );
        }
        else
        {
            m_RightButton.GetComponent<Canis::RectTransformComponent>().active = false;
        }
            
        m_titleText.GetComponent<Canis::RectTransformComponent>().active = true;
    }

    void Update(Canis::ScriptableEntity &_scriptableEntity, float _deltaTime)
    {
        State::Update(_scriptableEntity, _deltaTime);

        if (chosenIndex != -1)
        {
            Canis::Entity player = _scriptableEntity.m_Entity.GetEntityWithTag("Player");
            PlayerManager& playerManager = *((PlayerManager*)player.GetComponent<Canis::ScriptComponent>().Instance);
            playerManager.AddWeaponToSlot(playerManager.weaponIDoNotHave[chosenIndex]);
            playerManager.weaponIDoNotHave.erase(playerManager.weaponIDoNotHave.begin()+chosenIndex);
            chosenIndex = -1;
            ChangeState("HUDState");
        }
    }

    void Exit(Canis::ScriptableEntity &_scriptableEntity)
    {
        State::Exit(_scriptableEntity);

        _scriptableEntity.m_Entity.scene->SetTimeScale(1.0);

        m_panel.GetComponent<Canis::RectTransformComponent>().active = false;
        m_LeftButton.GetComponent<Canis::RectTransformComponent>().active = false;
        m_RightButton.GetComponent<Canis::RectTransformComponent>().active = false;
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
        m_states.push_back(new HUDLevelUpState([this] (std::string _name){this->ChangeState(_name);}, "HUDLevelUpState"));

        SetState(m_states[0]);
    }

    void OnReady()
    {
        StateMachine::OnReady();

        Canis::Entity player = m_Entity.GetEntityWithTag("Player");
        PlayerManager& playerManager = *((PlayerManager*)player.GetComponent<Canis::ScriptComponent>().Instance);
        playerManager.levelUpEvent = [this](){this->ChangeState("HUDLevelUpState");};
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