#include "A4Game/Game.h"
#include <stdexcept>
#include <A4Engine/Sound.hpp>
#include <A4Engine/SoundSystem.h>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/Sprite.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/RigidBodyComponent.h>
#include <A4Engine/VelocityComponent.hpp>
#include <A4Engine/BoxShape.hpp>
#include <A4Engine/PhysicsSystem.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <cstdlib>
#include <memory>



GameInstance::GameInstance(entt::registry& registry) : 
	m_registry(registry)
{
	if (s_instance != nullptr)
		throw std::runtime_error("only one InputManager can be created");

	s_instance = this;
	isPaused = false;
	score = 0;
}

GameInstance::~GameInstance()
{
}

void GameInstance::StartGame(entt::registry& registry, PhysicsSystem& physicSystem)
{
	//SET COLLISION TYPES
	cpCollisionHandler* handler = cpSpaceAddCollisionHandler(physicSystem.GetSpace(), CollisionTypes::PLAYER, CollisionTypes::KILLER);
	handler->beginFunc = GameInstance::RestartGameOnImpact;
	cpCollisionHandler* handler2 = cpSpaceAddCollisionHandler(physicSystem.GetSpace(), CollisionTypes::KILLER, CollisionTypes::HANDRESPAWN);
	handler2->beginFunc = GameInstance::MoveBackHandOnImpact;


	//AMBIENT SOUNDS
	auto soundAmbient01 = ResourceManager::Instance().GetSound("assets/sounds/ambientjungle.wav");
	soundAmbient01->SetLooping(true);
	auto mainTheme = ResourceManager::Instance().GetSound("assets/sounds/mainTheme.wav");
	mainTheme->SetLooping(true);
	SoundSystem::Instance().AddSoundAmbiant(soundAmbient01);
	SoundSystem::Instance().AddSoundAmbiant(mainTheme);
	soundAmbient01->Play();
	mainTheme->Play();


	//LIMITS (not an entity -> Physique pas gérée par PhysicsSystem
	cpBody* floorBody = cpBodyNewStatic();
	cpShape* floorShape = cpSegmentShapeNew(floorBody, cpv(0.f, 1050.f), cpv(10'000.f, 1050.f), 0.f);
	cpBody* roofBody = cpBodyNewStatic();
	cpShape* roofShape = cpSegmentShapeNew(roofBody, cpv(0.f, 1050.f), cpv(10'000.f, 0.f), 0.f);
	cpBody* handEnd = cpBodyNewStatic();
	cpShape* handEndShape = cpSegmentShapeNew(handEnd, cpv(-hands.sizeHands.x, 0), cpv(0, 10000.f), 0.f);
	cpShapeSetCollisionType(roofShape, PLAYER);
	cpShapeSetCollisionType(floorShape, KILLER); 
	cpShapeSetCollisionType(handEndShape, HANDRESPAWN);
	cpSpaceAddShape(physicSystem.GetSpace(), floorShape);
	cpSpaceAddShape(physicSystem.GetSpace(), roofShape);
	cpSpaceAddShape(physicSystem.GetSpace(), handEndShape);

	//CREATE ENEMIES
	CreateHands(registry, physicSystem);

	//CREATE WALLPAPER
	entt::entity wallpaperEnt = registry.create();
	auto wallpaper = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/sprites/wallpaper.jpg"));
	wallpaper->Resize(1680, 1050);
	registry.emplace<GraphicsComponent>(wallpaperEnt, std::move(wallpaper));
	registry.emplace<Transform>(wallpaperEnt);


}

void GameInstance::CreateHands(entt::registry& registry, PhysicsSystem& physicSystem)
{
	auto randomY = std::rand() / ((RAND_MAX + 1u) / hands.SPAWN_RANDOM_Y);

	//INSTANTIATE FIRST HAND
	auto hand = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/sprites/Hand.png"));
	hand->Resize(hands.sizeHands.x, hands.sizeHands.y);
	hand->SetOrigin({ 0.5f, 0.5f });
	entt::entity handEnt = registry.create();
	registry.emplace<Transform>(handEnt);
	registry.emplace<RigidBodyComponent>(handEnt, 1);
	registry.emplace<GraphicsComponent>(handEnt, std::move(hand));

	//CREATE Shape and set Rigidbody
	auto shapeHand = std::make_shared<BoxShape>(hands.sizeHands.x, hands.sizeHands.y);
	auto& handPhysics = registry.get<RigidBodyComponent>(handEnt);
	handPhysics.AddShape(physicSystem.GetSpace(), shapeHand.get());
	handPhysics.SetPosition({ hands.SPAWN_X, randomY + hands.BETWEEN_HANDS });
	handPhysics.SetBodyKinematic();
	cpBodySetVelocity(handPhysics.GetBody(), { -hands.speed, 0 });


	//INSTANTIATE SECOND HAND
	auto hand2 = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/sprites/Hand.png"));
	hand2->Resize(hands.sizeHands.x, hands.sizeHands.y);
	hand2->SetOrigin({ 0.5f, 0.5f });
	entt::entity handEnt2 = registry.create();
	registry.emplace<Transform>(handEnt2);
	registry.emplace<RigidBodyComponent>(handEnt2, 1);
	registry.emplace<GraphicsComponent>(handEnt2, std::move(hand2));

	auto shapeHand2 = std::make_shared<BoxShape>(hands.sizeHands.x, hands.sizeHands.y);
	auto& handPhysics2 = registry.get<RigidBodyComponent>(handEnt2);
	handPhysics2.AddShape(physicSystem.GetSpace(), shapeHand2.get());
	handPhysics2.SetPosition({ hands.SPAWN_X , randomY });
	handPhysics2.SetBodyKinematic();
	handPhysics2.SetAngle(-3.14f);
	cpBodySetVelocity(handPhysics2.GetBody(), { -hands.speed, 0 });

	cpShapeSetCollisionType(shapeHand->GetShape(), CollisionTypes::KILLER);
	cpShapeSetCollisionType(shapeHand2->GetShape(), CollisionTypes::KILLER);

	hands.handDown = handEnt;
	hands.handUp = handEnt2;
}

void GameInstance::RestartGame()
{
	std::cout << "Restart Game" << std::endl;
}

cpBool GameInstance::RestartGameOnImpact(cpArbiter* arb, cpSpace* space, void* data)
{
	Instance().RestartGame();
	return cpTrue;
}

void GameInstance::MoveBackHand()
{
	hands.speed += hands.ACCELERATION;
	double randomY = std::rand() / ((RAND_MAX + 1u) / hands.SPAWN_RANDOM_Y);
	auto& hand01Body = m_registry.get<RigidBodyComponent>(hands.handDown);
	hand01Body.SetPosition({ hands.SPAWN_X, randomY + hands.BETWEEN_HANDS });
	cpBodySetVelocity(hand01Body.GetBody(), { -hands.speed, 0 });
	auto& hand02Body = m_registry.get<RigidBodyComponent>(hands.handUp);
	hand02Body.SetPosition({ hands.SPAWN_X, randomY });
	cpBodySetVelocity(hand02Body.GetBody(), { -hands.speed, 0 });
}

cpBool GameInstance::MoveBackHandOnImpact(cpArbiter* arb, cpSpace* space, void* data)
{
	Instance().MoveBackHand();
	return cpBool();
}

void GameInstance::Update(float deltaTime)
{
	/*ImGui::LabelText("Score", "" + (int)score);
	score += deltaTime;*/
}


GameInstance& GameInstance::Instance()
{
	if (s_instance == nullptr)
		throw std::runtime_error("GameInstance hasn't been instantiated");

	return *s_instance;
}

GameInstance* GameInstance::s_instance = nullptr;