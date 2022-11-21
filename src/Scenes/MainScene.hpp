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

using namespace Canis;

class MainScene : public Scene
{
    private:
        entt::registry entity_registry;

        Shader shader;
        Shader spriteShader;

        RenderMeshSystem *renderMeshSystem;
        RenderSkyboxSystem *renderSkyboxSystem;
        RenderTextSystem *renderTextSystem;
        SpriteRenderer2DSystem *spriteRenderer2DSystem;
        RenderHUDSystem *renderHUDSystem;

        bool firstMouseMove = true;
        bool mouseLock = false;

        int cubeModelId = 0;
        int antonioFontId = 0;

        GLTexture diffuseColorPaletteTexture = {};
        GLTexture specularColorPaletteTexture = {};
        GLTexture supperPupStudioLogoTexture = {};

    public:
        MainScene(std::string _name) : Scene(_name) {}
        ~MainScene()
        {
            delete renderSkyboxSystem;
            delete renderMeshSystem;
            delete renderTextSystem;
            delete spriteRenderer2DSystem;
            delete renderHUDSystem;
        }
        
        void PreLoad()
        {
            Scene::PreLoad();

            // configure global opengl state
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_ALPHA);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
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

            // Load color palette
            diffuseColorPaletteTexture = assetManager->Get<TextureAsset>(
                assetManager->LoadTexture("assets/textures/palette/diffuse.png")
            )->GetTexture();
            specularColorPaletteTexture = assetManager->Get<TextureAsset>(
                assetManager->LoadTexture("assets/textures/palette/specular.png")
            )->GetTexture();

            // load icon
            supperPupStudioLogoTexture = assetManager->Get<TextureAsset>(
                assetManager->LoadTexture("assets/textures/SupperPupStudioLogo.png")
            )->GetTexture();

            // load model
            cubeModelId = assetManager->LoadModel("assets/models/white_block.obj");

            // load font
            antonioFontId = assetManager->LoadText("assets/fonts/Antonio-Bold.ttf", 48);

            renderSkyboxSystem = new RenderSkyboxSystem();
            renderMeshSystem = new RenderMeshSystem();
            renderTextSystem = new RenderTextSystem();
            spriteRenderer2DSystem = new SpriteRenderer2DSystem();
            renderHUDSystem = new RenderHUDSystem();

            ReadySystem(renderSkyboxSystem);
            renderSkyboxSystem->Init();

            ReadySystem(renderTextSystem);
            renderTextSystem->Init();

            renderMeshSystem->shader = &shader;
            ReadySystem(renderMeshSystem);
            renderMeshSystem->diffuseColorPaletteTexture = &diffuseColorPaletteTexture;
            renderMeshSystem->specularColorPaletteTexture = &specularColorPaletteTexture;

            ReadySystem(spriteRenderer2DSystem);
            spriteRenderer2DSystem->Init(GlyphSortType::TEXTURE, &spriteShader);

            ReadySystem(renderHUDSystem);
            renderHUDSystem->Init(GlyphSortType::TEXTURE, &spriteShader);


            // Draw mode
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        void Load()
        {            
            camera->Position = glm::vec3(20.0f,20.0f,-20.0f);
            camera->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            camera->Pitch = PITCH-35.0f;
            camera->Yaw = YAW+135.0f;
            camera->override_camera = false;
            camera->UpdateCameraVectors();
            mouseLock = false;
            window->MouseLock(mouseLock);

            { // light cube
            entt::entity light_entity = entity_registry.create();
            entity_registry.emplace<TransformComponent>(light_entity,
                true, // active
                glm::vec3(-5.0f, 10.0f, -5.0f), // position
                glm::vec3(0.0f, 0.0f, 0.0f), // rotation
                glm::vec3(1, 1, 1) // scale
            );
            entity_registry.emplace<ColorComponent>(light_entity,
                glm::vec4(1.0f,0.0f,0.0f,1.0f)
            );
            entity_registry.emplace<MeshComponent>(light_entity,
                cubeModelId,
                assetManager->Get<ModelAsset>(cubeModelId)->GetVAO(),
                assetManager->Get<ModelAsset>(cubeModelId)->GetSize(),
                false
            );
            entity_registry.emplace<SphereColliderComponent>(light_entity,
                glm::vec3(0.0f),
                1.0f
            );
            }

            { // direction light
            glm::vec3 dir = -glm::normalize(glm::vec3(-5.0f, 10.0f, -5.0f));
            entt::entity directionalLight = entity_registry.create();
            entity_registry.emplace<TransformComponent>(directionalLight,
                true, // active
                glm::vec3(-5.0f, 10.0f, -5.0f), // position
                dir, // rotation
                glm::vec3(1, 1, 1) // scale
            );
            entity_registry.emplace<DirectionalLightComponent>(directionalLight,
                glm::vec3(0.05f, 0.05f, 0.05f), // ambient
                glm::vec3(0.8f, 0.8f, 0.8f), // diffuse
                glm::vec3(0.5f, 0.5f, 0.5f) // specular
            );
            }

            { // cube
            entt::entity cube_entity = entity_registry.create();
            entity_registry.emplace<TransformComponent>(cube_entity,
                true, // active
                glm::vec3(2.0f, 0.0f, 0.0f), // position
                glm::vec3(0.0f, 0.0f, 0.0f), // rotation
                glm::vec3(1, 1, 1) // scale
            );
            entity_registry.emplace<ColorComponent>(cube_entity,
                glm::vec4(0.0f,0.0f,1.0f,1.0f)
            );
            entity_registry.emplace<MeshComponent>(cube_entity,
                cubeModelId,
                assetManager->Get<ModelAsset>(cubeModelId)->GetVAO(),
                assetManager->Get<ModelAsset>(cubeModelId)->GetSize(),
                true
            );
            entity_registry.emplace<SphereColliderComponent>(cube_entity,
                glm::vec3(0.0f),
                1.0f
            );
            }

            { // ground
            entt::entity ground_entity = entity_registry.create();
            entity_registry.emplace<TransformComponent>(ground_entity,
                true, // active
                glm::vec3(0.0f, -0.55f, 0.0f), // position
                glm::vec3(0.0f, 0.0f, 0.0f), // rotation
                glm::vec3(20.0f, 0.2f, 20.0f) // scale
            );
            entity_registry.emplace<ColorComponent>(ground_entity,
                glm::vec4(0.0f,1.0f,0.0f,1.0f)
            );
            entity_registry.emplace<MeshComponent>(ground_entity,
                cubeModelId,
                assetManager->Get<ModelAsset>(cubeModelId)->GetVAO(),
                assetManager->Get<ModelAsset>(cubeModelId)->GetSize(),
                false
            );
            entity_registry.emplace<SphereColliderComponent>(ground_entity,
                glm::vec3(0.0f),
                1.0f
            );
            }

            { // health text
            entt::entity healthText = entity_registry.create();
            entity_registry.emplace<RectTransformComponent>(healthText,
                true, // active
                glm::vec2(25.0f, window->GetScreenHeight() - 65.0f), // position
                glm::vec2(0.0f,0.0f), // size
                glm::vec2(0.0f),
                0.0f, // rotation
                1.0f, // scale
                0.0f // depth
            );
            entity_registry.emplace<ColorComponent>(healthText,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) // #26854c
            );
            entity_registry.emplace<TextComponent>(healthText,
                assetManager->LoadText("assets/fonts/Antonio-Bold.ttf", 48),
                new std::string("Asset Manager Demo") // text
            );
            }

