#pragma once
#include "AL/al.h"
#include "AL/alc.h"
#include <A4Engine/Export.hpp>
#include <vector>

class A4ENGINE_API SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	const ALCdevice& GetDevice();
	const ALCcontext& GetContext();

private:
	ALCdevice* device;
	ALCcontext* context;
};
