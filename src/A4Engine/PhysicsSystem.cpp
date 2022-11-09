#include <A4Engine/PhysicsSystem.h>
#include <entt/entt.hpp>
#include <A4Engine/Math.hpp>
#include <A4Engine/Transform.hpp>


PhysicsSystem::PhysicsSystem(entt::registry& registry) :
	m_registry(registry)
{
	m_space = cpSpaceNew();
	SetGravity(981.f);
	SetDamping(0.5f);
}

PhysicsSystem::~PhysicsSystem()
{
	auto RigidBodyView = m_registry.view<RigidBodyComponent>();
	for (entt::entity entity : RigidBodyView)
	{
		RigidBodyComponent& entityRigidBody = RigidBodyView.get<RigidBodyComponent>(entity);
		entityRigidBody.CleanShapeBank(m_space);
	}

	cpSpaceFree(m_space);
}

cpSpace* PhysicsSystem::GetSpace()
{
	return m_space;
}

float PhysicsSystem::GetGravity()
{
	return cpSpaceGetGravity(m_space).y;
}

float PhysicsSystem::GetDamping()
{
	return cpSpaceGetDamping(m_space);
}

void PhysicsSystem::SetGravity(float value)
{
	cpSpaceSetGravity(m_space, cpv(0, value));
}

void PhysicsSystem::SetDamping(float value)
{
	cpSpaceSetDamping(m_space, value);
}

void PhysicsSystem::Step(float deltaTime)
{
	m_timeAccumulator += deltaTime;
	while (m_timeAccumulator >= m_timeStep) {
		cpSpaceStep(m_space, m_timeStep);
		m_timeAccumulator -= m_timeStep;
	}
}


//Tentative ratée de FixedUpdate (modifications enlevées)
void PhysicsSystem::FixedUpdate(float deltaTime)
{
	auto RigidBodyView = m_registry.view<RigidBodyComponent, Transform>();
	for (entt::entity entity : RigidBodyView)
	{
		RigidBodyComponent& entityRigidBody = RigidBodyView.get<RigidBodyComponent>(entity);
		Transform& entityTransform = RigidBodyView.get<Transform>(entity);

		cpVect pos = entityRigidBody.GetPosition();
		float rot = entityRigidBody.GetAngle() * Rad2Deg;

		entityTransform.SetPosition(Vector2f(pos.x, pos.y));
		entityTransform.SetRotation(rot);
	}
}
