#pragma once
#include <SDL_keyboard.h>

class DebugCamera2D : public Canis::ScriptableEntity
{
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {

    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        const float CAMERA_SPEED = 20.0f;
        const float SCALE_SPEED = 0.1f;

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        auto& camera2D = GetComponent<Canis::Camera2DComponent>();

        if (!GetWindow().GetMouseLock())
        {
            auto player = m_Entity.GetEntityWithTag("Player");
            if(player.entityHandle != entt::null)
            {
                auto& playerTransform = player.GetComponent<Canis::RectTransformComponent>();
                camera2D.position = playerTransform.position;
            }
        }
        
        if (keystate[SDL_SCANCODE_W] && GetWindow().GetMouseLock())
        {
            camera2D.position.y = camera2D.position.y + (CAMERA_SPEED * _dt);
        }
        if (keystate[SDL_SCANCODE_S] && GetWindow().GetMouseLock())
        {
            camera2D.position.y -= CAMERA_SPEED * _dt;
        }

        if (keystate[SDL_SCANCODE_A] && GetWindow().GetMouseLock())
        {
            camera2D.position.x -= CAMERA_SPEED * _dt;
        }

        if (keystate[SDL_SCANCODE_D] && GetWindow().GetMouseLock())
        {
            camera2D.position.x += CAMERA_SPEED * _dt;
        }

        if (keystate[SDL_SCANCODE_Q] && GetWindow().GetMouseLock())
        {
            camera2D.scale += SCALE_SPEED * _dt;
        }

        if (keystate[SDL_SCANCODE_E] && GetWindow().GetMouseLock())
        {
            camera2D.scale -= SCALE_SPEED * _dt;
            if (camera2D.scale <= 0.01f)
                camera2D.scale = 0.01f;
        }

        if (GetInputManager().JustPressedKey(SDLK_F8))
        {
            GetWindow().MouseLock(!GetWindow().GetMouseLock());
        }

        if (GetInputManager().JustPressedKey(SDLK_F5))
        {
            Canis::Log("Load Scene");
            ((Canis::SceneManager*)m_Entity.scene->sceneManager)->HotReload();
        }
    }
};

bool DecodeDebugCamera2D(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "DebugCamera2D")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<DebugCamera2D>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}