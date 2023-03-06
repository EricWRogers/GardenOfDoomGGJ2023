#pragma once
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include "../Components/PlayerHealthComponent.hpp"
#include "XP.hpp"

enum WeaponType
{
    GASAURA         = 0u,
    HANDOFGOD       = 1u,
    ORBITINGSPIKES  = 2u,
    PEASHOOTER      = 3u,
    BOMBS           = 4u,
    FIREBALLS       = 5u,
    SWORD           = 6u
};

struct Stat
{
    float value;
    float maxValue;
};

struct Stats
{
    Stat movementSpeed;
    Stat maxHealth;
    Stat healthRegen;
    Stat armor;
    Stat might;
    Stat area;
    Stat weaponSpeed;
    Stat duration;
    Stat amount;
    Stat cooldown;
    Stat luck;
    Stat growth;
    Stat greed;
    Stat curse;
    Stat pickupRange;
    Stat revives;
    Stat reroll;
    Stat skip;
    Stat banish;
};

Stats static PlayerStats = 
{
    {1.0f, FLT_MAX},    //movementSpeed (multipler value)
    {1000.0f, FLT_MAX},    //maxHealth (raw value)
    {0.0f, FLT_MAX},       //healthRegen (raw value)
    {0.0f, FLT_MAX},       //armor (raw value)
    {1.0f, 5.0f},       //might (multiplier value)
    {1.0f, 5.0},        //area (multiplier value)
    {1.0f, 5.0f},       //weaponSpeed (multiplier value)
    {1.0f, 5.0f},       //duration (multiplier value)
    {0.0f, 10.0f},      //amount (in terms of extra projectiles)
    {1.0f, 0.1f},       //cooldown (multiplier value in reverse)
    {1.0f, 5.0f},       //luck (multiplier value)
    {1.0f, 5.0f},       //growth (multiplier value)
    {1.0f, 5.0f},       //greed (multiplier value)
    {1.0f, 5.0f},       //curse (multiplier value)
    {20.0f, FLT_MAX},      //pickupRange (raw value as a radius)
    {0.0f, FLT_MAX},       //revives (raw value)
    {0.0f, FLT_MAX},       //reroll (raw value)
    {0.0f, FLT_MAX},       //skip (raw value)
    {0.0f, FLT_MAX},       //banish (raw value)
};

class PlayerManager : public Canis::ScriptableEntity
{
private:
    float baseMovementSpeed = 100.0f;
    glm::vec2 m_inputDirection;
    glm::vec2 direction;
    Canis::Entity m_healthSlider;
    Canis::Entity m_expSlider;
    int idleId = 0;
    int runId = 0;
    bool wasMoving = false;
    const unsigned int MAXWEAPONS = 5;
    std::vector<Canis::Entity> m_weaponSlotEntities = {};
    std::vector<Canis::Entity> m_weaponSlotIconEntities = {};
    std::vector<unsigned int> m_weaponIDoNotHave = {1,2,5,6};
    float currentXp = 0.0f;
    const float MAXEXP = 1000.0f;
    glm::vec2 lastDirection = glm::vec2(-1.0f, 0.0f);

public:
    void AddWeaponToSlot(unsigned int _weaponType) {
        if (m_weaponSlotIconEntities.size() >= MAXWEAPONS)
            return;
        
        switch(_weaponType) {
            case WeaponType::GASAURA: {
                m_Entity.GetEntityWithTag("GasAura").GetComponent<Canis::RectTransformComponent>().active = true;
                break;
            }
            case WeaponType::HANDOFGOD: {
                m_Entity.GetEntityWithTag("HandOfGod").GetComponent<Canis::RectTransformComponent>().active = true;
                break;
            }
            case WeaponType::ORBITINGSPIKES: {
                m_Entity.GetEntityWithTag("OrbitingSpikes").GetComponent<Canis::RectTransformComponent>().active = true;
                break;
            }
            case WeaponType::PEASHOOTER: {
                m_Entity.GetEntityWithTag("PeaShooter").GetComponent<Canis::RectTransformComponent>().active = true;
                break;
            }
            case WeaponType::BOMBS: {
                m_Entity.GetEntityWithTag("Bomb").GetComponent<Canis::RectTransformComponent>().active = true;
                break;
            }
            case WeaponType::FIREBALLS: {
                m_Entity.GetEntityWithTag("FireBall").GetComponent<Canis::RectTransformComponent>().active = true;
                break;
            }
            case WeaponType::SWORD: {
                m_Entity.GetEntityWithTag("Sword").GetComponent<Canis::RectTransformComponent>().active = true;
                break;
            }

        }

        auto e = CreateEntity();
        m_weaponSlotIconEntities.push_back(e);

        auto& rect = e.AddComponent<Canis::RectTransformComponent>();
        rect = m_weaponSlotEntities[m_weaponSlotIconEntities.size()-1].GetComponent<Canis::RectTransformComponent>();
        rect.position += glm::vec2(8.0f,8.0f);
        rect.size = glm::vec2(32);
        rect.depth = -1.0f;

        auto& color = e.AddComponent<Canis::ColorComponent>();
        color.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        auto& sprite = e.AddComponent<Canis::UIImageComponent>();
        sprite.texture = GetAssetManager().Get<Canis::TextureAsset>(
            GetAssetManager().LoadTexture("assets/textures/weapons/weapon_icons_sprite_sheet.png"))->GetTexture();

        GetUIImageFromTextureAtlas(
            sprite,
            0,
            0,
            _weaponType,
            0,
            32,
            32,
            false,
            false
        );
    }

