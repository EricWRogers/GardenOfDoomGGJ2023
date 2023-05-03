#pragma once
#include <SDL_keycode.h>
#include <random>
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
    SWORD           = 6u,
    FINAL           = 7u    //make sure is the last element
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
    {100.0f, FLT_MAX},    //maxHealth (raw value)
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

struct Character
{
    std::string name;
    std::string weaponIconPath;
    std::string characterIconPath;
    WeaponType startingWeapon;
    Stats startingStats = 
    {
    {1.0f, FLT_MAX},       //movementSpeed (multipler value)
    {1000.0f, FLT_MAX},    //maxHealth (raw value)
    {0.0f, FLT_MAX},       //healthRegen (raw value)
    {0.0f, FLT_MAX},       //armor (raw value)
    {1.0f, 5.0f},          //might (multiplier value)
    {1.0f, 5.0},           //area (multiplier value)
    {1.0f, 5.0f},          //weaponSpeed (multiplier value)
    {1.0f, 5.0f},          //duration (multiplier value)
    {0.0f, 10.0f},         //amount (in terms of extra projectiles)
    {1.0f, 0.1f},          //cooldown (multiplier value in reverse)
    {1.0f, 5.0f},          //luck (multiplier value)
    {1.0f, 5.0f},          //growth (multiplier value)
    {1.0f, 5.0f},          //greed (multiplier value)
    {1.0f, 5.0f},          //curse (multiplier value)
    {20.0f, FLT_MAX},      //pickupRange (raw value as a radius)
    {0.0f, FLT_MAX},       //revives (raw value)
    {0.0f, FLT_MAX},       //reroll (raw value)
    {0.0f, FLT_MAX},       //skip (raw value)
    {0.0f, FLT_MAX},       //banish (raw value)
};
};

class PlayerManager : public Canis::ScriptableEntity
{
private:
    float baseMovementSpeed = 90.0f;
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
    float currentXp = 0.0f;
    float MAXEXP = 125.0f;
    float xpIncreaseLow = 200.0f;
    float xpIncreaseMid = 550.0f;
    glm::vec2 lastDirection = glm::vec2(-1.0f, 0.0f);

public:
    std::vector<unsigned int> weaponIDoNotHave = {};
    std::function<void()> levelUpEvent = nullptr;
    bool holdingSeed = false;
    Canis::Entity seed;
    Character character;
    int level = 0;

    void CurveExp()
    {
        if (level == 0)
        {
            return;
        }

        if (level == 1)
        {
            MAXEXP += xpIncreaseLow;
        }

        if (level == 3)
        {
            MAXEXP += xpIncreaseMid;
        }
    }

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
    }

    virtual void OnReady()//Start
    {
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
        seed.scene = m_Entity.scene;
        std::default_random_engine rng(m_Entity.scene->seed);
        std::shuffle(std::begin(weaponIDoNotHave), std::end(weaponIDoNotHave), rng);
        m_healthSlider = m_Entity.GetEntityWithTag("HealthSlider");
        m_expSlider = m_Entity.GetEntityWithTag("ExpSlider");
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot0"));
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot1"));
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot2"));
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot3"));
        m_weaponSlotEntities.push_back(m_Entity.GetEntityWithTag("WeaponSlot4"));
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt) //Update
    {
        auto& rect = GetComponent<Canis::RectTransformComponent>();
        auto& anim = GetComponent<Canis::SpriteAnimationComponent>();

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
            if (weaponIDoNotHave.size() > 0)
            {
                level++;
                //curve xp
                CurveExp();
                GetAssetManager().Get<Canis::SoundAsset>(GetAssetManager().LoadSound("assets/sounds/powerUp.wav"))->Play();
                currentXp = 0;
                int wIndex = 0;

                if (levelUpEvent != nullptr)
                {
                    if (weaponIDoNotHave.size() > 1)
                    {
                        std::default_random_engine rng(m_Entity.scene->seed);
                        std::shuffle(std::begin(weaponIDoNotHave), std::end(weaponIDoNotHave), rng);
                    }

                    levelUpEvent();
                }
                else
                {
                    AddWeaponToSlot(weaponIDoNotHave[wIndex]);
                    weaponIDoNotHave.erase(weaponIDoNotHave.begin()+wIndex);
                }
            }
        }

        ManageSeed();
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

    void ManageSeed()
    {
        glm::vec2 holdPos = glm::vec2(GetComponent<Canis::RectTransformComponent>().position.x, 
            GetComponent<Canis::RectTransformComponent>().position.y + 50.0f);

        std::vector<entt::entity> hits = GetSystem<Canis::CollisionSystem2D>()->GetHits(m_Entity.entityHandle);

        if (holdingSeed)
        {
            seed.GetComponent<Canis::RectTransformComponent>().position = holdPos;
        }

        if (!holdingSeed)
        {
            for(int i = 0; i < hits.size(); i++)
            {
                seed.entityHandle = hits[i];
                if (seed.HasComponent<SeedComponent>())
                {
                    seed.GetComponent<Canis::RectTransformComponent>().position = holdPos;
                    holdingSeed = true;
                    break;
                }
            }
        }
    }
};

