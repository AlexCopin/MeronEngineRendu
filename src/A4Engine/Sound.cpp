#include "A4Engine/Sound.hpp"

Sound::Sound(const char* path)
{
	drwav wav;
	if (!drwav_init_file(&wav, path, nullptr))
	{
		std::cout << "failed to load file" << std::endl;
		invalid = true;
	}else if(drwav_init_file(&wav, path, nullptr))
	{
		m_samples = std::vector<std::int16_t>(wav.totalPCMFrameCount * wav.channels);
		drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, m_samples.data());

		alGenBuffers(1, &m_buffer);
		alBufferData(m_buffer, (wav.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, m_samples.data(), m_samples.size(), 44100);

		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, m_buffer);

		drwav_uninit(&wav);

		std::cout << "Sound " << path << " loaded" << std::endl;
		invalid = false;
	}
}

Sound::Sound(Sound&& sound) noexcept
{
	m_buffer = sound.m_buffer;
	m_source = sound.m_source;
	m_samples = sound.m_samples;

	invalid = sound.invalid;
}
Sound::~Sound()
{
	alDeleteBuffers(1, &m_buffer);
}

Sound Sound::LoadFromFile(const char* soundPath)
{
	return Sound(soundPath);
}

void Sound::Play()
{
	if (IsValid()) {
		alSourcePlay(m_source);
	}
	else {
		std::cout << "Sound invalid" << std::endl;
	}
}

bool Sound::IsValid() const
{
	return !invalid;
}
