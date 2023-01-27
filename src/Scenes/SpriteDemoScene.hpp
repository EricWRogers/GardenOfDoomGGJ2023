#pragma once

#include <Canis/Yaml.hpp>

#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>
#include <Canis/ECS/Components/Sprite2DComponent.hpp>
#include <Canis/ECS/Components/UIImageComponent.hpp>
#include <Canis/ECS/Components/Camera2DComponent.hpp>

class SpriteDemoScene : public Canis::Scene
{
    private:
        bool firstMouseMove = true;
        bool mouseLock = false;

        int cubeModelId = 0;
        int antonioFontId = 0;

        Canis::GLTexture supperPupStudioLogoTexture = {};

        entt::entity camera2DEntt;

    public:
        SpriteDemoScene(std::string _name) : Canis::Scene(_name) {}
        ~SpriteDemoScene() { }
        
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

            // load icon
            supperPupStudioLogoTexture = assetManager->Get<Canis::TextureAsset>(
                assetManager->LoadTexture("assets/textures/SupperPupStudioLogo.png")
            )->GetTexture();

            // load font
            antonioFontId = assetManager->LoadText("assets/fonts/Antonio-Bold.ttf", 48);
        
            YAML::Node root = YAML::LoadFile("assets/scenes/SpriteDemo.scene");

            std::string n = root["Scene"].as<std::string>();

            Canis::Log("Scene: " + n);

            // serialize assets
            if(YAML::Node renderSystems = root["RenderSystems"]) {
                for(int i = 0;  i < renderSystems.size(); i++) {
                    for(int d = 0;  d < ((Canis::SceneManager *)sceneManager)->decodeRenderSystem.size(); d++) {
                        if (((Canis::SceneManager *)sceneManager)->decodeRenderSystem[d](renderSystems, i, this))
                            continue;
                    }
                }
            }
        }

        void Load()
        {
            Canis::Scene::Load();

            mouseLock = false;
            window->MouseLock(mouseLock);

            YAML::Node root = YAML::LoadFile("assets/scenes/SpriteDemo.scene");

            auto entities = root["Entities"];

            if(entities)
            {
                for(auto e : entities)
                {
                    Canis::Entity entity = CreateEntity();

                    auto camera2dComponent = e["Canis::Camera2DComponent"];
                    if (camera2dComponent)
                    {
                        auto& c2dc = entity.AddComponent<Canis::Camera2DComponent>();
                        c2dc.position = camera2dComponent["position"].as<glm::vec2>();
                        c2dc.scale = camera2dComponent["scale"].as<float>();
                    }

                    auto rectTransform = e["Canis::RectTransformComponent"];
                    if (rectTransform)
                    {
                        auto& rt = entity.AddComponent<Canis::RectTransformComponent>();
                        rt.active = rectTransform["active"].as<bool>();
                        rt.anchor = (Canis::RectAnchor)rectTransform["anchor"].as<int>();
                        rt.position = rectTransform["position"].as<glm::vec2>();
                        rt.size = rectTransform["size"].as<glm::vec2>();
                        rt.originOffset = rectTransform["originOffset"].as<glm::vec2>();
                        rt.rotation = rectTransform["rotation"].as<float>();
                        rt.scale = rectTransform["scale"].as<float>();
                        rt.depth = rectTransform["depth"].as<float>();
                    }

                    auto colorComponent = e["Canis::ColorComponent"];
                    if (colorComponent)
                    {
                        auto& cc = entity.AddComponent<Canis::ColorComponent>();
                        cc.color = colorComponent["color"].as<glm::vec4>();
                    }

                    auto textComponent = e["Canis::TextComponent"];
                    if (textComponent)
                    {
                        auto& tc = entity.AddComponent<Canis::TextComponent>();
                        auto asset = textComponent["assetId"];
                        if (asset)
                        {
                            tc.assetId = assetManager->LoadText(
                                asset["path"].as<std::string>(),
                                asset["size"].as<unsigned int>()
                            );
                        }
                        tc.text = new std::string;
                        (*tc.text) = textComponent["text"].as<std::string>();
                        tc.align = textComponent["align"].as<unsigned int>();
                    }

                    auto sprite2DComponent = e["Canis::Sprite2DComponent"];
                    if (sprite2DComponent)
                    {
                        auto& s2dc = entity.AddComponent<Canis::Sprite2DComponent>();
                        s2dc.uv = sprite2DComponent["uv"].as<glm::vec4>();
                        s2dc.texture = assetManager->Get<Canis::TextureAsset>(
                            assetManager->LoadTexture(
                                sprite2DComponent["texture"].as<std::string>()
                            )
                        )->GetTexture();
                    }
                }
            }
        }

        void UnLoad()
        {
            Canis::Scene::UnLoad();
        }

        void Update()
        {
            Canis::Scene::Update();
        }

        void LateUpdate()
        {
            Canis::Scene::LateUpdate();

            /*const float CAMERA_SPEED = 20.0f;
            const float SCALE_SPEED = 0.1f;

            const Uint8 *keystate = SDL_GetKeyboardState(NULL);

            auto cam = entityRegistry.view<Canis::Camera2DComponent>();
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
            }*/

            

            if (inputManager->justPressedKey(SDLK_ESCAPE))
            {
                mouseLock = !mouseLock;

                window->MouseLock(mouseLock);
            }

            if (inputManager->justPressedKey(SDLK_F5))
            {
                Canis::Log("Load Scene");
                ((Canis::SceneManager*)sceneManager)->Load("SpriteDemoScene");
            }
        }

        void Draw()
        {
            Canis::Scene::Draw();

            window->SetWindowName("Canis : Template | fps : " + std::to_string(int(window->fps))
            + " deltaTime : " + std::to_string(deltaTime));
        }

        void InputUpdate()
        {
            Canis::Scene::InputUpdate();
        }
};