#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Renderable.hpp>
#include <A4Engine/Vector2.hpp>
#include <SDL.h>
#include <memory>

class SDLppRenderer;
class SDLppTexture;
class Transform;

class A4ENGINE_API Sprite : public Renderable // Une portion d'une texture
{
	public:
		Sprite(std::shared_ptr<const SDLppTexture> texture);
		Sprite(std::shared_ptr<const SDLppTexture> texture, const SDL_Rect& rect);
		Sprite(const Sprite&) = default;
		Sprite(Sprite&&) = default;
		~Sprite() = default;

		void Draw(SDLppRenderer& renderer, const Transform& cameraTransform, const Transform& transform) override;

		int GetHeight() const;
		const Vector2f& GetOrigin() const;
		int GetWidth() const;

		void Resize(int width, int height);

		void SetOrigin(const Vector2f& origin);
		void SetRect(SDL_Rect rect);

		Sprite& operator=(const Sprite&) = default;
		Sprite& operator=(Sprite&&) = default;

	private:
		std::shared_ptr<const SDLppTexture> m_texture;
		SDL_Rect m_rect;
		Vector2f m_origin;
		int m_width;
		int m_height;
};