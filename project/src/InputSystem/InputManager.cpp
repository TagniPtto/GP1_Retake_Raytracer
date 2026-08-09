
#include "InputManager.h"


#include "../MathUtils/Vector3.h"
#include <SDL.h>

#include <vector>


#include "Gamepad/Gamepad.h"
#include "Mouse/Mouse.h"
#include "Keyboard/Keyboard.h"
#include "IInputCommand.h"

#include <iostream>

namespace dae {

	class InputManagerImpl final {

	private:
		std::unique_ptr<Mouse> m_mouse;
		std::unique_ptr<Keyboard> m_keyboard;
		std::vector<std::unique_ptr<Gamepad>> m_gamepads;

		using InputAction = std::pair<std::vector<InputBinding>, std::unique_ptr<IInputCommand>>;
		std::vector<InputAction> m_bindings;
	
	private:
		friend class InputManager;
		InputManagerImpl();
	public:

		void BindCommand(std::unique_ptr<IInputCommand>  command, std::initializer_list<InputBinding> bindings);
		bool ProcessInput();
	private:
		bool IsBindingTriggered(const InputBinding& binding, InputContext& context);
		IInputDevice* GetDevice(InputType deviceType, uint32_t deviceIndex) const;
	};


	InputManagerImpl::InputManagerImpl():
		m_mouse(std::unique_ptr<Mouse>(new Mouse())),
		m_keyboard(std::unique_ptr<Keyboard>(new Keyboard))
	{
		for (unsigned int i = 0; i < SDL_NumJoysticks(); ++i)
		{
			if (SDL_IsGameController(i))
			{
				m_gamepads.emplace_back(std::make_unique<dae::Gamepad>(i));
			}
		}
	}

	void InputManagerImpl::BindCommand(std::unique_ptr<IInputCommand>  command, std::initializer_list<InputBinding> bindings)
	{
		m_bindings.emplace_back(bindings ,std::move(command));
	}

	bool InputManagerImpl::ProcessInput()
	{
		m_mouse->ProcessInput();
		m_keyboard->ProcessInput();
		for (auto& gamepad : m_gamepads) {
			gamepad->ProcessInput();
		}

		for (auto& [bindings, command] : m_bindings)
		{
			InputContext context;
			bool triggered{true};
			for (auto& binding : bindings) 
			{
				if (!IsBindingTriggered(binding, context))
				{
					triggered = false;
					break;
				}

			}
			if (triggered) 
			{
				command->Execute(context);
			}
		}
		return true;
	}

	bool InputManagerImpl::IsBindingTriggered(const InputBinding& binding, InputContext& context)
	{
		IInputDevice* device = GetDevice(binding.deviceType, binding.deviceIndex);

		if (!device) return false;

		switch (binding.deviceValue) {
			case InputValueType::Boolean:
			{
				bool triggered{ false };

				switch (binding.trigger) {
				case InputTriggerType::Held:
					triggered = device->GetButtonHeld(binding.code);
					break;
				case InputTriggerType::Pressed:
					triggered = device->GetButtonPressed(binding.code);
					break;
				case InputTriggerType::Released:
					triggered = device->GetButtonReleased(binding.code);
					break;
				}
				if (triggered) {
					context = { .binding = binding, .value = true };
					return true;
				}
			}
			break;
			case InputValueType::Float:
			{
				float value = device->GetAxis1D(binding.code);
				if (value != 0)
				{
					context = {.binding = binding, .value = value};
					return true;
				}
			}
			break;
			case InputValueType::Vector2:
			{
				Vector2 value = device->GetAxis2D(binding.code);
				if (value.SqrMagnitude() > 0.1f)
				{
					context = { .binding = binding, .value = value };
					return true;
				}
			}
			break;
		}
		return false;
	}

	IInputDevice* dae::InputManagerImpl::GetDevice(InputType deviceType, uint32_t deviceIndex) const
	{
		switch (deviceType) {
		case InputType::Keyboard: 
		{
			return m_keyboard.get();
		}
		case InputType::Mouse:
		{
			return m_mouse.get();
		}
		case InputType::Gamepad:
		{
			if (m_gamepads.size() > 0)
			{
				return m_gamepads[deviceIndex].get();
			}
		}
		}
		return nullptr;
	}
}





dae::InputManager::InputManager() :
	m_pImpl(new dae::InputManagerImpl())
{
}

bool dae::InputManager::ProcessInput()
{
	return m_pImpl->ProcessInput();
}

void dae::InputManager::BindCommand(std::unique_ptr<IInputCommand> command, std::initializer_list<InputBinding> bindings)
{
	return m_pImpl->BindCommand(std::move(command), bindings);
}


void dae::InputManager::BindCommand(
	std::unique_ptr<IInputCommand> command,
	InputValueType valueType, 
	InputType inputType, 
	uint32_t deviceId,
	uint32_t code, 
	InputTriggerType triggerType)
{
	InputBinding binding{ .code = code, .deviceIndex = deviceId,.deviceType = inputType, .deviceValue = valueType , .trigger = triggerType };
	return BindCommand(std::move(command), {binding});
}

void dae::InputManager::BindCommand(
	std::unique_ptr<IInputCommand> command, 
	InputValueType valueType, 
	uint32_t deviceId, 
	GamepadInput code, 
	InputTriggerType triggerType)
{
	BindCommand(std::move(command),  valueType, InputType::Gamepad, deviceId, uint32_t(code), triggerType);
}
void dae::InputManager::BindCommand(
	std::unique_ptr<IInputCommand> command,
	InputValueType valueType, 
	KeyboardInput code, 
	InputTriggerType triggerType)
{
	BindCommand(std::move(command),   valueType, InputType::Keyboard, 0, uint32_t(code), triggerType);
}
void dae::InputManager::BindCommand(
	std::unique_ptr<IInputCommand> command,
	InputValueType valueType, 
	MouseInput code,
	InputTriggerType triggerType)
{
	BindCommand(std::move(command),  valueType, InputType::Mouse, 0, uint32_t(code), triggerType);
}
dae::InputManager& dae::InputManager::Get()
{
	static dae::InputManager instance;
	return instance;
}
dae::InputManager::~InputManager() = default;

