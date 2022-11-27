#pragma once

#include <entt/entt.hpp>

class Spritesheet;
class SpritesheetComponent;
class GraphicsComponent;
class Transform;
class InputComponent;
class RigidBodyComponent;
class PlayerControlled;

struct MovementPlayer
{
	const float JUMPFORCE = -8000.f;

	const float JUMPINTERVALVALUE = 0.5f;
	float jumpinterval = 0.f;
};
class Player 
{
public:
	Player(entt::registry& registry, std::shared_ptr<Spritesheet> spritesheet);

	entt::entity& GetEntity();

	entt::entity CreatePlayer(std::shared_ptr<Spritesheet> spritesheet);
	void Update(float deltaTime);
private :
	entt::registry& registry;
	entt::entity player;

	//Components
	RigidBodyComponent* entityPhysics;

	MovementPlayer movementPlayer;

};