    void OnCreate() //Awake
    {
        idleId = GetAssetManager().LoadSpriteAnimation("assets/animations/player_idle.anim");
        runId = GetAssetManager().LoadSpriteAnimation("assets/animations/player_run.anim");
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
    }

    void OnReady()//Start
    {
        std::default_random_engine rng(m_Entity.scene->seed);
        std::shuffle(std::begin(m_weaponIDoNotHave), std::end(m_weaponIDoNotHave), rng);
        m_healthSlider = m_Entity.GetEntityWithTag("HealthSlider");
        m_expSlider = m_Entity.GetEntityWithTag("ExpSlider");
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot0"));
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot1"));
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot2"));
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot3"));
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot4"));
        AddWeaponToSlot(0);
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt) //Update
    {
        auto& rect = GetComponent<Canis::RectTransformComponent>();
        auto& anim = GetComponent<Canis::SpriteAnimationComponent>();

        //Canis::CollisionSystem2D *collsionSystem2d = GetSystem<Canis::CollisionSystem2D>();
        //int size = collsionSystem2d->BoxCast(rect.position, rect.size, glm::vec2(0.0f), rect.rotation, Canis::BIT::TWO).size();
        //Canis::Log("Hit : " + std::to_string(size));

        UpdateInput(rect);

        bool moving = false;
        
        if (m_inputDirection.x < 0.0f) //Left
        {
            if(anim.flipX != true)
            {
                anim.flipX = true;
                anim.redraw = true;
            }
        }
        if (m_inputDirection != glm::vec2(0.0f))
        {
            moving = true;
        }
        if (m_inputDirection.x > 0.0f) //Right
        {
            if (anim.flipX != false)
            {
                anim.flipX = false;
                anim.redraw = true;
            }
        }

        direction = (!moving) ? glm::vec2(0.0f) : ( (glm::length(m_inputDirection) > 1.0f) ? glm::normalize(m_inputDirection) : m_inputDirection);
        rect.position += (direction * (baseMovementSpeed * PlayerStats.movementSpeed.value * _dt));

        if (moving && !wasMoving) // change to run
        {
            anim.redraw = true;
            anim.index = 0;
            anim.animationId = runId;
        }

        if (!moving && wasMoving) // idle
        {
            anim.redraw = true;
            anim.index = 0;
            anim.animationId = idleId;
        }

        wasMoving = moving;


        std::vector<entt::entity> hits = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);

        Canis::Entity hitEntity;
        hitEntity.scene = m_Entity.scene;

        for(entt::entity hit : hits)
        {
            hitEntity.entityHandle = hit;

            if (hitEntity.HasComponent<XP>())
            {
                GetAssetManager().Get<Canis::SoundAsset>(GetAssetManager().LoadSound("assets/sounds/pickupCoin.wav"))->Play();
                currentXp += hitEntity.GetComponent<XP>().GetXP();
                m_Entity.scene->entityRegistry.destroy(hit);
            }
        }
    
