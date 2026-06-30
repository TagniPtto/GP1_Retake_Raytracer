#pragma once

#include "../MathUtils/Vector2.h"
#include <cstdint>

namespace dae {

	class IInputDevice
	{
	public:
		virtual bool GetButtonHeld(uint32_t code) const = 0;
		virtual bool GetButtonPressed(uint32_t code) const = 0;
		virtual bool GetButtonReleased(uint32_t code) const = 0;

		virtual float GetAxis1D(uint32_t code) const = 0;
		virtual Vector2 GetAxis2D(uint32_t code) const = 0;

		virtual void ProcessInput() = 0;

		IInputDevice() = default;
		virtual ~IInputDevice() = default;

		IInputDevice(IInputDevice&&) = delete;
		IInputDevice(const IInputDevice&) = delete;
		IInputDevice& operator=(IInputDevice&&) = delete;
		IInputDevice& operator=(const IInputDevice&) = delete;
	};
}