#pragma once

#include <A4Engine/Export.hpp>

class SDLppRenderer;
class SDLppWindow;
union SDL_Event;
struct ImGuiContext;

class A4ENGINE_API SDLppImGui
{
	public:
		SDLppImGui(SDLppWindow& window, SDLppRenderer& renderer);
		SDLppImGui(const SDLppImGui&) = delete;
		SDLppImGui(SDLppImGui&&) = delete;
		~SDLppImGui();

		ImGuiContext* GetContext();

		void Render();

		void NewFrame();

		void ProcessEvent(SDL_Event& event);

		SDLppImGui& operator=(const SDLppImGui&) = delete;
		SDLppImGui& operator=(SDLppImGui&&) = delete;

	private:
		ImGuiContext* m_context;
};
