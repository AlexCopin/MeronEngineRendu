#include <A4Engine/AnimationSystem.hpp>
#include <A4Engine/SpritesheetComponent.hpp>
#include <entt/entt.hpp>

AnimationSystem::AnimationSystem(entt::registry& registry) :
m_registry(registry)
{
}

void AnimationSystem::Update(float deltaTime)
{
	auto view = m_registry.view<SpritesheetComponent>();
	for (entt::entity entity : view)
	{
		SpritesheetComponent& entitySpritesheet = view.get<SpritesheetComponent>(entity);
		entitySpritesheet.Update(deltaTime);
	}
}
