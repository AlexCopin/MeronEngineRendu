#pragma once

#include <A4Engine/Export.hpp>
#include <entt/fwd.hpp> //< header spécial qui fait des déclarations anticipées des classes de la lib

class SDLppRenderer;

class A4ENGINE_API RenderSystem
{
	public:
		RenderSystem(SDLppRenderer& renderer, entt::registry& registry);

		void Update(float deltaTime);

	private:
		SDLppRenderer& m_renderer;
		entt::registry& m_registry;
};