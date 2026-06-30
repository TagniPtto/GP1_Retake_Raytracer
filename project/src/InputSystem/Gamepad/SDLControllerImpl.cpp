#include "SDLControllerImpl.h"

#include <limits>

#include "../InputTypes.h"


SDL_GameControllerButton ToSDLButton(dae::GamepadInput input);


bool dae::SDLControllerImpl::IsButtonHeld(uint32_t code) const
{
	return m_currentButtonState[ToSDLButton(GamepadInput(code))];
}

bool dae::SDLControllerImpl::IsButtonReleased(uint32_t code) const
{
	return !m_currentButtonState[ToSDLButton(GamepadInput(code))] && m_previousButtonState[ToSDLButton(GamepadInput(code))];
}

bool dae::SDLControllerImpl::IsButtonPressed(uint32_t code) const
{
	return m_currentButtonState[ToSDLButton(GamepadInput(code))] && !m_previousButtonState[ToSDLButton(GamepadInput(code))];;
}

void dae::SDLControllerImpl::ProcessInput()
{

	m_previousButtonState = m_currentButtonState;

	for (int i{}; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
	{
		m_currentButtonState[i] = SDL_GameControllerGetButton(m_gamepad,static_cast<SDL_GameControllerButton>(i));
	}

	constexpr static float AXIS_MAX = static_cast<float>(std::numeric_limits<Sint16>::max());

	
	
	leftThumbX	= SDL_GameControllerGetAxis(m_gamepad, SDL_CONTROLLER_AXIS_LEFTX ) / AXIS_MAX;
	leftThumbY	= SDL_GameControllerGetAxis(m_gamepad, SDL_CONTROLLER_AXIS_LEFTY) / AXIS_MAX;
	
	rightThumbX	= SDL_GameControllerGetAxis(m_gamepad, SDL_CONTROLLER_AXIS_RIGHTX) / AXIS_MAX;
	rightThumbY	= SDL_GameControllerGetAxis(m_gamepad, SDL_CONTROLLER_AXIS_RIGHTY)	/ AXIS_MAX;
	
	leftTrigger	= SDL_GameControllerGetAxis(m_gamepad, SDL_CONTROLLER_AXIS_TRIGGERLEFT)	/ AXIS_MAX;
	rightTrigger = SDL_GameControllerGetAxis(m_gamepad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)	/ AXIS_MAX;

}

float dae::SDLControllerImpl::GetLeftThumbX() const
{
	return leftThumbX;
}

float dae::SDLControllerImpl::GetLeftThumbY() const
{
	return leftThumbY;
}

float dae::SDLControllerImpl::GetRightThumbX() const
{
	return rightThumbX;
}

float dae::SDLControllerImpl::GetRightThumbY() const
{
	return rightThumbY;
}

float dae::SDLControllerImpl::GetLeftTrigger() const
{
	return leftTrigger;
}

float dae::SDLControllerImpl::GetRightTrigger() const
{
	return rightTrigger;
}

unsigned int dae::SDLControllerImpl::GetId() const
{
	return m_id;
}

dae::SDLControllerImpl::SDLControllerImpl(unsigned int id):
	m_id(id)
{
	if (SDL_IsGameController(id))
	{
		
		m_gamepad = SDL_GameControllerOpen(id);

		if (m_gamepad)
		{
			SDL_Log("Opened Gamepad: %s", SDL_GameControllerName(m_gamepad));
		}
		else
		{
			SDL_Log("Failed to open gamepad: %s",SDL_GetError());
		}
	}
}
SDL_GameControllerButton ToSDLButton(dae::GamepadInput input)
{
	switch (input)
	{
	case dae::GamepadInput::ButtonA:         return SDL_CONTROLLER_BUTTON_A;
	case dae::GamepadInput::ButtonB:         return SDL_CONTROLLER_BUTTON_B;
	case dae::GamepadInput::ButtonX:         return SDL_CONTROLLER_BUTTON_X;
	case dae::GamepadInput::ButtonY:         return SDL_CONTROLLER_BUTTON_Y;
	case dae::GamepadInput::LeftShoulder:    return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	case dae::GamepadInput::RightShoulder:   return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	case dae::GamepadInput::Start:           return SDL_CONTROLLER_BUTTON_START;
	case dae::GamepadInput::Back:            return SDL_CONTROLLER_BUTTON_BACK;
	case dae::GamepadInput::LeftThumb:       return SDL_CONTROLLER_BUTTON_LEFTSTICK;
	case dae::GamepadInput::RightThumb:      return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
	case dae::GamepadInput::DPadUp:          return SDL_CONTROLLER_BUTTON_DPAD_UP;
	case dae::GamepadInput::DPadDown:        return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	case dae::GamepadInput::DPadLeft:        return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	case dae::GamepadInput::DPadRight:       return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	default:                                 return SDL_CONTROLLER_BUTTON_INVALID;
	}
}
