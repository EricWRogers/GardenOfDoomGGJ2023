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

class PlayerManager : public Canis::ScriptableEntity
{
private:
   float speed = 100.0f;
    glm::vec2 direction;
    Canis::Entity m_healthSlider;
    Canis::Entity m_expSlider;
    int idleId = 0;
    int runId = 0;
    bool wasMoving = false;
    const unsigned int MAXWEAPONS = 5;
    std::vector<Canis::Entity> m_weaponSlotEntities = {};
    std::vector<Canis::Entity> m_weaponSlotIconEntities = {};
    std::vector<unsigned int> m_weaponIDoNotHave = {4,5,6,1,0};
    float currentXp = 0.0f;
    const float MAXEXP = 1000.0f;

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
        AddWeaponToSlot(3);
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt) //Update
    {
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        
        float horizontal = 0.0f;
        float vertical = 0.0f;

        auto& rect = GetComponent<Canis::RectTransformComponent>();
        auto& anim = GetComponent<Canis::SpriteAnimationComponent>();

        bool moving = false;
        
        
        if ((keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) && !GetWindow().GetMouseLock() && (rect.position.x-(rect.size.x/2.0f) > ((GetWindow().GetScreenWidth()/2.0f)-50*32.0f))) //Left
        {
            moving = true;
            horizontal = -1.0f;
            if(anim.flipX != true)
            {
                anim.flipX = true;
                anim.redraw = true;
            }
        }

        if ((keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) && !GetWindow().GetMouseLock() && (rect.position.y+(rect.size.y/2.0f) < (50*32.0f - (GetWindow().GetScreenHeight()/2.0f)))) //Forwards
        {
            moving = true;
            vertical = 1.0f;
        }
        if ((keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) && !GetWindow().GetMouseLock() && (rect.position.x+(rect.size.x/2.0f) < (50*32.0f - (GetWindow().GetScreenWidth()/2.0f)))) //Right
        {
            moving = true;
            horizontal = 1.0f;
            if (anim.flipX != false)
            {
                anim.flipX = false;
                anim.redraw = true;
            }
        }
        if ((keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) && !GetWindow().GetMouseLock() && (rect.position.y-(rect.size.y/2.0f) > ((GetWindow().GetScreenHeight()/2.0f) - 50*32.0f))) //back
        {
            moving = true;
            vertical = -1.0f;
        }

        direction = (glm::vec2(horizontal, vertical) == glm::vec2(0.0f)) ? glm::vec2(0.0f) : glm::normalize(glm::vec2(horizontal, vertical));
        rect.position += (direction * (speed * _dt));

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
};