#pragma once

#include <A4Engine/Export.hpp>
#include <entt/fwd.hpp>
#include <chipmunk/chipmunk.h>
#include <A4Engine/RigidBodyComponent.h>
#include <vector>

class A4ENGINE_API PhysicsSystem {
public:

	//deltaTimeStep is the step between each update
	PhysicsSystem(entt::registry& registry);
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&&) = delete;
	~PhysicsSystem();

	PhysicsSystem& operator=(const PhysicsSystem&) = delete;
	PhysicsSystem& operator=(PhysicsSystem&&) = delete;

	cpSpace* GetSpace();

	float GetGravity();
	float GetDamping();
	void SetGravity(float value);
	void SetDamping(float value);

	void Step(float deltaTime);

	void FixedUpdate(float deltaTime);

private:
	cpSpace* m_space;
	entt::registry& m_registry;

	float m_timeStep;
	float m_timeAccumulator;

};