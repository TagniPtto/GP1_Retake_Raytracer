#pragma once


#include <cstdint>


namespace dae {
	class IGamepadImpl {
	public:
		IGamepadImpl() = default;
		virtual ~IGamepadImpl() = default;
		IGamepadImpl(const IGamepadImpl& gp) = delete;
		IGamepadImpl(IGamepadImpl&& gp) = delete;
		IGamepadImpl& operator=(const IGamepadImpl& gp) = delete;
		IGamepadImpl& operator=(IGamepadImpl&& gp) = delete;

		virtual unsigned int GetId() const = 0;
		virtual void ProcessInput() = 0;

		virtual bool IsButtonHeld(uint32_t code) const = 0;
		virtual bool IsButtonReleased(uint32_t code) const = 0;
		virtual bool IsButtonPressed(uint32_t code) const = 0;

		virtual float GetLeftThumbX() const = 0;
		virtual float GetLeftThumbY() const = 0;
		virtual float GetRightThumbX() const = 0;
		virtual float GetRightThumbY() const = 0;
		virtual float GetLeftTrigger() const = 0;
		virtual float GetRightTrigger() const = 0;
	};
}