//#include <AL/al.h>
//#include "AL/alc.h"
//#include "dr_wav.h"
#include <A4Engine/SDLpp.hpp>
#include <A4Engine/SDLppWindow.hpp>
#include <A4Engine/SDLppRenderer.hpp>
#include "A4Engine/Sound.hpp"
#include <iostream>
#include <A4Engine/ResourceManager.hpp>
#include "A4Engine/SoundSystem.h"
#include <vector>

int main()
{
	/*const char* deviceList = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

	std::cout << deviceList << std::endl;

	std::vector<std::string> devices;
	while (true)
	{
		std::size_t length = std::strlen(deviceList);
		if (length == 0)
			break;
		devices.emplace_back(deviceList, length);
		deviceList += length + 1;
	}*/


	SDLpp sdl;

	SDLppWindow window("MeronEngineRendu", 1280, 720);
	SDLppRenderer renderer(window, "", SDL_RENDERER_PRESENTVSYNC);

	ResourceManager resourceManager(renderer);
	SoundSystem soundSystem;

	std::shared_ptr<Sound> soundTest = ResourceManager::Instance().GetSound("assets/Tristram.wav");
	soundTest->Play();
	//std::shared_ptr<Sound> soundError = ResourceManager::Instance().GetSound("assets/Error.wav");
	//soundError->Play();



	std::getchar();


	return 0;
}
