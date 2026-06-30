
#include "InputManager.h"


#include "../MathUtils/Vector3.h"
#include <SDL.h>

#include <vector>


#include "Gamepad/Gamepad.h"
#include "Mouse/Mouse.h"
#include "Keyboard/Keyboard.h"
#include "IInputCommand.h"


#define MAX_CONTROLLERS 4

namespace dae {

	class InputManagerImpl final {

	private:
		std::unique_ptr<Mouse> m_mouse;
		std::unique_ptr<Keyboard> m_keyboard;
		std::vector<std::unique_ptr<Gamepad>> m_gamepads;


		std::vector< std::pair<InputBinding, std::unique_ptr<IInputCommand>>> m_bindings;
	private:
		friend class InputManager;
		InputManagerImpl();
	public:

		void BindCommand(std::unique_ptr<IInputCommand>  command, InputBinding binding);
		bool ProcessInput();
	private:
		void ProcessBinding(const InputBinding& binding, IInputCommand* command);
		IInputDevice* GetDevice(InputType deviceType, uint32_t deviceIndex) const;
	};




	InputManagerImpl::InputManagerImpl():
		m_mouse(std::unique_ptr<Mouse>(new Mouse())),
		m_keyboard(std::unique_ptr<Keyboard>(new Keyboard))
	{
		for (unsigned int i = 0; i < MAX_CONTROLLERS; ++i)
		{
			m_gamepads.emplace_back(std::make_unique<dae::Gamepad>(i));
		}
	}

	void InputManagerImpl::BindCommand(std::unique_ptr<IInputCommand>  command, InputBinding binding)
	{
		m_bindings.emplace_back(binding, std::move(command));
	}

	bool InputManagerImpl::ProcessInput()
	{

		m_mouse->ProcessInput();
		m_keyboard->ProcessInput();
		for (auto& gamepad : m_gamepads) {
			gamepad->ProcessInput();
		}

		for (auto& [binding, command] : m_bindings)
		{
			ProcessBinding(binding, command.get());
		}
		return true;
	}

	void InputManagerImpl::ProcessBinding(const InputBinding& binding, IInputCommand* command)
	{
		IInputDevice* device = GetDevice(binding.deviceType, binding.deviceIndex);

		if (!device) return;

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
				command->Execute(InputContext{.binding = binding,.value = true });
			}
		}
		break;
		case InputValueType::Float:
		{
			float value = device->GetAxis1D(binding.code);
			if (value != 0)
			{
				command->Execute(InputContext{ .binding = binding,.value = value });
			}
		}
		break;
		case InputValueType::Vector2:
		{
			Vector2 value = device->GetAxis2D(binding.code);
			if (std::abs(value.x) > 0.0f || std::abs(value.y) > 0.0f)
			{
				command->Execute(InputContext{ .binding = binding,.value = value });
			}
		}
		break;
		}
	}

	IInputDevice* dae::InputManagerImpl::GetDevice(InputType deviceType, uint32_t deviceIndex) const
	{
		switch (deviceType) {
		case InputType::Keyboard:
			return m_keyboard.get();
		case InputType::Mouse:
			return m_mouse.get();
		case InputType::Gamepad:
			return m_gamepads[deviceIndex].get();
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

void dae::InputManager::BindCommand(std::unique_ptr<IInputCommand> command, InputBinding binding)
{
	return m_pImpl->BindCommand(std::move(command), binding);
}

void dae::InputManager::BindCommand(std::unique_ptr<IInputCommand> command, InputType inputType, uint32_t deviceId, InputValueType valueType, uint32_t code, InputTriggerType triggerType)
{
	InputBinding binding{ .code = code, .deviceIndex = deviceId,.deviceType = inputType, .deviceValue = valueType , .trigger = triggerType };
	return BindCommand(std::move(command), binding);
}
void dae::InputManager::BindCommand(std::unique_ptr<IInputCommand> command, uint32_t deviceId, InputValueType valueType, GamepadInput code, InputTriggerType triggerType)
{
	BindCommand(std::move(command), InputType::Gamepad, deviceId, valueType, uint32_t(code), triggerType);
}
void dae::InputManager::BindCommand(std::unique_ptr<IInputCommand> command, InputValueType valueType, KeyboardInput code, InputTriggerType triggerType)
{
	BindCommand(std::move(command), InputType::Keyboard, 0, valueType, uint32_t(code), triggerType);
}
void dae::InputManager::BindCommand(std::unique_ptr<IInputCommand> command, InputValueType valueType, MouseInput code, InputTriggerType triggerType)
{
	BindCommand(std::move(command), InputType::Mouse, 0, valueType, uint32_t(code), triggerType);
}
dae::InputManager::~InputManager() = default;

