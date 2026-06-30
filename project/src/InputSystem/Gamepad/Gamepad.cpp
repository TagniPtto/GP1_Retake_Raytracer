#include "Gamepad.h"

#include "IGamepadImpl.h"
#include "SDLControllerImpl.h"

#include "../InputTypes.h"


dae::Gamepad::Gamepad(unsigned int id) :
	m_pImpl(std::make_unique<SDLControllerImpl>(id))
{
}
dae::Gamepad::~Gamepad() = default;

unsigned int dae::Gamepad::GetId() const
{
	return m_pImpl->GetId();
}
void dae::Gamepad::ProcessInput()
{
	return m_pImpl->ProcessInput();
}

bool dae::Gamepad::GetButtonHeld(uint32_t code) const
{
	return m_pImpl->IsButtonHeld(code);
}

bool dae::Gamepad::GetButtonReleased(uint32_t code) const
{
	return m_pImpl->IsButtonReleased(code);
}

bool dae::Gamepad::GetButtonPressed(uint32_t code) const
{
	return m_pImpl->IsButtonPressed(code);
}

float dae::Gamepad::GetAxis1D(uint32_t code) const
{
	switch (GamepadInput(code)) {
		case GamepadInput::LeftTrigger:
			return m_pImpl->GetLeftTrigger();
		case GamepadInput::RightTrigger:
			return m_pImpl->GetRightTrigger();
		default:
			break;
	}
	return 0;
}

dae::Vector2 dae::Gamepad::GetAxis2D(uint32_t code) const
{
	switch (GamepadInput(code)) {
	case GamepadInput::LeftThumb:
		return dae::Vector2{ m_pImpl->GetLeftThumbX(),m_pImpl->GetLeftThumbY() };
	case GamepadInput::RightThumb:
		return dae::Vector2{ m_pImpl->GetRightThumbX(),m_pImpl->GetRightThumbY() };
	default:
		break;
	}
	return dae::Vector2{};
}






