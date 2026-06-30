#include "Keyboard.h"


#include <SDL_keyboard.h>

dae::Keyboard::Keyboard()
{
	const Uint8* kbState = SDL_GetKeyboardState(nullptr);
	memcpy(m_currentState.data(), kbState, SDL_NUM_SCANCODES * sizeof(uint8_t));
}
void dae::Keyboard::ProcessInput()
{
	m_previousState = m_currentState;
	const Uint8* kbState = SDL_GetKeyboardState(nullptr);
	memcpy(m_currentState.data(), kbState, SDL_NUM_SCANCODES * sizeof(uint8_t));
}

bool dae::Keyboard::GetButtonHeld(uint32_t code) const
{
	return m_currentState[uint8_t(code)];
}

bool dae::Keyboard::GetButtonReleased(uint32_t code) const
{
	return !m_currentState[uint8_t(code)] && m_previousState[uint8_t(code)];
}	

bool dae::Keyboard::GetButtonPressed(uint32_t code) const
{
	return m_currentState[uint8_t(code)] && !m_previousState[uint8_t(code)];
}

float dae::Keyboard::GetAxis1D(uint32_t) const
{
	return 0.0f;
}

dae::Vector2 dae::Keyboard::GetAxis2D(uint32_t) const
{
	return Vector2();
}
