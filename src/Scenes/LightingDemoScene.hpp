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

#include <Canis/ECS/Systems/RenderHDRSystem.hpp>
#include <Canis/ECS/Systems/RenderMeshSystem.hpp>
#include <Canis/ECS/Systems/RenderSkyboxSystem.hpp>
#include <Canis/ECS/Systems/RenderTextSystem.hpp>
#include <Canis/ECS/Systems/SpriteRenderer2DSystem.hpp>

#include <Canis/ECS/Components/TransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>
#include <Canis/ECS/Components/MeshComponent.hpp>
#include <Canis/ECS/Components/SphereColliderComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include <Canis/ECS/Components/DirectionalLightComponent.hpp>
#include <Canis/ECS/Components/SpotLightComponent.hpp>
#include <Canis/ECS/Components/PointLightComponent.hpp>

class LightingDemoScene : public Canis::Scene
{
    private:
        entt::registry entity_registry;

        entt::entity directionalLight, spotLight, pointLight0, pointLight1;

        Canis::Shader shader;
        Canis::Shader spriteShader;

        Canis::RenderHDRSystem *renderHDRSystem;
        Canis::RenderMeshSystem *renderMeshSystem;
        Canis::RenderSkyboxSystem *renderSkyboxSystem;
        Canis::RenderTextSystem *renderTextSystem;
        Canis::SpriteRenderer2DSystem *spriteRenderer2DSystem;

        bool firstMouseMove = true;
        bool mouseLock = false;

        int cubeModelId = 0;
        int antonioFontId = 0;

        Canis::GLTexture diffuseColorPaletteTexture = {};
        Canis::GLTexture specularColorPaletteTexture = {};
        Canis::GLTexture supperPupStudioLogoTexture = {};

    public:
        LightingDemoScene(std::string _name) : Canis::Scene(_name) {}
        ~LightingDemoScene()
        {
            delete renderHDRSystem;
            delete renderSkyboxSystem;
            delete renderMeshSystem;
            delete renderTextSystem;
            delete spriteRenderer2DSystem;
        }
        
