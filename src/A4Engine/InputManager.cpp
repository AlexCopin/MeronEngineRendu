#include <A4Engine/InputManager.hpp>
#include <stdexcept>

InputManager::InputManager()
{
	if (s_instance != nullptr)
		throw std::runtime_error("only one InputManager can be created");

	s_instance = this;
}

InputManager::~InputManager()
{
	s_instance = nullptr;
}

void InputManager::BindKeyPressed(SDL_KeyCode keyCode, std::string action)
{
	if (!action.empty())
		m_keyToAction[keyCode] = std::move(action);
	else
		m_keyToAction.erase(keyCode);
}

void InputManager::BindMouseButtonPressed(MouseButton button, std::string action)
{
	// Plutôt que de traduire depuis notre enum vers les defines de la SDL à chaque événement
	// on peut le faire une seule fois au binding (plus efficace)
	int mouseButton;
	switch (button)
	{
		case MouseButton::Left:   mouseButton = SDL_BUTTON_LEFT;   break;
		case MouseButton::Right:  mouseButton = SDL_BUTTON_RIGHT;  break;
		case MouseButton::Middle: mouseButton = SDL_BUTTON_MIDDLE; break;
		case MouseButton::X1:     mouseButton = SDL_BUTTON_X1;     break;
		case MouseButton::X2:     mouseButton = SDL_BUTTON_X2;     break;
		default:
			return; //< ne devrait pas arriver
	}

	m_mouseButtonToAction[mouseButton] = std::move(action);
}

void InputManager::BindControllerButton(SDL_GameControllerButton button, std::string action)
{
	m_controllerButtonToAction[button] = std::move(action);
}

void InputManager::ClearBindings()
{
	m_controllerButtonToAction.clear();
	m_mouseButtonToAction.clear();
	m_keyToAction.clear();
}

void InputManager::HandleEvent(const SDL_Event& event)
{
	switch (event.type)
	{
		case SDL_CONTROLLERBUTTONDOWN:
		{
			auto it = m_controllerButtonToAction.find(static_cast<SDL_GameControllerButton>(event.cbutton.button));
			if (it != m_controllerButtonToAction.end())
				TriggerAction(it->second);

			break;
		}

		case SDL_CONTROLLERBUTTONUP:
		{
			auto it = m_controllerButtonToAction.find(static_cast<SDL_GameControllerButton>(event.cbutton.button));
			if (it != m_controllerButtonToAction.end())
				ReleaseAction(it->second);

			break;
		}

		case SDL_KEYDOWN:
		{
			auto it = m_keyToAction.find(event.key.keysym.sym);
			if (it != m_keyToAction.end())
				TriggerAction(it->second);

			break;
		}

		case SDL_KEYUP:
		{
			auto it = m_keyToAction.find(event.key.keysym.sym);
			if (it != m_keyToAction.end())
				ReleaseAction(it->second);

			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			auto it = m_mouseButtonToAction.find(event.button.button);
			if (it != m_mouseButtonToAction.end())
				TriggerAction(it->second);

			break;
		}

		case SDL_MOUSEBUTTONUP:
		{
			auto it = m_mouseButtonToAction.find(event.button.button);
			if (it != m_mouseButtonToAction.end())
				ReleaseAction(it->second);

			break;
		}
	}
}

bool InputManager::IsActive(const std::string& action) const
{
	auto it = m_actions.find(action);
	if (it == m_actions.end())
		return false;

	const ActionData& actionData = it->second;
	return actionData.isActive;
}

void InputManager::OnAction(std::string action, std::function<void(bool)> func)
{
	ActionData& actionData = GetActionData(action);
	actionData.func = std::move(func);
}

InputManager& InputManager::Instance()
{
	if (!s_instance)
		throw std::runtime_error("InputManager hasn't been instantied");

	return *s_instance;
}

InputManager::ActionData& InputManager::GetActionData(const std::string& action)
{
	auto it = m_actions.find(action);
	if (it != m_actions.end())
		return it->second;
	else
	{
		ActionData& actionData = m_actions[action];
		actionData.isActive = false;

		return actionData;
	}
}

void InputManager::TriggerAction(const std::string& action)
{
	ActionData& actionData = GetActionData(action);
	actionData.isActive = true;
	if (actionData.func)
		actionData.func(true);
}

void InputManager::ReleaseAction(const std::string& action)
{
	ActionData& actionData = GetActionData(action);
	actionData.isActive = false;
	if (actionData.func)
		actionData.func(false);
}

InputManager* InputManager::s_instance = nullptr;