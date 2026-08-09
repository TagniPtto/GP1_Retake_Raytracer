
#include <SDL_mouse.h>

#include "Mouse.h"
#include "../InputTypes.h"

void dae::Mouse::ProcessInput()
{
	m_previousButtonState = m_currentButtonState;

	int x{};
	int y{};
	m_currentButtonState = SDL_GetMouseState(&x, &y);
	SDL_GetRelativeMouseState(&m_deltaX, &m_deltaY);
	
	m_x = x;
	m_y = y;

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
		return float(m_deltaX);
	case MouseInput::MoveY:
		return float(m_deltaX);
	case MouseInput::MouseX:
		return float(m_x);
	case MouseInput::MouseY:
		return float(m_y);
	default:
		break;
	}
	return 0.0f;
}

dae::Vector2 dae::Mouse::GetAxis2D(uint32_t code) const
{
	switch (MouseInput(code)) {
	case MouseInput::Move:
		return Vector2(m_deltaX, m_deltaY);
	case MouseInput::Mouse:
		return Vector2(m_x,m_y);
	default:
		break;
	}
	return Vector2();
}
