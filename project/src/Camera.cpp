#include "Camera.h"

#include "InputSystem/IInputCommand.h"
#include "InputSystem/InputManager.h"

#include <iostream>


namespace dae {
	class MoveCameraCommand : public IInputCommand{
	public:
		Camera* cam;
		virtual ~MoveCameraCommand() = default;
		
		void Execute(dae::InputContext context) override
		{
			auto direction = std::get<Vector2> (context.value);
			direction.Normalize();
			cam->origin += cam->forward * direction.y + cam->right*direction.x;

			std::cout << "Moving" << std::endl;

		}

	};
	class RotateCameraCommand : public IInputCommand {
	public:
		Camera* cam;
		virtual ~RotateCameraCommand() = default;

		void Execute(dae::InputContext context) override
		{
			auto direction = std::get<Vector2>(context.value);
			direction.Normalize();
			cam->totalPitch += direction.x *0.01f;
			cam->totalYaw += direction.y * 0.01f;
			std::cout << "rotating" << std::endl;
		}

	};
}


void dae::Camera::Update(Timer* pTimer)
{
	const float deltaTime = pTimer->GetElapsed();

	//Keyboard Input
	const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);


	//Mouse Input
	int mouseX{}, mouseY{};
	const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

}

dae::Camera::Camera(const Vector3 & _origin, float _fovAngle):
	origin(_origin),
	fovAngle(_fovAngle)
{
	auto moveCommand = std::make_unique<dae::MoveCameraCommand>();
	auto rotateCommand = std::make_unique<dae::RotateCameraCommand>();
	moveCommand->cam = this;
	rotateCommand->cam = this;
	InputManager::Get().BindCommand(
		std::move(moveCommand),
		InputValueType::Vector2,
		0,
		GamepadInput::LeftThumb,
		InputTriggerType::Held);

	InputManager::Get().BindCommand(
		std::move(rotateCommand),
		InputValueType::Vector2,
		0,
		GamepadInput::RightThumb,
		InputTriggerType::Held);
}

dae::Matrix dae::Camera::CalculateCameraToWorld()
{
	Matrix LookAtMatrix = Matrix::CreateLookAtLH(origin, forward, up);
	return LookAtMatrix;
}

