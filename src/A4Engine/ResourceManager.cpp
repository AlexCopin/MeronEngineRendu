#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/Model.hpp>
#include <A4Engine/SDLppSurface.hpp>
#include <A4Engine/SDLppTexture.hpp>
#include <stdexcept>

ResourceManager::ResourceManager(SDLppRenderer& renderer) :
m_renderer(renderer)
{
	if (s_instance != nullptr)
		throw std::runtime_error("only one ResourceManager can be created");

	s_instance = this;
}

ResourceManager::~ResourceManager()
{
	s_instance = nullptr;
}

void ResourceManager::Clear()
{
	m_missingModel.reset();
	m_missingTexture.reset();
	m_models.clear();
	m_textures.clear();
}

const std::shared_ptr<Model>& ResourceManager::GetModel(const std::string& modelPath)
{
	// Avons-nous déjà ce modèle en stock ?
	auto it = m_models.find(modelPath);
	if (it != m_models.end())
		return it->second; // Oui, on peut le renvoyer

	// Non, essayons de le charger
	Model model = Model::LoadFromFile(modelPath);
	if (!model.IsValid())
	{
		// On a pas pu charger le modèle, utilisons un modèle "manquant"
		if (!m_missingModel)
			m_missingModel = std::make_shared<Model>();

		m_models.emplace(modelPath, m_missingModel);
		return m_missingModel;
	}

	it = m_models.emplace(modelPath, std::make_shared<Model>(std::move(model))).first;
	return it->second;
}

const std::shared_ptr<SDLppTexture>& ResourceManager::GetTexture(const std::string& texturePath)
{
	// Avons-nous déjà cette texture en stock ?
	auto it = m_textures.find(texturePath);
	if (it != m_textures.end())
		return it->second; // Oui, on peut la renvoyer

	// Non, essayons de la charger
	SDLppSurface surface = SDLppSurface::LoadFromFile(texturePath);
	if (!surface.IsValid())
	{
		// On a pas pu charger la surface, utilisons une texture "manquante"
		if (!m_missingTexture)
		{
			// On créé la texture la première fois qu'on en a besoin
			surface = SDLppSurface(64, 64);
			surface.FillRect(SDL_Rect{ 0, 0, 16, 16 }, 255, 0, 255, 255);
			surface.FillRect(SDL_Rect{ 16, 0, 16, 16 }, 0, 0, 0, 255);
			surface.FillRect(SDL_Rect{ 0, 16, 16, 16 }, 0, 0, 0, 255);
			surface.FillRect(SDL_Rect{ 16, 16, 16, 16 }, 255, 0, 255, 255);

			m_missingTexture = std::make_shared<SDLppTexture>(SDLppTexture::LoadFromSurface(m_renderer, surface));
		}
		
		// On enregistre cette texture comme une texture manquante (pour ne pas essayer de la charger à chaque fois)
		m_textures.emplace(texturePath, m_missingTexture);
		return m_missingTexture;
	}

	// On a réussi à charger la surface, on la transforme en texture et on l'enregistre
	std::shared_ptr<SDLppTexture> texture = std::make_shared<SDLppTexture>(SDLppTexture::LoadFromSurface(m_renderer, surface));

	// .emplace et .insert renvoient un std::pair<iterator, bool>, le booléen indiquant si la texture a été insérée dans la map (ce qu'on sait déjà ici)
	it = m_textures.emplace(texturePath, std::move(texture)).first;

	// Attention, on ne peut pas renvoyer texture directement (même sans std::move) car on renvoie une référence constante
	// qui serait alors une référence constante sur une variable temporaire détruite à la fin de la fonction (texture)

	return it->second;
}

void ResourceManager::Purge()
{
	// On va itérer sur le conteneur tout en enlevant certains éléments pendant l'itération, cela demande un peu de pratique
	for (auto it = m_textures.begin(); it != m_textures.end(); ) //< pas d'incrémentation de it
	{
		// On vérifie le compteur pour vérifier si la texture est utilisée ailleurs ou non
		if (it->second.use_count() > 1)
		{
			++it; // la texture est utilisée, on la garde et on passe à la suivante
		}
		else
		{
			// la texture n'est plus utilisée, on peut l'enlever avec .erase(it), qui renvoie un nouvel itérateur sur l'élément *suivant*
			// (celui du prochain tour de boucle = pas d'incrémentation dans ce cas)
			it = m_textures.erase(it);
		}
	}

	// Même chose pour les modèles
	for (auto it = m_models.begin(); it != m_models.end(); )
	{
		if (it->second.use_count() > 1)
			++it;
		else
			it = m_models.erase(it);
	}

}

ResourceManager& ResourceManager::Instance()
{
	if (s_instance == nullptr)
		throw std::runtime_error("ResourceManager hasn't been instantied");

	return *s_instance; 
}

ResourceManager* ResourceManager::s_instance = nullptr;
