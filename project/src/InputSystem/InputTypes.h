#pragma once
#include <variant>
#include "../MathUtils/Vector2.h"

namespace dae
{
	enum class InputType
	{
		Keyboard,
		Mouse,
		Gamepad
	};
	enum class InputValueType
	{
		Boolean,
		Float,
		Vector2,
	};
	enum class InputTriggerType {
		Released,
		Pressed,
		Held,
	};
	struct InputBinding {
		uint32_t code;

		uint32_t deviceIndex;
		InputType deviceType;

		InputValueType deviceValue;
		InputTriggerType trigger;
	};

	struct InputContext
	{
		InputBinding binding;
		std::variant<bool, float, Vector2> value;
	};

	enum class GamepadInput : uint32_t {
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		RightTrigger = 0x0400,
		LeftTrigger = 0x0800,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRight = 0x0008
	};

	enum class MouseInput : uint32_t
	{
		LeftButton,
		RightButton,
		MiddleButton,
		Button4,
		Button5,

		WheelUp,
		WheelDown,

		MoveX,
		MoveY
	};

	enum class KeyboardInput : uint32_t
	{
		// Letters
		KeyA = 4,
		KeyB,
		KeyC,
		KeyD,
		KeyE,
		KeyF,
		KeyG,
		KeyH,
		KeyI,
		KeyJ,
		KeyK,
		KeyL,
		KeyM,
		KeyN,
		KeyO,
		KeyP,
		KeyQ,
		KeyR,
		KeyS,
		KeyT,
		KeyU,
		KeyV,
		KeyW,
		KeyX,
		KeyY,
		KeyZ,

		// Numbers
		Num1 = 30,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Num0,

		// Special keys
		Enter = 40,
		Escape,
		Backspace,
		Tab,
		Space,

		Minus,
		Equals,
		LeftBracket,
		RightBracket,
		Backslash,

		Semicolon = 51,
		Apostrophe,
		Grave,
		Comma,
		Period,
		Slash,

		CapsLock = 57,

		// Function keys
		F1 = 58,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		// Navigation
		Insert = 73,
		Home,
		PageUp,
		Delete,
		End,
		PageDown,

		RightArrow,
		LeftArrow,
		DownArrow,
		UpArrow,

		// Modifiers
		LeftCtrl = 224,
		LeftShift,
		LeftAlt,

		RightCtrl = 228,
		RightShift,
		RightAlt
	};
}