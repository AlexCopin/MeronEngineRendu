#include "A4Engine/SoundSystem.h"

SoundSystem::SoundSystem()
{
	device = alcOpenDevice(nullptr);
	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);
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
