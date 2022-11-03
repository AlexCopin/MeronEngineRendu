#include <A4Engine/SDLppWindow.hpp>
#include <SDL.h>

SDLppWindow::SDLppWindow(const std::string& title, int width, int height, Uint32 flags) :
SDLppWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags)
{
}

SDLppWindow::SDLppWindow(const std::string& title, int x, int y, int width, int height, Uint32 flags)
{
	m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
}

SDLppWindow::SDLppWindow(SDLppWindow&& window) noexcept
{
	m_window = window.m_window;
	window.m_window = nullptr;
}

SDLppWindow::~SDLppWindow()
{
	if (m_window)
		SDL_DestroyWindow(m_window);
}

SDL_Window* SDLppWindow::GetHandle() const
{
	return m_window;
}

SDLppWindow& SDLppWindow::operator=(SDLppWindow&& window) noexcept
{
	std::swap(m_window, window.m_window);
	return *this;
}