class Ned : public PlayerManager
{
    public:
    void OnReady() override
    {
        PlayerManager::OnReady();

        Character character = {
            "Ned",
            "Tempweaponpath",
            "Tempcharacterpath",
            GASAURA,
            {
            {0.9f, FLT_MAX},       //movementSpeed (multipler value)
            {80.0f, FLT_MAX},    //maxHealth (raw value)
            {0.0f, FLT_MAX},       //healthRegen (raw value)
            {0.0f, FLT_MAX},       //armor (raw value)
            {1.0f, 5.0f},          //might (multiplier value)
            {1.2f, 5.0},           //area (multiplier value)
            {1.0f, 5.0f},          //weaponSpeed (multiplier value)
            {1.0f, 5.0f},          //duration (multiplier value)
            {0.0f, 10.0f},         //amount (in terms of extra projectiles)
            {1.1f, 0.1f},          //cooldown (multiplier value in reverse)
            {1.0f, 5.0f},          //luck (multiplier value)
            {0.8f, 5.0f},          //growth (multiplier value)
            {1.0f, 5.0f},          //greed (multiplier value)
            {1.0f, 5.0f},          //curse (multiplier value)
            {25.0f, FLT_MAX},      //pickupRange (raw value as a radius)
            {0.0f, FLT_MAX},       //revives (raw value)
            {0.0f, FLT_MAX},       //reroll (raw value)
            {0.0f, FLT_MAX},       //skip (raw value)
            {0.0f, FLT_MAX},       //banish (raw value)
            }
        };

        PlayerStats = character.startingStats;
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
        GetComponent<PlayerHealthComponent>().currentHealth = GetComponent<PlayerHealthComponent>().maxHealth;

        AddWeaponToSlot(character.startingWeapon);
        
        for(int i = 0; i < FINAL; i++)
        {
            weaponIDoNotHave.push_back(i);
        }
        weaponIDoNotHave.erase(weaponIDoNotHave.begin() + character.startingWeapon);
    }
};

