#pragma once

#include <A4Engine/Export.hpp>
#include <memory> //< std::shared_ptr
#include <string> //< std::string
#include <unordered_map> //< std::unordered_map est plus efficace que std::map pour une association cl�/valeur

class Model;
class SDLppRenderer;
class SDLppTexture;

class A4ENGINE_API ResourceManager
{
	public:
		ResourceManager(SDLppRenderer& renderer);
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		~ResourceManager();

		void Clear();

		const std::shared_ptr<Model>& GetModel(const std::string& texturePath);
		const std::shared_ptr<SDLppTexture>& GetTexture(const std::string& texturePath);

		void Purge();

		static ResourceManager& Instance();

		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

	private:
		std::shared_ptr<Model> m_missingModel;
		std::shared_ptr<SDLppTexture> m_missingTexture;
		std::unordered_map<std::string /*texturePath*/, std::shared_ptr<Model>> m_models;
		std::unordered_map<std::string /*texturePath*/, std::shared_ptr<SDLppTexture>> m_textures;
		SDLppRenderer& m_renderer;

		static ResourceManager* s_instance;
};