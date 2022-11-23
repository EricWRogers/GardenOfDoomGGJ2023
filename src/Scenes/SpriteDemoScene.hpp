#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <math.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Canis/Canis.hpp>
#include <Canis/Debug.hpp>
#include <Canis/Math.hpp>
#include <Canis/Time.hpp>
#include <Canis/Window.hpp>
#include <Canis/Shader.hpp>
#include <Canis/Camera.hpp>
#include <Canis/IOManager.hpp>
#include <Canis/InputManager.hpp>
#include <Canis/Scene.hpp>
#include <Canis/SceneManager.hpp>
#include <Canis/AssetManager.hpp>
#include <Canis/Data/GLTexture.hpp>
#include <Canis/External/entt.hpp>
#include <Canis/GameHelper/AStar.hpp>

#include <Canis/ECS/Systems/RenderMeshSystem.hpp>
#include <Canis/ECS/Systems/RenderSkyboxSystem.hpp>
#include <Canis/ECS/Systems/RenderTextSystem.hpp>
#include <Canis/ECS/Systems/SpriteRenderer2DSystem.hpp>
#include <Canis/ECS/Systems/RenderHUDSystem.hpp>

#include <Canis/ECS/Components/TransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>
#include <Canis/ECS/Components/MeshComponent.hpp>
#include <Canis/ECS/Components/SphereColliderComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include <Canis/ECS/Components/UIImageComponent.hpp>
#include <Canis/ECS/Components/Camera2DComponent.hpp>

class SpriteDemoScene : public Canis::Scene
{
    private:
        entt::registry entity_registry;

        Canis::Shader shader;
        Canis::Shader spriteShader;

        Canis::RenderTextSystem *renderTextSystem;
        Canis::SpriteRenderer2DSystem *spriteRenderer2DSystem;
        Canis::RenderHUDSystem *renderHUDSystem;

        bool firstMouseMove = true;
        bool mouseLock = false;

        int cubeModelId = 0;
        int antonioFontId = 0;

        Canis::GLTexture supperPupStudioLogoTexture = {};

        entt::entity camera2DEntt;

    public:
        SpriteDemoScene(std::string _name) : Canis::Scene(_name) {}
        ~SpriteDemoScene()
        {
            delete renderTextSystem;
            delete spriteRenderer2DSystem;
            delete renderHUDSystem;
        }
        
        void PreLoad()
        {
            Canis::Scene::PreLoad();

            // configure global opengl state
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_ALPHA);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthFunc(GL_LESS);
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // glEnable(GL_CULL_FACE);
            // build and compile our shader program
            shader.Compile("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");
            shader.AddAttribute("aPos");
            shader.AddAttribute("aNormal");
            shader.AddAttribute("aTexcoords");
            shader.Link();

            spriteShader.Compile(
                "assets/shaders/sprite.vs",
                "assets/shaders/sprite.fs"
            );
            spriteShader.AddAttribute("vertexPosition");
            spriteShader.AddAttribute("vertexColor");
            spriteShader.AddAttribute("vertexUV");
            spriteShader.Link();

            // load icon
            supperPupStudioLogoTexture = assetManager->Get<Canis::TextureAsset>(
                assetManager->LoadTexture("assets/textures/SupperPupStudioLogo.png")
            )->GetTexture();

            // load model
            cubeModelId = assetManager->LoadModel("assets/models/white_block.obj");

            // load font
            antonioFontId = assetManager->LoadText("assets/fonts/Antonio-Bold.ttf", 48);

            renderTextSystem = new Canis::RenderTextSystem();
            spriteRenderer2DSystem = new Canis::SpriteRenderer2DSystem();
            renderHUDSystem = new Canis::RenderHUDSystem();

            ReadySystem(renderTextSystem);
            renderTextSystem->Init();

            ReadySystem(spriteRenderer2DSystem);
            spriteRenderer2DSystem->Init(Canis::GlyphSortType::TEXTURE, &spriteShader);

            ReadySystem(renderHUDSystem);
            renderHUDSystem->Init(Canis::GlyphSortType::TEXTURE, &spriteShader);

