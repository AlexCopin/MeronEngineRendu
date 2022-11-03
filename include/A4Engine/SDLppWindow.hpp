#pragma once

#include <A4Engine/Export.hpp>
#include <SDL.h>
#include <string>

class A4ENGINE_API SDLppWindow
{
	public:
		SDLppWindow(const std::string& title, int width, int height, Uint32 flags = 0);
		SDLppWindow(const std::string& title, int x, int y, int width, int height, Uint32 flags = 0);
		SDLppWindow(const SDLppWindow&) = delete; // constructeur par copie
		SDLppWindow(SDLppWindow&& window) noexcept; // constructeur par mouvement
		~SDLppWindow();

		SDL_Window* GetHandle() const;

		SDLppWindow& operator=(const SDLppWindow&) = delete; // opérateur d'affectation par copie
		SDLppWindow& operator=(SDLppWindow&& window) noexcept; // opérateur d'affectation par mouvement

	private:
		SDL_Window* m_window;
};
