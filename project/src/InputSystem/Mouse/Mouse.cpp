
#include <SDL_mouse.h>

#include "Mouse.h"
#include "../InputTypes.h"


void dae::Mouse::ProcessInput()
{
	m_previousButtonState = m_currentButtonState;
	m_currentButtonState = SDL_GetMouseState(&m_x, &m_y);

	Uint32 buttonsChanged = m_previousButtonState ^ m_currentButtonState;
	m_buttonsReleasedThisFrame = m_previousButtonState & buttonsChanged;
	m_buttonsPressedThisFrame = buttonsChanged & m_currentButtonState;
}


bool dae::Mouse::GetButtonHeld(uint32_t code) const
{
	return m_currentButtonState & code;
}

bool dae::Mouse::GetButtonPressed(uint32_t code) const
{
	return m_buttonsPressedThisFrame & code;
}

bool dae::Mouse::GetButtonReleased(uint32_t code) const
{
	return m_buttonsReleasedThisFrame & code;
}

float dae::Mouse::GetAxis1D(uint32_t code) const
{
	switch (MouseInput(code)) {
	case MouseInput::MoveX :
		return float(m_x);
	case MouseInput::MoveY:
		return float(m_y);
	default:
		break;
	}
	return 0.0f;
}

dae::Vector2 dae::Mouse::GetAxis2D(uint32_t) const
{

	return Vector2();
}
