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

#include <Canis/ECS/Components/TransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>
#include <Canis/ECS/Components/MeshComponent.hpp>
#include <Canis/ECS/Components/SphereColliderComponent.hpp>

class MainScene : public Canis::Scene
{
    private:
        entt::registry entity_registry;

        Canis::Shader shader;

        Canis::RenderMeshSystem *renderMeshSystem;
        Canis::RenderSkyboxSystem *renderSkyboxSystem;
        Canis::RenderTextSystem *renderTextSystem;

        bool firstMouseMove = true;
        bool mouseLock = false;

        int cubeModelId = 0;

        Canis::GLTexture diffuseColorPaletteTexture = {};
        Canis::GLTexture specularColorPaletteTexture = {};

    public:
        MainScene(std::string _name) : Canis::Scene(_name) {}
        ~MainScene()
        {
            delete renderSkyboxSystem;
            delete renderMeshSystem;
            delete renderTextSystem;
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

            // Load color palette
            diffuseColorPaletteTexture = Canis::AssetManager::GetInstance().Get<Canis::Texture>(
                Canis::AssetManager::GetInstance().LoadTexture("assets/textures/palette/diffuse.png")
            )->GetTexture();
            specularColorPaletteTexture = Canis::AssetManager::GetInstance().Get<Canis::Texture>(
                Canis::AssetManager::GetInstance().LoadTexture("assets/textures/palette/specular.png")
            )->GetTexture();

            // load model
            cubeModelId = Canis::AssetManager::GetInstance().LoadModel("assets/models/white_block.obj");


            renderSkyboxSystem = new Canis::RenderSkyboxSystem();
            renderMeshSystem = new Canis::RenderMeshSystem();
            renderTextSystem = new Canis::RenderTextSystem();

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

            // Draw mode
            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        void Load()
        {            
            camera->Position = glm::vec3(0.0f,0.15f,-0.3f);
            camera->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            camera->Pitch = Canis::PITCH+0.0f;
            camera->Yaw = Canis::YAW+90.0f;
            camera->override_camera = false;
            camera->UpdateCameraVectors();

            window->MouseLock(mouseLock);

            // this is a hack need to sort object by distance to camera from render
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
                Canis::AssetManager::GetInstance().Get<Canis::Model>(cubeModelId)->GetVAO(),
                Canis::AssetManager::GetInstance().Get<Canis::Model>(cubeModelId)->GetSize()
            );
            entity_registry.emplace<Canis::SphereColliderComponent>(cube_entity,
                glm::vec3(0.0f),
                1.0f
            );
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
            if (inputManager->isKeyPressed(SDLK_w) && mouseLock)
            {
                camera->ProcessKeyboard(Canis::Camera_Movement::FORWARD, deltaTime);
            }

            if (inputManager->isKeyPressed(SDLK_s) && mouseLock)
            {
                camera->ProcessKeyboard(Canis::Camera_Movement::BACKWARD, deltaTime);
            }

            if (inputManager->isKeyPressed(SDLK_a) && mouseLock)
            {
                camera->ProcessKeyboard(Canis::Camera_Movement::LEFT, deltaTime);
            }

            if (inputManager->isKeyPressed(SDLK_d) && mouseLock)
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
                ((Canis::SceneManager*)sceneManager)->Load("GameLoop");
            }
        }

        void Draw()
        {
            glDepthFunc(GL_LESS);
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

            renderSkyboxSystem->UpdateComponents(deltaTime, entity_registry);
            renderMeshSystem->UpdateComponents(deltaTime, entity_registry);
            renderTextSystem->UpdateComponents(deltaTime, entity_registry);

            window->SetWindowName("Canis : Template | fps : " + std::to_string(int(window->fps))
            + " deltaTime : " + std::to_string(deltaTime)
            + " Enitity : " + std::to_string(entity_registry.size())
            + " Rendered : " + std::to_string(renderMeshSystem->entities_rendered));
        }

        void InputUpdate()
        {

        }
};