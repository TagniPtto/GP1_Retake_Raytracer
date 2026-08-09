#pragma once
#include <memory>
#include <cstdint>
#include "InputTypes.h"

namespace dae
{
	class IInputCommand;
	class InputManagerImpl;
	class InputManager final
	{
	private:
		InputManager();

	public:
		static InputManager& Get();

		~InputManager();
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		bool ProcessInput();

		void BindCommand(std::unique_ptr<IInputCommand> command, std::initializer_list<InputBinding> bindings);
		void BindCommand(std::unique_ptr<IInputCommand> command, InputValueType valueType, InputType inputType, uint32_t deviceId, uint32_t code, InputTriggerType triggerType);
		
		void BindCommand(std::unique_ptr<IInputCommand> command, InputValueType valueType, uint32_t deviceId,GamepadInput code,		InputTriggerType triggerType = InputTriggerType::Held);
		void BindCommand(std::unique_ptr<IInputCommand> command, InputValueType valueType, KeyboardInput code,						InputTriggerType triggerType = InputTriggerType::Held);
		void BindCommand(std::unique_ptr<IInputCommand> command, InputValueType valueType, MouseInput code,							InputTriggerType triggerType = InputTriggerType::Held);

	private:
		std::unique_ptr<InputManagerImpl> m_pImpl;
	};

}