        void PreLoad()
        {
            Canis::Scene::PreLoad();

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
            diffuseColorPaletteTexture = Canis::AssetManager::GetInstance().Get<Canis::TextureAsset>(
                Canis::AssetManager::GetInstance().LoadTexture("assets/textures/palette/diffuse.png")
            )->GetTexture();
            specularColorPaletteTexture = Canis::AssetManager::GetInstance().Get<Canis::TextureAsset>(
                Canis::AssetManager::GetInstance().LoadTexture("assets/textures/palette/specular.png")
            )->GetTexture();

            // load icon
            supperPupStudioLogoTexture = Canis::AssetManager::GetInstance().Get<Canis::TextureAsset>(
                Canis::AssetManager::GetInstance().LoadTexture("assets/textures/SupperPupStudioLogo.png")
            )->GetTexture();

            // load model
            cubeModelId = Canis::AssetManager::GetInstance().LoadModel("assets/models/white_block.obj");

            // load font
            antonioFontId = Canis::AssetManager::GetInstance().LoadText("assets/fonts/Antonio-Bold.ttf", 48);

            renderHDRSystem = new Canis::RenderHDRSystem(window);
            renderSkyboxSystem = new Canis::RenderSkyboxSystem();
            renderMeshSystem = new Canis::RenderMeshSystem();
            renderTextSystem = new Canis::RenderTextSystem();
            spriteRenderer2DSystem = new Canis::SpriteRenderer2DSystem();

            renderHDRSystem->renderMeshSystem = renderMeshSystem;
            renderHDRSystem->renderSkyboxSystem = renderSkyboxSystem;

            renderSkyboxSystem->window = window;
            renderSkyboxSystem->camera = camera;
            renderSkyboxSystem->Init();

            renderTextSystem->camera = camera;
            renderTextSystem->window = window;
            renderTextSystem->Init();

            renderMeshSystem->shader = &shader;
            renderMeshSystem->camera = camera;
            renderMeshSystem->window = window;
            renderMeshSystem->diffuseColorPaletteTexture = &diffuseColorPaletteTexture;
            renderMeshSystem->specularColorPaletteTexture = &specularColorPaletteTexture;

            spriteRenderer2DSystem->window = window;
            spriteRenderer2DSystem->Init(Canis::GlyphSortType::TEXTURE, &spriteShader);

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

            { // direction light
            directionalLight = entity_registry.create();
            entity_registry.emplace<Canis::TransformComponent>(directionalLight,
                false, // active
                glm::vec3(-5.0f, 10.0f, -5.0f), // position
                glm::vec3(-0.2f, -1.0f, -0.3f), // rotation
                glm::vec3(1, 1, 1) // scale
            );
            entity_registry.emplace<Canis::DirectionalLightComponent>(directionalLight,
                glm::vec3(0.05f, 0.05f, 0.05f), // ambient
                glm::vec3(0.8f, 0.8f, 0.8f), // diffuse
                glm::vec3(0.5f, 0.5f, 0.5f) // specular
            );
            }

            { // point light 0
            pointLight0 = entity_registry.create();
            entity_registry.emplace<Canis::TransformComponent>(pointLight0,
                true, // active
                glm::vec3(5.0f, 2.0f, 0.0f), // position
                glm::vec3(0.0f,0.0f,0.0f), // rotation
                glm::vec3(1, 1, 1) // scale
            );
            entity_registry.emplace<Canis::PointLightComponent>(pointLight0,
                1.0f,                           // constant
                0.09f,                          // linear
                0.032f,                         // quadratic
                glm::vec3(0.0f, 0.05f, 0.05f), // ambient
                glm::vec3(0.0f, 0.8f, 0.8f),    // diffuse
                glm::vec3(0.0f, 1.0f, 1.0f)    // specular
            );
            }

            { // point light 1
            pointLight1 = entity_registry.create();
            entity_registry.emplace<Canis::TransformComponent>(pointLight1,
                true, // active
                glm::vec3(-5.0f, 2.0f, 0.0f), // position
                glm::vec3(0.0f,0.0f,0.0f), // rotation
                glm::vec3(1, 1, 1) // scale
            );
            entity_registry.emplace<Canis::PointLightComponent>(pointLight1,
                1.0f,                           // constant
                0.09f,                          // linear
                0.032f,                         // quadratic
                glm::vec3(0.05f, 0.05f, 0.0f), // ambient
                glm::vec3(0.8f, 0.8f, 0.0f),    // diffuse
                glm::vec3(1.0f, 1.0f, 0.0f)    // specular
            );
            }

            { // spot light
            spotLight = entity_registry.create();
            entity_registry.emplace<Canis::TransformComponent>(spotLight,
                true, // active
                camera->Position, // position
                camera->Front, // rotation
                glm::vec3(1, 1, 1) // scale
            );
            float cutOff = glm::cos(glm::radians(12.5f));
            float outerCutOff = glm::cos(glm::radians(15.0f));
            entity_registry.emplace<Canis::SpotLightComponent>(spotLight,
                cutOff,                         // cutOff
                outerCutOff,                    // outerCutOff
                1.0f,                           // constant
                0.09f,                          // linear
                0.032f,                         // quadratic
                glm::vec3(0.05f, 0.05f, 0.0f),  // ambient
                glm::vec3(0.8f, 0.8f, 0.0f),    // diffuse
                glm::vec3(1.0f, 1.0f, 0.0f)     // specular
            );
            }

            { // cube
            entt::entity cube_entity = entity_registry.create();
            entity_registry.emplace<Canis::TransformComponent>(cube_entity,
                true, // active
                glm::vec3(2.0f, 0.0f, 0.0f), // position
                glm::vec3(0.0f, 0.0f, 0.0f), // rotation
                glm::vec3(1, 1, 1) // scale
            );
            entity_registry.emplace<Canis::ColorComponent>(cube_entity,
                glm::vec4(1.0f)
            );
            entity_registry.emplace<Canis::MeshComponent>(cube_entity,
                cubeModelId,
                Canis::AssetManager::GetInstance().Get<Canis::ModelAsset>(cubeModelId)->GetVAO(),
                Canis::AssetManager::GetInstance().Get<Canis::ModelAsset>(cubeModelId)->GetSize(),
                true
            );
            entity_registry.emplace<Canis::SphereColliderComponent>(cube_entity,
                glm::vec3(0.0f),
                1.0f
            );
            }

            { // ground
            entt::entity ground_entity = entity_registry.create();
            entity_registry.emplace<Canis::TransformComponent>(ground_entity,
                true, // active
                glm::vec3(0.0f, -0.5f, 0.0f), // position
                glm::vec3(0.0f, 0.0f, 0.0f), // rotation
                glm::vec3(20.0f, 0.1f, 20.0f) // scale
            );
            entity_registry.emplace<Canis::ColorComponent>(ground_entity,
                glm::vec4(1.0f)
            );
            entity_registry.emplace<Canis::MeshComponent>(ground_entity,
                cubeModelId,
                Canis::AssetManager::GetInstance().Get<Canis::ModelAsset>(cubeModelId)->GetVAO(),
                Canis::AssetManager::GetInstance().Get<Canis::ModelAsset>(cubeModelId)->GetSize(),
                true
            );
            entity_registry.emplace<Canis::SphereColliderComponent>(ground_entity,
                glm::vec3(0.0f),
                1.0f
            );
            }

            { // health text
            entt::entity healthText = entity_registry.create();
            entity_registry.emplace<Canis::RectTransformComponent>(healthText,
                true, // active
                glm::vec2(25.0f, window->GetScreenHeight() - 65.0f), // position
                glm::vec2(0.0f,0.0f), // size
                glm::vec2(0.0f, 0.0f), // rotation
                1.0f, // scale
                0.0f // depth
            );
            entity_registry.emplace<Canis::ColorComponent>(healthText,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) // #26854c
            );
            entity_registry.emplace<Canis::TextComponent>(healthText,
                Canis::AssetManager::GetInstance().LoadText("assets/fonts/Antonio-Bold.ttf", 48),
                new std::string("Asset Manager Demo") // text
            );
            }

