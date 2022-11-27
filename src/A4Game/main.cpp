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
#include "A4Engine/Sound.hpp"
#include "A4Engine/SoundSystem.h";

#include "A4Game/PlayerController.hpp"
#include "A4Game/Game.h"

entt::entity CreateBox(entt::registry& registry);
entt::entity CreateCamera(entt::registry& registry);
entt::entity CreateHouse(entt::registry& registry);

void EntityInspector(const char* windowName, entt::registry& registry, entt::entity entity);
void MenuPause(const char* windowName);

int main()
{
	SDLpp sdl;

	SDLppWindow window("Hardy Crow", 1680, 1050);
	SDLppRenderer renderer(window, "", SDL_RENDERER_PRESENTVSYNC);

	ResourceManager resourceManager(renderer);
	InputManager inputManager;

	SDLppImGui imgui(window, renderer);
	SoundSystem soundSystem;

	// Si on initialise ImGui dans une DLL (ce que nous faisons avec la classe SDLppImGui) et l'utilisons dans un autre exécutable (DLL/.exe)
	// la bibliothèque nous demande d'appeler ImGui::SetCurrentContext dans l'exécutable souhaitant utiliser ImGui, avec le contexte précédemment récupéré
	// Ceci est parce qu'ImGui utilise des variables globales en interne qui ne sont pas partagées entre la .dll et l'exécutable (comme indiqué dans sa documentation)
	ImGui::SetCurrentContext(imgui.GetContext());
	entt::registry registry;
	//Create PhysicsSystem
	PhysicsSystem physicsSystem = PhysicsSystem(registry);

	AnimationSystem animSystem(registry);
	RenderSystem renderSystem(renderer, registry);
	VelocitySystem velocitySystem(registry);

	entt::entity cameraEntity = CreateCamera(registry);

	Uint64 lastUpdate = SDL_GetPerformanceCounter();


	InputManager::Instance().BindKeyPressed(SDLK_SPACE, "Jump");

	GameInstance gameInstance(registry);
	//Player
	std::shared_ptr<Spritesheet> spriteSheetPlayer = std::make_shared<Spritesheet>();
	spriteSheetPlayer->AddAnimation("run", 8, 0.1f, Vector2i{ 0, 0 }, Vector2i{ 205, 205 });
	//Preferred to make a class Player to empty the main and centralize data
	Player birdPlayer(registry, spriteSheetPlayer);	

	float physicsTimestep = 1.f / 50.f;
	float physicsAccumulator = 0.f;

	//Function to start game
	GameInstance::Instance().StartGame(registry, physicsSystem);

	std::function<void(bool)> pause = [&](bool isDown) { if(!isDown)GameInstance::Instance().SetPaused(); };
	InputManager::Instance().BindKeyPressed(SDLK_ESCAPE, "Escape");
	InputManager::Instance().OnAction("Escape", pause);

	bool isOpen = true;
	while (isOpen)
	{
		Uint64 now = SDL_GetPerformanceCounter();
		float deltaTime = (float) (now - lastUpdate) / SDL_GetPerformanceFrequency();
		lastUpdate = now;

		//fmt::print("FPS: {}\n", 1.f / deltaTime);

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
		if (!GameInstance::Instance().IsPaused())
		{
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
			//physicsSystem.DebugDraw(renderer, registry.get<Transform>(cameraEntity).GetTransformMatrix().Invert());

			birdPlayer.Update(deltaTime);

			gameInstance.Update(deltaTime);

			/*EntityInspector("Box", registry, box);
			EntityInspector("Camera", registry, cameraEntity);
			EntityInspector("Bird Player", registry, birdPlayer.GetEntity());*/
		}
		else 
		{
			MenuPause("Menu Pause");
		}

		imgui.Render();

		renderer.Present();
	}

	return 0;
}
void MenuPause(const char* windowName)
{
	ImGui::Begin(windowName);

	ImGui::LabelText("Main Menu", "Volume sounds");

	float volume = SoundSystem::Instance().GetVolumeEffects();
	if(ImGui::SliderFloat("Effects", &volume, 0.f, 1.f));
		SoundSystem::Instance().SetVolumeEffects(volume);

	float volumeAmb = SoundSystem::Instance().GetVolumeAmbiance();
	if (ImGui::SliderFloat("Ambiance ", &volumeAmb, 0.f, 1.f));
		SoundSystem::Instance().SetVolumeAmbiance(volumeAmb);

	if (ImGui::Button("Reset"))
	{
		SoundSystem::Instance().SetVolumeEffects(0.5f);
		SoundSystem::Instance().SetVolumeAmbiance(0.5f);
	}

	ImGui::End();
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