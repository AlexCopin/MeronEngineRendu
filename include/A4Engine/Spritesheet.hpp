#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Vector2.hpp>
#include <optional>
#include <unordered_map>
#include <vector>

class A4ENGINE_API Spritesheet
{
	public:
		struct Animation
		{
			Vector2i size;
			Vector2i start;
			unsigned int frameCount;
			float frameDuration;
		};

		void AddAnimation(std::string name, unsigned int frameCount, float frameDuration, Vector2i start, Vector2i size);

		const Animation& GetAnimation(std::size_t animIndex) const;
		std::optional<std::size_t> GetAnimationByName(const std::string& animName) const;
		std::size_t GetAnimationCount() const;

	private:
		std::unordered_map<std::string /*animName*/, std::size_t /*animIndex*/> m_animationByName;
		std::vector<Animation> m_animations;
};
