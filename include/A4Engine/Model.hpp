#pragma once

#include <A4Engine/Color.hpp>
#include <A4Engine/Export.hpp>
#include <A4Engine/Renderable.hpp>
#include <A4Engine/Vector2.hpp>
#include <nlohmann/json_fwd.hpp> //< header spécial qui fait des déclarations anticipées des classes de la lib
#include <SDL.h>
#include <filesystem>
#include <memory>
#include <vector>

class SDLppRenderer;
class SDLppTexture;
class Transform;

struct ModelVertex
{
	Vector2f pos;
	Vector2f uv;
	Color color;
};

class A4ENGINE_API Model : public Renderable // Un ensemble de triangles
{
	public:
		Model() = default;
		Model(std::shared_ptr<const SDLppTexture> texture, std::vector<ModelVertex> vertices, std::vector<int> indices);
		Model(const Model&) = default;
		Model(Model&&) = default;
		~Model() = default;

		void Draw(SDLppRenderer& renderer, const Transform& cameraTransform, const Transform& transform) override;

		bool IsValid() const;

		bool SaveToFile(const std::filesystem::path& filepath) const;
		nlohmann::ordered_json SaveToJSon() const;

		Model& operator=(const Model&) = delete;
		Model& operator=(Model&&) = default;

		static Model LoadFromFile(const std::filesystem::path& filepath);
		static Model LoadFromJSon(const nlohmann::json& doc);

	private:
		bool SaveToFileRegular(const std::filesystem::path& filepath) const;
		bool SaveToFileCompressed(const std::filesystem::path& filepath) const;
		bool SaveToFileBinary(const std::filesystem::path& filepath) const;

		static Model LoadFromFileRegular(const std::filesystem::path& filepath);
		static Model LoadFromFileCompressed(const std::filesystem::path& filepath);
		static Model LoadFromFileBinary(const std::filesystem::path& filepath);

		std::shared_ptr<const SDLppTexture> m_texture;
		std::vector<ModelVertex> m_vertices;
		std::vector<SDL_Vertex> m_sdlVertices;
		std::vector<int> m_indices;
};