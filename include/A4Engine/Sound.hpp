#pragma once
#include <A4Engine/Export.hpp>
#include <AL/al.h>
#include "AL/alc.h"
#include "dr_wav.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>

class A4ENGINE_API Sound
{
public:
	Sound() = delete;
	Sound(const Sound&) = default;
	Sound(Sound&& sound) noexcept;
	~Sound();
	//Only .wav files
	static Sound LoadFromFile(const char* soundPath);

	void Play();
	/*
	ALuint GetBuffer();
	drwav* GetWav();
	std::vector<std::int16_t>& GetSamples();
	*/

	Sound& operator=(const Sound&) = delete;
	Sound& operator=(Sound&&) = default;

	bool IsValid() const;
private:
	//Only .wav files
	Sound(const char* path);

	bool invalid;

	drwav m_wav;
	ALuint m_buffer;
	ALuint m_source;
	std::vector<std::int16_t> m_samples;
};