#include <A4Engine/SDLppTexture.hpp>
#include <A4Engine/SDLppRenderer.hpp>
#include <A4Engine/SDLppSurface.hpp>
#include <SDL.h>
#include <SDL_image.h>

SDLppTexture::SDLppTexture(SDLppTexture&& texture) noexcept :
m_filepath(std::move(texture.m_filepath))
{
	m_texture = texture.m_texture;
	texture.m_texture = nullptr;
}

SDLppTexture::~SDLppTexture()
{
	if (m_texture)
		SDL_DestroyTexture(m_texture);
}

const std::string& SDLppTexture::GetFilepath() const
{
	return m_filepath;
}

SDL_Texture* SDLppTexture::GetHandle() const
{
	return m_texture;
}

SDL_Rect SDLppTexture::GetRect() const
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	SDL_QueryTexture(m_texture, nullptr, nullptr, &rect.w, &rect.h);

	return rect;
}

SDLppTexture& SDLppTexture::operator=(SDLppTexture&& texture) noexcept
{
	// Les classes peuvent être move directement
	m_filepath = std::move(texture.m_filepath);

	// On possède déjà potentiellement une texture
	// On la donne à texture (qui va être détruit de toute façon)
	// tout en volant son pointeur : on échange donc les pointeurs
	// => std::swap
	std::swap(m_texture, texture.m_texture);
	return *this;
}

SDLppTexture SDLppTexture::LoadFromFile(SDLppRenderer& renderer, const std::string& filepath)
{
	return LoadFromSurface(renderer, SDLppSurface::LoadFromFile(filepath));
}

SDLppTexture SDLppTexture::LoadFromSurface(SDLppRenderer& renderer, const SDLppSurface& surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetHandle(), surface.GetHandle());
	return SDLppTexture(texture, surface.GetFilepath());
}

SDLppTexture::SDLppTexture(SDL_Texture* texture, std::string filepath) :
m_texture(texture),
m_filepath(std::move(filepath))
{
}
