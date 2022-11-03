#include <A4Engine/SDLppImGui.hpp>
#include <A4Engine/SDLppRenderer.hpp>
#include <A4Engine/SDLppWindow.hpp>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

SDLppImGui::SDLppImGui(SDLppWindow& window, SDLppRenderer& renderer)
{
	// Setup imgui
	IMGUI_CHECKVERSION();
	m_context = ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(window.GetHandle(), renderer.GetHandle());
	ImGui_ImplSDLRenderer_Init(renderer.GetHandle());
}

SDLppImGui::~SDLppImGui()
{
	// Cleanup
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext(m_context);
}

ImGuiContext* SDLppImGui::GetContext()
{
	return m_context;
}

void SDLppImGui::Render()
{
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void SDLppImGui::NewFrame()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void SDLppImGui::ProcessEvent(SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}
