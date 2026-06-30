#pragma once

#include <memory>

#include "../IInputDevice.h"
#include "../../MathUtils/Vector2.h"

namespace dae {
	class IGamepadImpl;

	class Gamepad final : public IInputDevice{
	private:
		std::unique_ptr<IGamepadImpl> m_pImpl;
	public:
		Gamepad(unsigned int id);
		~Gamepad();

		unsigned int GetId() const;

		void ProcessInput() override;

		bool GetButtonHeld(uint32_t code) const override;
		bool GetButtonReleased(uint32_t code) const override;
		bool GetButtonPressed(uint32_t code) const override;
		float GetAxis1D(uint32_t code) const override;
		Vector2 GetAxis2D(uint32_t code) const override;

	};
}