#include <iostream>
#include <SDL.h>
#include <A4Engine/AnimationSystem.hpp>
#include <A4Engine/CameraComponent.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/InputManager.hpp>
#include <A4Engine/Model.hpp>
#include <A4Engine/RenderSystem.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/SDLpp.hpp>
#include <A4Engine/SDLppImGui.hpp>
#include <A4Engine/SDLppRenderer.hpp>
#include <A4Engine/SDLppTexture.hpp>
#include <A4Engine/SDLppWindow.hpp>
#include <A4Engine/Sprite.hpp>
#include <A4Engine/SpritesheetComponent.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/VelocityComponent.hpp>
#include <A4Engine/VelocitySystem.hpp>
#include <chipmunk/chipmunk.h>
#include <A4Engine/PhysicsSystem.h>
#include <A4Engine/RigidBodyComponent.h>
#include <A4Engine/BoxShape.hpp>
#include <A4Engine/Shape.h>
#include <entt/entt.hpp>
#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include "A4Engine/Matrix3.h"

entt::entity CreateBox(entt::registry& registry);
entt::entity CreateCamera(entt::registry& registry);
entt::entity CreateHouse(entt::registry& registry);
entt::entity CreateRunner(entt::registry& registry, std::shared_ptr<Spritesheet> spritesheet);

void EntityInspector(const char* windowName, entt::registry& registry, entt::entity entity);

void HandleCameraMovement(entt::registry& registry, entt::entity camera, float deltaTime);
void HandleRunnerMovement(entt::registry& registry, entt::entity runner, float deltaTime);

struct InputComponent
{
	bool left = false;
	bool right = false;
	bool jump = false;
};

struct PlayerControlled {};

void PlayerControllerSystem(entt::registry& registry)
{
	auto view = registry.view<RigidBodyComponent, InputComponent>();
	for (entt::entity entity : view)
	{
		auto& entityInput = view.get<InputComponent>(entity);
		auto& entityPhysics = view.get<RigidBodyComponent>(entity);

		Vector2f velocity = Vector2f(0.f, 0.f);
		if (entityInput.left)
			velocity.x -= 500.f;

		if (entityInput.right)
			velocity.x += 500.f;

		if (entityInput.jump)
			cpBodyApplyImpulseAtWorldPoint(entityPhysics.GetBody(), cpv(0.f, -1000.f), cpBodyGetPosition(entityPhysics.GetBody()));

		float velY = cpBodyGetVelocity(entityPhysics.GetBody()).y;
		cpBodySetVelocity(entityPhysics.GetBody(), cpv(velocity.x, velY));
	}
}

void PlayerInputSystem(entt::registry& registry)
{
	auto view = registry.view<PlayerControlled, InputComponent>();
	for (entt::entity entity : view)
	{
		auto& entityInput = view.get<InputComponent>(entity);
		entityInput.left = InputManager::Instance().IsActive("MoveLeft");
		entityInput.right = InputManager::Instance().IsActive("MoveRight");
		entityInput.jump = InputManager::Instance().IsActive("Jump");
	}
}

