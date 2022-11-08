#include <AL/al.h>
#include "AL/alc.h"
#define DR_WAV_IMPLEMENTATION
#include <iostream>
#include <vector>
#include "dr_wav.h"

int main()
{
	const char* deviceList = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

	std::cout << deviceList << std::endl;

	std::vector<std::string> devices;
	while (true)
	{
		std::size_t length = std::strlen(deviceList);
		if (length == 0)
			break;
		devices.emplace_back(deviceList, length);
		deviceList += length + 1;
	}

	ALCdevice* device = alcOpenDevice(nullptr);
	
	ALCcontext* context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	ALuint buffer;
	alGenBuffers(1, &buffer);

	//TODO*

	drwav wav;
	if(!drwav_init_file(&wav, "assets/Tristram.wav", nullptr))
	{
		std::cout << "failed to load file" << std::endl;
		return 0;
	}

	std::vector<std::int16_t> samples(wav.totalPCMFrameCount * wav.channels);
	drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, samples.data());
	/*for (std::size_t i = 0; i < 44100; ++i)
	{
		samples.push_back(std::sin(float(2 * 3.14f * i / 44100 * (400 + i / 2))) * 32767);
		samples.push_back(std::sin(float(2 * 3.14f * i / 44100 * (1200 + i / 2))) * -1 * 32767);
	}*/


	alBufferData(buffer, (wav.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, samples.data(), samples.size(), 44100);

	drwav_uninit(&wav);

	ALuint source;
	alGenSources(1, &source);

	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);

	std::getchar();

	//Libération
	alDeleteBuffers(1, &buffer);

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);

	alcCloseDevice(device);


	return 0;
}