#pragma once
#include <SDL.h>
#include <iostream>
#include <entt/entt.hpp>
#include <A4Game/PlayerController.hpp>
#include <chipmunk/chipmunk.h>
#include <A4Engine/Vector2.hpp>

class PhysicsSystem;

struct HandsDuo
{
	float speed = 250.f;
	const float ACCELERATION = 100.f;
	const float SPAWN_X = 2000;
	const int BETWEEN_HANDS = 1000;
	const float SPAWN_RANDOM_Y = 300;
	const Vector2i sizeHands = Vector2i(400, 600);

	HandsDuo() = default;
	HandsDuo(entt::entity handDwn, entt::entity handp) :
		handDown(handDwn),
		handUp(handp)
	{}

	entt::entity handDown;
	entt::entity handUp;
};

class GameInstance
{
public:
	GameInstance(entt::registry& registry);
	GameInstance(const GameInstance&) = delete;
	GameInstance(GameInstance&&) = delete;
	~GameInstance();
	GameInstance& operator=(const GameInstance&) = delete;
	GameInstance& operator=(GameInstance&&) = delete;

	void SetPaused() { isPaused = isPaused ? false : true; }
	bool IsPaused() { return isPaused; }

	static GameInstance& Instance(); 


	void StartGame(entt::registry& registry, PhysicsSystem& physicSystem);

	void RestartGame();
	static cpBool RestartGameOnImpact(cpArbiter* arb, cpSpace* space, void* data);


	void MoveBackHand();
	static cpBool MoveBackHandOnImpact(cpArbiter* arb, cpSpace* space, void* data);

	void Update(float deltaTime);
private:
	std::string m_gameName;
	static GameInstance* s_instance;
	entt::registry& m_registry;
	bool isPaused;

	//Game variables
	HandsDuo hands;
	void CreateHands(entt::registry& registry, PhysicsSystem& physicSystem);
	float score;
};