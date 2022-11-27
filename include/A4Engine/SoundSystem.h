#pragma once
#include "AL/al.h"
#include "AL/alc.h"
#include <A4Engine/Export.hpp>
#include <vector>
#include <A4Engine/Sound.hpp>
#include <memory>

class A4ENGINE_API SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	const ALCdevice& GetDevice();
	const ALCcontext& GetContext();

	void SetVolumeEffects(float v);
	void SetVolumeAmbiance(float v);

	float GetVolumeEffects() { return volumeEffects; }
	float GetVolumeAmbiance() { return volumeAmbiance; }

	void AddSoundEffect(std::shared_ptr<Sound>& sound);
	void AddSoundAmbiant(std::shared_ptr<Sound>& sound);

	static SoundSystem& Instance();

private:
	std::vector<std::shared_ptr<Sound>> m_soundsEffects;
	std::vector<std::shared_ptr<Sound>> m_soundsAmbiance;

	float volumeEffects = 0.3f;
	float volumeAmbiance = 0.5f;

	ALCdevice* device;
	ALCcontext* context;

	static SoundSystem* s_instance;
};