int main()
{
	SDLpp sdl;

	SDLppWindow window("MeronEngineRendu", 1280, 720);
	SDLppRenderer renderer(window, "", SDL_RENDERER_PRESENTVSYNC);

	ResourceManager resourceManager(renderer);
	InputManager inputManager;

	SDLppImGui imgui(window, renderer);

	// Si on initialise ImGui dans une DLL (ce que nous faisons avec la classe SDLppImGui) et l'utilisons dans un autre exécutable (DLL/.exe)
	// la bibliothèque nous demande d'appeler ImGui::SetCurrentContext dans l'exécutable souhaitant utiliser ImGui, avec le contexte précédemment récupéré
	// Ceci est parce qu'ImGui utilise des variables globales en interne qui ne sont pas partagées entre la .dll et l'exécutable (comme indiqué dans sa documentation)
	ImGui::SetCurrentContext(imgui.GetContext());

	// ZQSD
	InputManager::Instance().BindKeyPressed(SDLK_q, "MoveLeft");
	InputManager::Instance().BindKeyPressed(SDLK_d, "MoveRight");
	InputManager::Instance().BindKeyPressed(SDLK_z, "MoveUp");
	InputManager::Instance().BindKeyPressed(SDLK_s, "MoveDown");

	// Touches directionnelles (caméra)
	InputManager::Instance().BindKeyPressed(SDLK_LEFT, "CameraMoveLeft");
	InputManager::Instance().BindKeyPressed(SDLK_RIGHT, "CameraMoveRight");
	InputManager::Instance().BindKeyPressed(SDLK_UP, "CameraMoveUp");
	InputManager::Instance().BindKeyPressed(SDLK_DOWN, "CameraMoveDown");

	std::shared_ptr<Spritesheet> spriteSheet = std::make_shared<Spritesheet>();
	spriteSheet->AddAnimation("idle", 5, 0.1f, Vector2i{ 0, 0 },  Vector2i{ 32, 32 });
	spriteSheet->AddAnimation("run",  8, 0.1f, Vector2i{ 0, 32 }, Vector2i{ 32, 32 });
	spriteSheet->AddAnimation("jump", 4, 0.1f, Vector2i{ 0, 64 }, Vector2i{ 32, 32 });

	entt::registry registry;

	AnimationSystem animSystem(registry);
	RenderSystem renderSystem(renderer, registry);
	VelocitySystem velocitySystem(registry);

	entt::entity cameraEntity = CreateCamera(registry);

	entt::entity house = CreateHouse(registry);
	registry.get<Transform>(house).SetPosition({ 750.f, 275.f });
	registry.get<Transform>(house).SetScale({ 2.f, 2.f });

	entt::entity runner = CreateRunner(registry, spriteSheet);
	registry.get<Transform>(runner).SetPosition({ 300.f, 250.f });


	Uint64 lastUpdate = SDL_GetPerformanceCounter();

	InputManager::Instance().BindKeyPressed(SDL_KeyCode::SDLK_r, "PlayRun");

	InputManager::Instance().OnAction("PlayRun", [&](bool pressed)
	{
		if (pressed)
			registry.get<SpritesheetComponent>(runner).PlayAnimation("run");
		else
			registry.get<SpritesheetComponent>(runner).PlayAnimation("idle");
	});

	//Create PhysicsSystem
	PhysicsSystem physicsSystem = PhysicsSystem(registry);

	//Create Shape for runner
	std::shared_ptr<Shape> shapeRunner = std::make_shared<BoxShape>(128.f, 256.f);
	registry.get<RigidBodyComponent>(runner).AddShape(physicsSystem.GetSpace(), shapeRunner.get());
	registry.get<RigidBodyComponent>(runner).SetPosition({ 100,100 });

	//Create shape for box
	entt::entity box = CreateBox(registry);
	std::shared_ptr<Shape> shapeBox = std::make_shared<BoxShape>(256.f, 256.f);
	registry.get<RigidBodyComponent>(box).AddShape(physicsSystem.GetSpace(), shapeBox.get());
	registry.get<RigidBodyComponent>(box).SetPosition({ 400,400 });
	
	//Floor (not an entity -> Physique pas gérée par PhysicsSystem
	cpBody* floorBody = cpBodyNewStatic();

	cpShape* floorShape = cpSegmentShapeNew(floorBody, cpv(0.f, 720.f), cpv(10'000.f, 720.f), 0.f);
	cpSpaceAddShape(physicsSystem.GetSpace(), floorShape);


	InputManager::Instance().BindKeyPressed(SDLK_SPACE, "Jump");

	float physicsTimestep = 1.f / 50.f;
	float physicsAccumulator = 0.f;

	bool isOpen = true;
	while (isOpen)
	{
		Uint64 now = SDL_GetPerformanceCounter();
		float deltaTime = (float) (now - lastUpdate) / SDL_GetPerformanceFrequency();
		lastUpdate = now;

		fmt::print("FPS: {}\n", 1.f / deltaTime);

		SDL_Event event;
		while (SDLpp::PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				isOpen = false;

			imgui.ProcessEvent(event);

			InputManager::Instance().HandleEvent(event);
		}

		imgui.NewFrame();

		renderer.SetDrawColor(127, 0, 127, 255);
		renderer.Clear();

		HandleCameraMovement(registry, cameraEntity, deltaTime);

		physicsAccumulator += deltaTime;
		while (physicsAccumulator >= physicsTimestep)
		{
			cpSpaceStep(physicsSystem.GetSpace(), physicsTimestep);
			physicsAccumulator -= physicsTimestep;
		}

		animSystem.Update(deltaTime);
		velocitySystem.Update(deltaTime);
		renderSystem.Update(deltaTime);
		physicsSystem.FixedUpdate(deltaTime);
		PlayerInputSystem(registry);
		PlayerControllerSystem(registry);

		EntityInspector("Box", registry, box);
		EntityInspector("Camera", registry, cameraEntity);
		EntityInspector("Runner", registry, runner);

		imgui.Render();

		renderer.Present();
	}

	return 0;
}

void EntityInspector(const char* windowName, entt::registry& registry, entt::entity entity)
{
	Transform& transform = registry.get<Transform>(entity);

	float rotation = transform.GetRotation();
	Vector2f pos = transform.GetPosition();
	Vector2f scale = transform.GetScale();

	ImGui::Begin(windowName);

	ImGui::LabelText("Position", "X: %f\nY: %f", pos.x, pos.y);

	float positionVal[2] = { pos.x, pos.y };
	
	if (ImGui::SliderFloat2("Position", positionVal, -5000, 5000))
		transform.SetPosition({ positionVal[0], positionVal[1] });

	if (ImGui::InputFloat("Rotation", &rotation, 1))
		transform.SetRotation(rotation);

	float scaleVal[2] = { scale.x, scale.y };
	if (ImGui::InputFloat2("Scale", scaleVal))
		transform.SetScale({ scaleVal[0], scaleVal[1] });

	if (ImGui::Button("Reset"))
	{
		transform.SetScale({ 1.f, 1.f });
		transform.SetRotation(0.f);
		transform.SetPosition(Vector2f(0.f, 0.f));
	}

	ImGui::End();
}

entt::entity CreateBox(entt::registry& registry)
{
	std::shared_ptr<Sprite> box = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/box.png"));
	box->SetOrigin({ 0.5f, 0.5f });

	entt::entity entity = registry.create();
	registry.emplace<GraphicsComponent>(entity, std::move(box));
	registry.emplace<Transform>(entity);
	registry.emplace<RigidBodyComponent>(entity, 300.f);

	return entity;
}

entt::entity CreateCamera(entt::registry& registry)
{
	entt::entity entity = registry.create();
	registry.emplace<CameraComponent>(entity);
	registry.emplace<Transform>(entity);

	return entity;
}

entt::entity CreateHouse(entt::registry& registry)
{
	std::shared_ptr<Model> house = ResourceManager::Instance().GetModel("assets/house.model");

	entt::entity entity = registry.create();
	registry.emplace<GraphicsComponent>(entity, std::move(house));
	registry.emplace<Transform>(entity);

	return entity;
}

entt::entity CreateRunner(entt::registry& registry, std::shared_ptr<Spritesheet> spritesheet)
{
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/runner.png"));
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->Resize(256, 256);
	sprite->SetRect(SDL_Rect{ 0, 0, 32, 32 });

	entt::entity entity = registry.create();
	registry.emplace<SpritesheetComponent>(entity, spritesheet, sprite);
	registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	registry.emplace<Transform>(entity);
	registry.emplace<InputComponent>(entity);
	registry.emplace<RigidBodyComponent>(entity, 80.f);
	registry.emplace<PlayerControlled>(entity);

	return entity;
}

void HandleCameraMovement(entt::registry& registry, entt::entity camera, float deltaTime)
{
	Transform& cameraTransform = registry.get<Transform>(camera);

	if (InputManager::Instance().IsActive("CameraMoveDown"))
		cameraTransform.Translate(Vector2f(0.f, 500.f * deltaTime));

	if (InputManager::Instance().IsActive("CameraMoveLeft"))
		cameraTransform.Translate(Vector2f(-500.f * deltaTime, 0.f));

	if (InputManager::Instance().IsActive("CameraMoveRight"))
		cameraTransform.Translate(Vector2f(500.f * deltaTime, 0.f));

	if (InputManager::Instance().IsActive("CameraMoveUp"))
		cameraTransform.Translate(Vector2f(0.f, -500.f * deltaTime));
}

void HandleRunnerMovement(entt::registry& registry, entt::entity runner, float deltaTime)
{
	Transform& transform = registry.get<Transform>(runner);

	if (InputManager::Instance().IsActive("MoveDown"))
		transform.Translate(Vector2f(0.f, 500.f * deltaTime));

	if (InputManager::Instance().IsActive("MoveLeft"))
		transform.Translate(Vector2f(-500.f * deltaTime, 0.f));

	if (InputManager::Instance().IsActive("MoveRight"))
		transform.Translate(Vector2f(500.f * deltaTime, 0.f));

	if (InputManager::Instance().IsActive("MoveUp"))
		transform.Translate(Vector2f(0.f, -500.f * deltaTime));
}