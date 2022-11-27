#pragma once

#include <A4Engine/Export.hpp>
#include <entt/fwd.hpp>
#include <chipmunk/chipmunk.h>
#include <A4Engine/RigidBodyComponent.h>
#include <A4Engine/SDLppRenderer.hpp>
#include <A4Engine/Matrix3.h>
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

	void DebugDraw(SDLppRenderer& renderer, const Matrix3& cameraInverseTransform);

	float GetGravity();
	float GetDamping();
	void SetGravity(float value);
	void SetDamping(float value);

	void AddHandler(cpCollisionHandler handler);


	void Step(float deltaTime);

	void FixedUpdate(float deltaTime);

	static PhysicsSystem& Instance();


private:
	cpSpace* m_space;
	entt::registry& m_registry;

	std::vector<cpCollisionHandler> m_handlers;

	float m_timeStep;
	float m_timeAccumulator;

	static PhysicsSystem* s_instance;

};