#pragma once


#include "../IInputDevice.h"
#include "../../MathUtils/Vector2.h"


namespace dae 
{
	class Mouse final : public IInputDevice
	{
		friend class InputManagerImpl;
	private:
		Mouse() = default;
		int m_x{};
		int m_y{};
		int m_deltaX{};
		int m_deltaY{};
		Uint32 m_currentButtonState{};
		Uint32 m_previousButtonState{};

		Uint32 m_buttonsPressedThisFrame{};
		Uint32 m_buttonsReleasedThisFrame{};
	public:

		~Mouse() = default;
		void ProcessInput() override;
		bool GetButtonHeld(uint32_t code) const override;
		bool GetButtonPressed(uint32_t code) const override;
		bool GetButtonReleased(uint32_t code) const override;

		float GetAxis1D(uint32_t code) const override;
		Vector2 GetAxis2D(uint32_t code) const override;

	};
}