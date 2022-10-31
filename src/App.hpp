#include <iostream>
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

#ifdef __ANDROID__
    #include <android/log.h>
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN,__VA_ARGS__)
#endif

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
#include <Canis/Data/GLTexture.hpp>
#include <Canis/Data/Vertex.hpp>
#include <Canis/External/entt.hpp>

#include "Scenes/MainScene.hpp"
#include "Scenes/ShadowDemoScene.hpp"
#include "Scenes/LightingDemoScene.hpp"


enum AppState
{
    ON,
    OFF
};

class App
{
public:
    App();
    ~App();

    void Run();

private:
    void Load();
    void Loop();
    void Update();
    void Draw();
    void LateUpdate();
    void FixedUpdate(float dt);
    void InputUpdate();

    void LoadECS();

    Canis::SceneManager sceneManager;

    AppState appState = AppState::OFF;

    

    Canis::Window window;

    Canis::Time time;

    Canis::InputManager inputManager;

    Canis::Camera camera = Canis::Camera(glm::vec3(0.0f, 0.15f, -0.3f),glm::vec3(0.0f, 1.0f, 0.0f),Canis::YAW+90.0f,Canis::PITCH+0.0f);

    float lastXMousePos;
    float lastYMousePos;
    
   

    high_resolution_clock::time_point currentTime;
    high_resolution_clock::time_point previousTime;
    double deltaTime;

    Canis::AStar aStar;

    unsigned int seed;
};