#include "A4Game/Game.h"
#include <stdexcept>
#include <A4Engine/Sound.hpp>
#include <A4Engine/SoundSystem.h>
#include <A4Engine/ResourceManager.hpp>

GameInstance::GameInstance()
{
	if (s_instance != nullptr)
		throw std::runtime_error("only one InputManager can be created");

	s_instance = this;
	isPaused = false;
}

GameInstance::~GameInstance()
{
}

void GameInstance::StartGame()
{
	auto soundAmbient01 = ResourceManager::Instance().GetSound("assets/sounds/ambientjungle.wav");
	soundAmbient01->SetLooping(true);
	auto mainTheme = ResourceManager::Instance().GetSound("assets/sounds/mainTheme.wav");
	mainTheme->SetLooping(true);
	SoundSystem::Instance().AddSoundAmbiant(soundAmbient01);
	SoundSystem::Instance().AddSoundAmbiant(mainTheme);
	soundAmbient01->Play();
	mainTheme->Play();
}


GameInstance& GameInstance::Instance()
{
	if (s_instance == nullptr)
		throw std::runtime_error("GameInstance hasn't been instantiated");

	return *s_instance;
}

GameInstance* GameInstance::s_instance = nullptr;