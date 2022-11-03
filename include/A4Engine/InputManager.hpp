#pragma once

#include <A4Engine/Export.hpp>
#include <SDL.h>
#include <functional> //< std::function
#include <string> //< std::string
#include <unordered_map> //< std::unordered_map est plus efficace que std::map pour une association clé/valeur

enum class MouseButton
{
	Left,
	Right,
	Middle,
	X1,
	X2
};

class A4ENGINE_API InputManager
{
	public:
		InputManager();
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		~InputManager();

		// Appuyer sur la touche "keyCode" déclenchera "action"
		void BindKeyPressed(SDL_KeyCode keyCode, std::string action);

		// Appuyer sur le bouton "button" déclenchera "action"
		void BindMouseButtonPressed(MouseButton button, std::string action);

		// Appuyer sur le bouton "button" du contrôleur (manette) déclenchera "action"
		void BindControllerButton(SDL_GameControllerButton button, std::string action);

		// Réinitialise toutes les associations clavier/souris vers des actions
		void ClearBindings();

		// Gère l'événement de la SDL et déclenche les actions associées, s'il y en a
		void HandleEvent(const SDL_Event& event);

		// Renvoie vrai si l'action est en cours
		bool IsActive(const std::string& action) const;

		// Lorsque l'action "action" se déclenche, on appellera "func"
		void OnAction(std::string action, std::function<void(bool)> func);

		InputManager& operator=(const InputManager&) = delete;
		InputManager& operator=(InputManager&&) = delete;

		static InputManager& Instance();

	private:
		struct ActionData
		{
			std::function<void(bool)> func;
			bool isActive;
		};

		ActionData& GetActionData(const std::string& action);
		void TriggerAction(const std::string& action);
		void ReleaseAction(const std::string& action);

		std::unordered_map<int /*mouseButton*/, std::string /*action*/> m_mouseButtonToAction;
		std::unordered_map<SDL_GameControllerButton /*controllerButton*/, std::string /*action*/> m_controllerButtonToAction;
		std::unordered_map<SDL_Keycode /*key*/, std::string /*action*/> m_keyToAction;
		std::unordered_map<std::string /*action*/, ActionData> m_actions;

		static InputManager* s_instance;
};