            { // sprite test supperPupStudioLogoTexture
            entt::entity spriteEntity = entity_registry.create();
            entity_registry.emplace<RectTransformComponent>(spriteEntity,
                true, // active
                glm::vec2(0.0f, 0.0f), // position
                glm::vec2(supperPupStudioLogoTexture.width/4,supperPupStudioLogoTexture.height/4), // size
                glm::vec2(0.0f),
                0.0f, // rotation
                1.0f, // scale
                0.0f // depth
            );
            entity_registry.emplace<ColorComponent>(spriteEntity,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            entity_registry.emplace<UIImageComponent>(spriteEntity,
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // uv
                supperPupStudioLogoTexture // texture
            );// test
            }

            { // sprite test supperPupStudioLogoTexture
            entt::entity spriteEntity = entity_registry.create();
            entity_registry.emplace<RectTransformComponent>(spriteEntity,
                true, // active
                glm::vec2(100.0f, 400.0f), // position
                glm::vec2(diffuseColorPaletteTexture.width/4,diffuseColorPaletteTexture.height/4), // size
                glm::vec2(0.0f),
                0.0f, // rotation
                1.0f, // scale
                0.0f // depth
            );
            entity_registry.emplace<ColorComponent>(spriteEntity,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            entity_registry.emplace<UIImageComponent>(spriteEntity,
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // uv
                diffuseColorPaletteTexture // texture
            );// test
            }
        }

        void UnLoad()
        {
            Log("Canis Clear");
            entity_registry.clear();
        }

        void Update()
        {
            
        }

        void LateUpdate()
        {
            const Uint8 *keystate = SDL_GetKeyboardState(NULL);

            if (keystate[SDL_SCANCODE_W] && mouseLock)
            {
                camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
            }

            if (keystate[SDL_SCANCODE_S] && mouseLock)
            {
                camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
            }

            if (keystate[SDL_SCANCODE_A] && mouseLock)
            {
                camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
            }

            if (keystate[SDL_SCANCODE_D] && mouseLock)
            {
                camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
            }

            if (inputManager->justPressedKey(SDLK_ESCAPE))
            {
                mouseLock = !mouseLock;
                //camera->override_camera = !camera->override_camera;

                window->MouseLock(mouseLock);
            }

            if (inputManager->justPressedKey(SDLK_F5))
            {
                Log("Load Scene");
                ((SceneManager*)sceneManager)->Load("MainScene");
            }
        }

        void Draw()
        {
            glDepthFunc(GL_LESS);
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

            renderSkyboxSystem->UpdateComponents(deltaTime, entity_registry);
            renderMeshSystem->UpdateComponents(deltaTime, entity_registry);
            renderHUDSystem->UpdateComponents(deltaTime, entity_registry);
            renderTextSystem->UpdateComponents(deltaTime, entity_registry);
            spriteRenderer2DSystem->UpdateComponents(deltaTime, entity_registry);
            

            window->SetWindowName("Canis : Template | fps : " + std::to_string(int(window->fps))
            + " deltaTime : " + std::to_string(deltaTime)
            + " Enitity : " + std::to_string(entity_registry.size())
            + " Rendered : " + std::to_string(renderMeshSystem->entities_rendered));
        }

        void InputUpdate()
        {

        }
};