#include "App.hpp"

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

RenderMeshSystem renderMeshSystem;
RenderTextSystem renderTextSystem;

App::App()
{
	
}
App::~App()
{
	
}

void App::Run()
{
	if (appState == AppState::ON)
		Canis::FatalError("App already running.");

	Canis::Init();

	unsigned int windowFlags = 0;

	// windowFlags |= Canis::WindowFlags::FULLSCREEN;

	// windowFlags |= Canis::WindowFlags::BORDERLESS;

	window.Create("Canis", 1280, 720, windowFlags);

	time.init(1000);

	Load();

	window.MouseLock(mouseLock);

	appState = AppState::ON;

	Loop();
}
void App::Load()
{
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA);
	// glEnable(GL_CULL_FACE);
	// build and compile our shader program
	shader.Compile("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");
	shader.AddAttribute("aPos");
	shader.AddAttribute("aNormal");
	shader.AddAttribute("aTexcoords");
	shader.Link();

	// Load color palette
	diffuseColorPaletteTexture = Canis::LoadPNGToGLTexture("assets/textures/palette/diffuse.png", GL_RGBA, GL_RGBA);
	specularColorPaletteTexture = Canis::LoadPNGToGLTexture("assets/textures/palette/specular.png", GL_RGBA, GL_RGBA);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<Canis::Vertex> vecVertex;

	bool res = Canis::LoadOBJ("assets/models/white_block.obj", vertices, uvs, normals);

	for(int i = 0; i < vertices.size(); i++)
	{
		Canis::Vertex v = {};
		v.Position = vertices[i];
		v.Normal = normals[i];
		v.TexCoords = uvs[i];
		vecVertex.push_back(v);
	}

	whiteCubeSize = vecVertex.size();
	Canis::Log("s " + std::to_string(vecVertex.size()));

	glGenVertexArrays(1, &whiteCubeVAO);
	glGenBuffers(1, &whiteCubeVBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(whiteCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, whiteCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, vecVertex.size() * sizeof(Canis::Vertex), &vecVertex[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Draw mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Game Systems
	LoadECS();

	// start timer
	previousTime = high_resolution_clock::now();
}
void App::Loop()
{
	while (appState == AppState::ON)
	{
		deltaTime = time.startFrame();

		Update();
		Draw();
		// Get SDL to swap our buffer
		window.SwapBuffer();
		LateUpdate();
		FixedUpdate(deltaTime);
		InputUpdate();

		float fps = time.endFrame();
		
		window.SetWindowName("Canis : Game Name fps : " + std::to_string(fps) + " deltaTime : " + std::to_string(deltaTime) + " Enitity : " + std::to_string(entity_registry.size()));
	}
}
void App::Update()
{
	
}
void App::Draw()
{
	glDepthFunc(GL_LESS); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

	renderMeshSystem.UpdateComponents(deltaTime, entity_registry);
	renderTextSystem.UpdateComponents(deltaTime, entity_registry);
}
void App::LateUpdate() {}
void App::FixedUpdate(float dt)
{
	if (inputManager.isKeyPressed(SDLK_w) && mouseLock)
	{
		camera.ProcessKeyboard(Canis::Camera_Movement::FORWARD, dt);
	}

	if (inputManager.isKeyPressed(SDLK_s) && mouseLock)
	{
		camera.ProcessKeyboard(Canis::Camera_Movement::BACKWARD, dt);
	}

	if (inputManager.isKeyPressed(SDLK_a) && mouseLock)
	{
		camera.ProcessKeyboard(Canis::Camera_Movement::LEFT, dt);
	}

	if (inputManager.isKeyPressed(SDLK_d) && mouseLock)
	{
		camera.ProcessKeyboard(Canis::Camera_Movement::RIGHT, dt);
	}

	if (inputManager.justPressedKey(SDLK_ESCAPE))
    {
		mouseLock = !mouseLock;

        window.MouseLock(mouseLock);
    }
}
void App::InputUpdate()
{
	inputManager.swapMaps();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			appState = AppState::OFF;
			break;
		case SDL_MOUSEMOTION:
			if (mouseLock)
			{
				camera.ProcessMouseMovement(
					event.motion.xrel,
					-event.motion.yrel);
			}
			else
			{
				inputManager.mouse.x = event.motion.x;
				inputManager.mouse.y = event.motion.y;
			}
			break;
		case SDL_KEYUP:
			inputManager.releasedKey(event.key.keysym.sym);
			//Canis::Log("UP" + std::to_string(event.key.keysym.sym));
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(event.key.keysym.sym);
			//Canis::Log("DOWN");
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
				inputManager.leftClick = true;
			if(event.button.button == SDL_BUTTON_RIGHT)
				inputManager.rightClick = true;
			break;
		}
	}
}

void App::LoadECS()
{
	const auto entity = entity_registry.create();
	entity_registry.emplace<TransformComponent>(entity,
		true, // active
		glm::vec3(0.0f, 0.0f, 0.0f), // position
		glm::vec3(0.0f, 0.0f, 0.0f), // rotation
		glm::vec3(1, 1, 1) // scale
	);
	entity_registry.emplace<ColorComponent>(entity,
		glm::vec4(0.15f, 0.52f, 0.30f, 1.0f) // #26854c
	);
	entity_registry.emplace<MeshComponent>(entity,
		whiteCubeVAO,
		whiteCubeSize
	);

	renderTextSystem.Init();
	renderTextSystem.camera = &camera;
	renderTextSystem.window = &window;

	renderMeshSystem.shader = &shader;
	renderMeshSystem.camera = &camera;
	renderMeshSystem.window = &window;
	renderMeshSystem.diffuseColorPaletteTexture = &diffuseColorPaletteTexture;
	renderMeshSystem.specularColorPaletteTexture = &specularColorPaletteTexture;
}