class Nathanial : public PlayerManager
{
    public:
    void OnReady() override
    {
        PlayerManager::OnReady();

        Character character = {
            "Nathanial",
            "Tempweaponpath",
            "Tempcharacterpath",
            HANDOFGOD,
            {
            {1.0f, FLT_MAX},       //movementSpeed (multipler value)
            {100.0f, FLT_MAX},    //maxHealth (raw value)
            {0.0f, FLT_MAX},       //healthRegen (raw value)
            {0.0f, FLT_MAX},       //armor (raw value)
            {1.15f, 5.0f},          //might (multiplier value)
            {0.95f, 5.0},           //area (multiplier value)
            {0.9f, 5.0f},          //weaponSpeed (multiplier value)
            {1.0f, 5.0f},          //duration (multiplier value)
            {0.0f, 10.0f},         //amount (in terms of extra projectiles)
            {0.9f, 0.1f},          //cooldown (multiplier value in reverse)
            {1.5f, 5.0f},          //luck (multiplier value)
            {1.0f, 5.0f},          //growth (multiplier value)
            {0.8f, 5.0f},          //greed (multiplier value)
            {1.1f, 5.0f},          //curse (multiplier value)
            {20.0f, FLT_MAX},      //pickupRange (raw value as a radius)
            {0.0f, FLT_MAX},       //revives (raw value)
            {0.0f, FLT_MAX},       //reroll (raw value)
            {0.0f, FLT_MAX},       //skip (raw value)
            {0.0f, FLT_MAX},       //banish (raw value)
            }
        };

        PlayerStats = character.startingStats;
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
        GetComponent<PlayerHealthComponent>().currentHealth = GetComponent<PlayerHealthComponent>().maxHealth;

        AddWeaponToSlot(character.startingWeapon);

        for(int i = 0; i < FINAL; i++)
        {
            weaponIDoNotHave.push_back(i);
        }
        weaponIDoNotHave.erase(weaponIDoNotHave.begin() + character.startingWeapon);
    }
};

class Rex : public PlayerManager
{
    public:
    void OnReady() override
    {
        PlayerManager::OnReady();

        Character character = {
            "Rex",
            "Tempweaponpath",
            "Tempcharacterpath",
            BOMBS,
            {
            {1.0f, FLT_MAX},       //movementSpeed (multipler value)
            {125.0f, FLT_MAX},    //maxHealth (raw value)
            {0.0f, FLT_MAX},       //healthRegen (raw value)
            {0.0f, FLT_MAX},       //armor (raw value)
            {1.2f, 5.0f},          //might (multiplier value)
            {1.1f, 5.0},           //area (multiplier value)
            {0.85f, 5.0f},          //weaponSpeed (multiplier value)
            {0.9f, 5.0f},          //duration (multiplier value)
            {0.0f, 10.0f},         //amount (in terms of extra projectiles)
            {1.15f, 0.1f},          //cooldown (multiplier value in reverse)
            {1.0f, 5.0f},          //luck (multiplier value)
            {1.0f, 5.0f},          //growth (multiplier value)
            {1.05f, 5.0f},          //greed (multiplier value)
            {1.0f, 5.0f},          //curse (multiplier value)
            {18.0f, FLT_MAX},      //pickupRange (raw value as a radius)
            {0.0f, FLT_MAX},       //revives (raw value)
            {0.0f, FLT_MAX},       //reroll (raw value)
            {0.0f, FLT_MAX},       //skip (raw value)
            {0.0f, FLT_MAX},       //banish (raw value)
            }
        };

        PlayerStats = character.startingStats;
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
        GetComponent<PlayerHealthComponent>().currentHealth = GetComponent<PlayerHealthComponent>().maxHealth;

        AddWeaponToSlot(character.startingWeapon);

        for(int i = 0; i < FINAL; i++)
        {
            weaponIDoNotHave.push_back(i);
        }
        weaponIDoNotHave.erase(weaponIDoNotHave.begin() + character.startingWeapon);
    }
};