            { // sprite test supperPupStudioLogoTexture
            entt::entity spriteEntity = entity_registry.create();
            entity_registry.emplace<Canis::RectTransformComponent>(spriteEntity,
                true, // active
                glm::vec2(0.0f, 0.0f), // position
                glm::vec2(supperPupStudioLogoTexture.width/4,supperPupStudioLogoTexture.height/4), // size
                glm::vec2(0.0f, 0.0f), // rotation
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

            { // sprite test supperPupStudioLogoTexture
            entt::entity spriteEntity = entity_registry.create();
            entity_registry.emplace<Canis::RectTransformComponent>(spriteEntity,
                true, // active
                glm::vec2(100.0f, 400.0f), // position
                glm::vec2(diffuseColorPaletteTexture.width/4,diffuseColorPaletteTexture.height/4), // size
                glm::vec2(0.0f, 0.0f), // rotation
                1.0f, // scale
                0.0f // depth
            );
            entity_registry.emplace<Canis::ColorComponent>(spriteEntity,
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
            );
            entity_registry.emplace<Canis::Sprite2DComponent>(spriteEntity,
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // uv
                diffuseColorPaletteTexture // texture
            );// test
            }
        }

        void UnLoad()
        {
            Canis::Log("Canis Clear");
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
                camera->ProcessKeyboard(Canis::Camera_Movement::FORWARD, deltaTime);
            }

            if (keystate[SDL_SCANCODE_S] && mouseLock)
            {
                camera->ProcessKeyboard(Canis::Camera_Movement::BACKWARD, deltaTime);
            }

            if (keystate[SDL_SCANCODE_A] && mouseLock)
            {
                camera->ProcessKeyboard(Canis::Camera_Movement::LEFT, deltaTime);
            }

            if (keystate[SDL_SCANCODE_D] && mouseLock)
            {
                camera->ProcessKeyboard(Canis::Camera_Movement::RIGHT, deltaTime);
            }

            if (inputManager->justPressedKey(SDLK_ESCAPE))
            {
                mouseLock = !mouseLock;
                //camera->override_camera = !camera->override_camera;

                window->MouseLock(mouseLock);
            }

            if (inputManager->justPressedKey(SDLK_F5))
            {
                Canis::Log("Load Scene");
                ((Canis::SceneManager*)sceneManager)->Load("MainScene");
            }

            auto [transform, spotlight] = entity_registry.get<Canis::TransformComponent, Canis::SpotLightComponent>(spotLight);
            transform.position = camera->Position;
            transform.rotation = camera->Front;
        }

        void Draw()
        {
            glDepthFunc(GL_LESS);
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
            
            // render hdr will call these two
            //renderSkyboxSystem->UpdateComponents(deltaTime, entity_registry);
            //renderMeshSystem->UpdateComponents(deltaTime, entity_registry);
            
            renderHDRSystem->UpdateComponents(deltaTime, entity_registry);
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