        m_healthSlider.GetComponent<Canis::UISliderComponent>().value = GetComponent<PlayerHealthComponent>().currentHealth / GetComponent<PlayerHealthComponent>().maxHealth;
        m_expSlider.GetComponent<Canis::UISliderComponent>().value = currentXp / MAXEXP;

        if(currentXp >= MAXEXP)
        {
             
            if (m_weaponSlotIconEntities.size() < 5)
            if (m_weaponIDoNotHave.size() > 0)
            {
                GetAssetManager().Get<Canis::SoundAsset>(GetAssetManager().LoadSound("assets/sounds/powerUp.wav"))->Play();
                currentXp = 0;
                AddWeaponToSlot(m_weaponIDoNotHave[0]);
                m_weaponIDoNotHave.erase(m_weaponIDoNotHave.begin());
            }
        }
    }

    void UpdateInput(Canis::RectTransformComponent &_rect) {
        m_inputDirection = glm::vec2(0.0f);

        if( !GetWindow().GetMouseLock() && (_rect.position.x-(_rect.size.x/2.0f) > ((GetWindow().GetScreenWidth()/2.0f)-50*32.0f)))
        {
            if ((GetInputManager().GetKey(SDL_SCANCODE_A) || GetInputManager().GetKey(SDL_SCANCODE_LEFT) || GetInputManager().GetButton(0, Canis::ControllerButton::DPAD_LEFT))) //Left
            {
                m_inputDirection.x = -1.0f;
            }

            if (GetInputManager().GetLeftStick(0).x < 0.0f)
            {
                m_inputDirection.x = GetInputManager().GetLeftStick(0).x;
            }
        }

        if (!GetWindow().GetMouseLock() && (_rect.position.y+(_rect.size.y/2.0f) < (50*32.0f - (GetWindow().GetScreenHeight()/2.0f))))
        {
            if ((GetInputManager().GetKey(SDL_SCANCODE_W) || GetInputManager().GetKey(SDL_SCANCODE_UP) || GetInputManager().GetButton(0, Canis::ControllerButton::DPAD_UP))) //Forwards
            {
                m_inputDirection.y = 1.0f;
            }

            if (GetInputManager().GetLeftStick(0).y > 0.0f)
            {
                m_inputDirection.y = GetInputManager().GetLeftStick(0).y;
            }
        }

        if(!GetWindow().GetMouseLock() && (_rect.position.x+(_rect.size.x/2.0f) < (50*32.0f - (GetWindow().GetScreenWidth()/2.0f))))
        {
            if ((GetInputManager().GetKey(SDL_SCANCODE_D) || GetInputManager().GetKey(SDL_SCANCODE_RIGHT) || GetInputManager().GetButton(0, Canis::ControllerButton::DPAD_RIGHT))) //Right
            {
                m_inputDirection.x = 1.0f;
            }

            if (GetInputManager().GetLeftStick(0).x > 0.0f)
            {
                m_inputDirection.x = GetInputManager().GetLeftStick(0).x;
            }
        }

        if(!GetWindow().GetMouseLock() && (_rect.position.y-(_rect.size.y/2.0f) > ((GetWindow().GetScreenHeight()/2.0f) - 50*32.0f)))
        {
            if ((GetInputManager().GetKey(SDL_SCANCODE_S) || GetInputManager().GetKey(SDL_SCANCODE_DOWN) || GetInputManager().GetButton(0, Canis::ControllerButton::DPAD_DOWN))) //back
            {
                m_inputDirection.y = -1.0f;
            }

            if (GetInputManager().GetLeftStick(0).y < 0.0f)
            {
                m_inputDirection.y = GetInputManager().GetLeftStick(0).y;
            }
        }

        if (m_inputDirection != glm::vec2(0.0f))
        {
            lastDirection = m_inputDirection;
        }
    }

    glm::vec2 GetInputDirection()
    {
        if (m_inputDirection == glm::vec2(0.0f))
        {
            return lastDirection;
        }
        
        return m_inputDirection;
    }
};