class Amber : public PlayerManager
{
    public:
    void OnReady() override
    {
        PlayerManager::OnReady();

        Character character = {
            "Amber",
            "Tempweaponpath",
            "Tempcharacterpath",
            ORBITINGSPIKES,
            {
            {1.1f, FLT_MAX},       //movementSpeed (multipler value)
            {90.0f, FLT_MAX},    //maxHealth (raw value)
            {0.0f, FLT_MAX},       //healthRegen (raw value)
            {0.0f, FLT_MAX},       //armor (raw value)
            {0.85f, 5.0f},          //might (multiplier value)
            {1.1f, 5.0},           //area (multiplier value)
            {1.1f, 5.0f},          //weaponSpeed (multiplier value)
            {1.0f, 5.0f},          //duration (multiplier value)
            {0.0f, 10.0f},         //amount (in terms of extra projectiles)
            {0.85f, 0.1f},          //cooldown (multiplier value in reverse)
            {1.05f, 5.0f},          //luck (multiplier value)
            {1.0f, 5.0f},          //growth (multiplier value)
            {0.8f, 5.0f},          //greed (multiplier value)
            {1.05f, 5.0f},          //curse (multiplier value)
            {20.0f, FLT_MAX},      //pickupRange (raw value as a radius)
            {0.0f, FLT_MAX},       //revives (raw value)
            {0.0f, FLT_MAX},       //reroll (raw value)
            {0.0f, FLT_MAX},       //skip (raw value)
            {0.0f, FLT_MAX},       //banish (raw value)
            }
        };

        PlayerStats = character.startingStats;
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
        GetComponent<PlayerHealthComponent>().currentHealth = GetComponent<PlayerHealthComponent>().maxHealth;

        AddWeaponToSlot(character.startingWeapon);
        
        for(int i = 0; i < FINAL; i++)
        {
            weaponIDoNotHave.push_back(i);
        }
        weaponIDoNotHave.erase(weaponIDoNotHave.begin() + character.startingWeapon);
    }
};

class David : public PlayerManager
{
    public:
    void OnReady() override
    {
        PlayerManager::OnReady();

        Character character = {
            "David",
            "Tempweaponpath",
            "Tempcharacterpath",
            PEASHOOTER,
            {
            {1.0f, FLT_MAX},       //movementSpeed (multipler value)
            {70.0f, FLT_MAX},    //maxHealth (raw value)
            {0.0f, FLT_MAX},       //healthRegen (raw value)
            {0.0f, FLT_MAX},       //armor (raw value)
            {1.5f, 5.0f},          //might (multiplier value)
            {0.9f, 5.0},           //area (multiplier value)
            {1.25f, 5.0f},          //weaponSpeed (multiplier value)
            {0.8f, 5.0f},          //duration (multiplier value)
            {0.0f, 10.0f},         //amount (in terms of extra projectiles)
            {1.25f, 0.1f},          //cooldown (multiplier value in reverse)
            {2.0f, 5.0f},          //luck (multiplier value)
            {1.1f, 5.0f},          //growth (multiplier value)
            {0.75f, 5.0f},          //greed (multiplier value)
            {1.2f, 5.0f},          //curse (multiplier value)
            {16.0f, FLT_MAX},      //pickupRange (raw value as a radius)
            {0.0f, FLT_MAX},       //revives (raw value)
            {0.0f, FLT_MAX},       //reroll (raw value)
            {0.0f, FLT_MAX},       //skip (raw value)
            {0.0f, FLT_MAX},       //banish (raw value)
            }
        };

        PlayerStats = character.startingStats;
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
        GetComponent<PlayerHealthComponent>().currentHealth = GetComponent<PlayerHealthComponent>().maxHealth;

        AddWeaponToSlot(character.startingWeapon);
        
        for(int i = 0; i < FINAL; i++)
        {
            weaponIDoNotHave.push_back(i);
        }
        weaponIDoNotHave.erase(weaponIDoNotHave.begin() + character.startingWeapon);
    }
};

