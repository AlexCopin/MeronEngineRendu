#include <A4Engine/Spritesheet.hpp>

void Spritesheet::AddAnimation(std::string name, unsigned int frameCount, float frameDuration, Vector2i start, Vector2i size)
{
	m_animationByName.emplace(std::move(name), m_animations.size());

	Animation& animation = m_animations.emplace_back();
	animation.frameCount = frameCount;
	animation.frameDuration = frameDuration;
	animation.size = size;
	animation.start = start;
}

const Spritesheet::Animation& Spritesheet::GetAnimation(std::size_t animIndex) const
{
	return m_animations[animIndex];
}

std::optional<std::size_t> Spritesheet::GetAnimationByName(const std::string& animName) const
{
	auto it = m_animationByName.find(animName);
	if (it == m_animationByName.end())
		return {}; //< on retourne une valeur vide

	return it->second; //< on retourne l'index de l'animation
}

std::size_t Spritesheet::GetAnimationCount() const
{
	return m_animations.size();
}
