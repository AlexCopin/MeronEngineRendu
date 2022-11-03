#pragma once

#include <A4Engine/Export.hpp>
#include <entt/fwd.hpp> //< header spécial qui fait des déclarations anticipées des classes de la lib

class A4ENGINE_API VelocitySystem
{
	public:
		VelocitySystem(entt::registry& registry);

		void Update(float deltaTime);

	private:
		entt::registry& m_registry;
};