class Ash : public PlayerManager
{
    public:
    void OnReady() override
    {
        PlayerManager::OnReady();

        Character character = {
            "Ash",
            "Tempweaponpath",
            "Tempcharacterpath",
            FIREBALLS,
            {
            {0.95f, FLT_MAX},       //movementSpeed (multipler value)
            {95.0f, FLT_MAX},    //maxHealth (raw value)
            {0.0f, FLT_MAX},       //healthRegen (raw value)
            {0.0f, FLT_MAX},       //armor (raw value)
            {1.2f, 5.0f},          //might (multiplier value)
            {0.9f, 5.0},           //area (multiplier value)
            {1.25f, 5.0f},          //weaponSpeed (multiplier value)
            {0.9f, 5.0f},          //duration (multiplier value)
            {0.0f, 10.0f},         //amount (in terms of extra projectiles)
            {1.15f, 0.1f},          //cooldown (multiplier value in reverse)
            {1.15f, 5.0f},          //luck (multiplier value)
            {1.0f, 5.0f},          //growth (multiplier value)
            {1.15f, 5.0f},          //greed (multiplier value)
            {1.2f, 5.0f},          //curse (multiplier value)
            {15.0f, FLT_MAX},      //pickupRange (raw value as a radius)
            {0.0f, FLT_MAX},       //revives (raw value)
            {0.0f, FLT_MAX},       //reroll (raw value)
            {0.0f, FLT_MAX},       //skip (raw value)
            {0.0f, FLT_MAX},       //banish (raw value)
            }
        };

        PlayerStats = character.startingStats;
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
        GetComponent<PlayerHealthComponent>().currentHealth = GetComponent<PlayerHealthComponent>().maxHealth;
        AddWeaponToSlot(character.startingWeapon);
        
        for(int i = 0; i < FINAL; i++)
        {
            weaponIDoNotHave.push_back(i);
        }
        weaponIDoNotHave.erase(weaponIDoNotHave.begin() + character.startingWeapon);
    }
};

class Tobias : public PlayerManager
{
    public:
    void OnReady() override
    {
        PlayerManager::OnReady();

        Character character = {
            "Tobias",
            "Tempweaponpath",
            "Tempcharacterpath",
            SWORD,
            {
            {1.6f, FLT_MAX},       //movementSpeed (multipler value)
            {115.0f, FLT_MAX},    //maxHealth (raw value)
            {0.0f, FLT_MAX},       //healthRegen (raw value)
            {0.0f, FLT_MAX},       //armor (raw value)
            {0.95f, 5.0f},          //might (multiplier value)
            {0.95f, 5.0},           //area (multiplier value)
            {1.35f, 5.0f},          //weaponSpeed (multiplier value)
            {0.95f, 5.0f},          //duration (multiplier value)
            {0.0f, 10.0f},         //amount (in terms of extra projectiles)
            {0.85f, 0.1f},          //cooldown (multiplier value in reverse)
            {0.9f, 5.0f},          //luck (multiplier value)
            {1.1f, 5.0f},          //growth (multiplier value)
            {1.05f, 5.0f},          //greed (multiplier value)
            {1.3f, 5.0f},          //curse (multiplier value)
            {25.0f, FLT_MAX},      //pickupRange (raw value as a radius)
            {0.0f, FLT_MAX},       //revives (raw value)
            {0.0f, FLT_MAX},       //reroll (raw value)
            {0.0f, FLT_MAX},       //skip (raw value)
            {0.0f, FLT_MAX},       //banish (raw value)
            }
        };

        PlayerStats = character.startingStats;
        GetComponent<PlayerHealthComponent>().maxHealth = PlayerStats.maxHealth.value;
        GetComponent<PlayerHealthComponent>().currentHealth = GetComponent<PlayerHealthComponent>().maxHealth;

        AddWeaponToSlot(character.startingWeapon);
        
        for(int i = 0; i < FINAL; i++)
        {
            weaponIDoNotHave.push_back(i);
        }
        weaponIDoNotHave.erase(weaponIDoNotHave.begin() + character.startingWeapon);
    }
};

bool DecodePlayerManager(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "PlayerManager")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<PlayerManager>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}

bool DecodeCharacter1(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "Ned")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<Ned>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}

bool DecodeCharacter2(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "Nathanial")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<Nathanial>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}

bool DecodeCharacter3(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "Rex")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<Rex>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}

bool DecodeCharacter4(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "Amber")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<Amber>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}

bool DecodeCharacter5(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "David")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<David>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}

bool DecodeCharacter6(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "Ash")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<Ash>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}

bool DecodeCharacter7(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "Tobias")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<Tobias>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}