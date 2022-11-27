#include "A4Game/PlayerController.hpp"
#include <A4Engine/Sprite.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/InputManager.hpp>
#include <chipmunk/chipmunk.h>
#include <A4Engine/RigidBodyComponent.h>
#include <A4Engine/SpritesheetComponent.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/Sound.hpp>
#include "A4Engine/SoundSystem.h"



//void PlayerInputSystem(entt::registry& registry)
//{
//	auto view = registry.view<PlayerControlled, InputComponent>();
//	for (entt::entity entity : view)
//	{
//		auto& entityInput = view.get<InputComponent>(entity);
//		entityInput.left = InputManager::Instance().IsActive("MoveLeft");
//		entityInput.right = InputManager::Instance().IsActive("MoveRight");
//		entityInput.jump = InputManager::Instance().IsActive("Jump");
//	}
//}



Player::Player(entt::registry& regist, std::shared_ptr<Spritesheet> spritesheet) :
	registry(regist)
{
	player = CreatePlayer(spritesheet);
}

entt::entity& Player::GetEntity()
{
	return player;
}

entt::entity Player::CreatePlayer(std::shared_ptr<Spritesheet> spritesheet)
{
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/sprites/spritesheets/BirdPlayer.png"));
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->Resize(205, 205);
	sprite->SetRect(SDL_Rect{ 0, 0, 205, 205 });

	player = registry.create();
	registry.emplace<SpritesheetComponent>(player, spritesheet, sprite);
	registry.emplace<GraphicsComponent>(player, std::move(sprite));
	registry.emplace<Transform>(player);
	registry.emplace<RigidBodyComponent>(player, 10.f);

	entityPhysics = &registry.get<RigidBodyComponent>(player);

	std::shared_ptr<Sound> jumpSound = ResourceManager::Instance().GetSound("assets/sounds/jump.wav");
	SoundSystem::Instance().AddSoundEffect(jumpSound);

	return player;
}
void Player::Update(float deltaTime)
{
	registry.get<SpritesheetComponent>(player).PlayAnimation(0);
	Vector2f velocity = Vector2f(0.f, 0.f);

	movementPlayer.jumpinterval -= deltaTime;

	if (movementPlayer.jumpinterval <= 0 && InputManager::Instance().IsActive("Jump"))
	{
		ResourceManager::Instance().GetSound("assets/sounds/jump.wav")->Play();
		cpBodyApplyImpulseAtWorldPoint(entityPhysics->GetBody(), cpv(0.f, movementPlayer.JUMPFORCE), cpBodyGetPosition(entityPhysics->GetBody()));
		movementPlayer.jumpinterval = movementPlayer.JUMPINTERVALVALUE;
	}
	float velY = cpBodyGetVelocity(entityPhysics->GetBody()).y;
	cpBodySetVelocity(entityPhysics->GetBody(), cpv(velocity.x, velY));
}
