#include "A4Engine/SoundSystem.h"
#include <iostream>

SoundSystem::SoundSystem()
{
	if (s_instance != nullptr)
		throw std::runtime_error("only one SoundSystem can be created");

	s_instance = this;

	device = alcOpenDevice(nullptr);
	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);
	alListener3f(AL_POSITION, 640.f / 100.f, 360.f / 100.f, 0.f);
	alListenerf(AL_GAIN, 0.5f);
}

SoundSystem::~SoundSystem()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);

	alcCloseDevice(device);
}


const ALCdevice& SoundSystem::GetDevice()
{
	return *device;
}

const ALCcontext& SoundSystem::GetContext()
{
	return *context;
}

void SoundSystem::SetVolumeEffects(float v)
{
	volumeEffects = v;
	for (auto& s : m_soundsEffects)
		s->SetGain(v);
}

void SoundSystem::SetVolumeAmbiance(float v)
{
	volumeAmbiance = v;
	for (auto& s : m_soundsAmbiance) 
		s->SetGain(v);
}
void SoundSystem::AddSoundEffect(std::shared_ptr<Sound>& sound)
{
	sound->SetGain(volumeEffects);
	m_soundsEffects.emplace_back(sound);
}
void SoundSystem::AddSoundAmbiant(std::shared_ptr<Sound>& sound)
{
	sound->SetGain(volumeAmbiance);
	m_soundsAmbiance.emplace_back(sound);
}
SoundSystem& SoundSystem::Instance()
{
	if (s_instance == nullptr)
		throw std::runtime_error("SoundSystem not instantiated");

	return *s_instance;
}
SoundSystem* SoundSystem::s_instance = nullptr;