            // Draw mode
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        void Load()
        {            
            camera->Position = glm::vec3(20.0f,20.0f,-20.0f);
            camera->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            camera->Pitch = Canis::PITCH-35.0f;
            camera->Yaw = Canis::YAW+135.0f;
            camera->override_camera = false;
            camera->UpdateCameraVectors();
            mouseLock = true;
            window->MouseLock(mouseLock);

            { // camera 2D
            camera2DEntt = entity_registry.create();
            entity_registry.emplace<Canis::Camera2DComponent>(camera2DEntt,
                glm::vec2(0.0f,0.0f), // position
                1.0f // scale
            );
            }

            { // demo text
            entt::entity healthText = entity_registry.create();
            entity_registry.emplace<Canis::RectTransformComponent>(healthText,
                true, // active
                glm::vec2(25.0f, window->GetScreenHeight() - 65.0f), // position
                glm::vec2(0.0f,0.0f), // size
                glm::vec2(0.0f),
                0.0f, // rotation
                1.0f, // scale
                0.0f // depth
            );
            entity_registry.emplace<Canis::ColorComponent>(healthText,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) // #26854c
            );
            entity_registry.emplace<Canis::TextComponent>(healthText,
                assetManager->LoadText("assets/fonts/Antonio-Bold.ttf", 48),
                new std::string("Sprite Demo") // text
            );
            }

            { // sprite test supperPupStudioLogoTexture
            glm::vec2 size = glm::vec2(supperPupStudioLogoTexture.width/4,supperPupStudioLogoTexture.height/4);
            entt::entity spriteEntity = entity_registry.create();
            entity_registry.emplace<Canis::RectTransformComponent>(spriteEntity,
                true, // active
                glm::vec2(0.0f,0.0f), // position
                size, // size
                glm::vec2(0.0f,0.0f), // origin
                0.0f, // rotation
                1.0f, // scale
                0.0f // depth
            );
            entity_registry.emplace<Canis::ColorComponent>(spriteEntity,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            entity_registry.emplace<Canis::Sprite2DComponent>(spriteEntity,
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // uv
                supperPupStudioLogoTexture // texture
            );// test
            }
        }

        void UnLoad()
        {
            entity_registry.clear();
        }

        void Update()
        {
            
        }

        void LateUpdate()
        {
            const float CAMERA_SPEED = 20.0f;
            const float SCALE_SPEED = 0.1f;

            const Uint8 *keystate = SDL_GetKeyboardState(NULL);

            auto cam = entity_registry.view<Canis::Camera2DComponent>();
            for(auto[entity, camera2D] : cam.each()) {

                //Canis::Log("Camera Entt X : " + std::to_string(camera2D.position.x) +
                //    " Y : " + std::to_string(camera2D.position.y) +
                //   " S : " + std::to_string(camera2D.scale));

                if (keystate[SDL_SCANCODE_W] && mouseLock)
                {
                    camera2D.position.y = camera2D.position.y + (CAMERA_SPEED * deltaTime);
                }
                if (keystate[SDL_SCANCODE_S] && mouseLock)
                {
                    camera2D.position.y -= CAMERA_SPEED * deltaTime;
                }

                if (keystate[SDL_SCANCODE_A] && mouseLock)
                {
                    camera2D.position.x -= CAMERA_SPEED * deltaTime;
                }

                if (keystate[SDL_SCANCODE_D] && mouseLock)
                {
                    camera2D.position.x += CAMERA_SPEED * deltaTime;
                }

                if (keystate[SDL_SCANCODE_Q] && mouseLock)
                {
                    camera2D.scale += SCALE_SPEED * deltaTime;
                }

                if (keystate[SDL_SCANCODE_E] && mouseLock)
                {
                    camera2D.scale -= SCALE_SPEED * deltaTime;
                    if (camera2D.scale <= 0.01f)
                        camera2D.scale = 0.01f;
                }
                continue;
            }

            

            if (inputManager->justPressedKey(SDLK_ESCAPE))
            {
                mouseLock = !mouseLock;

                window->MouseLock(mouseLock);
            }

            if (inputManager->justPressedKey(SDLK_F5))
            {
                Canis::Log("Load Scene");
                ((Canis::SceneManager*)sceneManager)->Load("MainScene");
            }
        }

        void Draw()
        {
            spriteRenderer2DSystem->UpdateComponents(deltaTime, entity_registry);
            renderTextSystem->UpdateComponents(deltaTime, entity_registry);
            

            window->SetWindowName("Canis : Template | fps : " + std::to_string(int(window->fps))
            + " deltaTime : " + std::to_string(deltaTime));
        }

        void InputUpdate()
        {

        }
};