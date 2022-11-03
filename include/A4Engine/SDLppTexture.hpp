#pragma once

#include <A4Engine/Export.hpp>
#include <SDL.h>
#include <string>

class SDLppRenderer;
class SDLppSurface;

class A4ENGINE_API SDLppTexture
{
	public:
		SDLppTexture(const SDLppTexture&) = delete; // constructeur par copie
		SDLppTexture(SDLppTexture&& texture) noexcept; // constructeur par mouvement
		~SDLppTexture();

		const std::string& GetFilepath() const;
		SDL_Texture* GetHandle() const;
		SDL_Rect GetRect() const;

		SDLppTexture& operator=(const SDLppTexture&) = delete; // opérateur d'assignation par copie
		SDLppTexture& operator=(SDLppTexture&&) noexcept; // opérateur d'assignation par mouvement

		static SDLppTexture LoadFromFile(SDLppRenderer& renderer, const std::string& filepath);
		static SDLppTexture LoadFromSurface(SDLppRenderer& renderer, const SDLppSurface& surface);

	private:
		SDLppTexture(SDL_Texture* texture, std::string filepath = "");

		SDL_Texture* m_texture;
		std::string m_filepath;
};