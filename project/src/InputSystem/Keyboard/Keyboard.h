#pragma once 

#include "../IInputDevice.h"


#include <SDL_scancode.h>
#include <array>
#include <unordered_map>

namespace dae {
	class Keyboard final : public IInputDevice{
	private:
		std::array<uint8_t, SDL_NUM_SCANCODES> m_currentState{};
		std::array<uint8_t, SDL_NUM_SCANCODES> m_previousState{};
	private:
		friend class InputManagerImpl;
		Keyboard();

	public:
		~Keyboard() = default;

		void ProcessInput() override;

		bool GetButtonHeld(uint32_t code) const override;
		bool GetButtonReleased(uint32_t code) const override;
		bool GetButtonPressed(uint32_t code) const override;

		float GetAxis1D(uint32_t code) const override;
		Vector2 GetAxis2D(uint32_t code) const override;

	};
}