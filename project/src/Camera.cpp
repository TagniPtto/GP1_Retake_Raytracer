#include "Camera.h"

#include "InputSystem/IInputCommand.h"
#include "InputSystem/InputManager.h"

#include "Timer.h"
#include <iostream>


namespace dae {
	class CameraCommand : public IInputCommand {
	public:
		Camera* cam;
		explicit CameraCommand(Camera* c) :cam(c) {}
		virtual ~CameraCommand() = default;
		virtual void Execute(dae::InputContext context) override {}
	};
	class MoveCameraCommand : public CameraCommand {
	public:
		explicit MoveCameraCommand(Camera* c) : CameraCommand(c) {}
		virtual ~MoveCameraCommand() = default;
		
		virtual void Execute(dae::InputContext context) override
		{
			auto direction = std::get<Vector2> (context.value);
			direction.Normalize();
			const auto deltaTime = Timer::Get().GetElapsed();
			const auto movement = (cam->forward * -direction.y + cam->right * direction.x) * cam->movementSensitivity * deltaTime;
			cam->origin += movement;
		}

	};
	class MoveCameraDirection : public MoveCameraCommand {
	public:
		Vector2 direction{};
		explicit MoveCameraDirection(Camera* c, Vector2 dir):
			MoveCameraCommand(c) , direction(dir)
		{}
		virtual ~MoveCameraDirection() = default;

		virtual void Execute(dae::InputContext context) override
		{
			context.value = direction * std::get<float>(context.value);
			MoveCameraCommand::Execute(context);
		}
	};
	class RotateCameraCommand : public CameraCommand {
	public:
		explicit RotateCameraCommand(Camera* c) :CameraCommand(c) {}

		virtual ~RotateCameraCommand() = default;

		void Execute(dae::InputContext context) override
		{
			auto direction = std::get<Vector2>(context.value);
			direction.Normalize();
			const auto deltaTime = Timer::Get().GetElapsed();

			cam->totalPitch += -direction.y * cam->rotationSensitivity * deltaTime ;
			cam->totalYaw += direction.x * cam->rotationSensitivity * deltaTime;
		}

	};
}


void dae::Camera::Update(Timer* pTimer)
{
	const auto rotation = Matrix::CreateRotation(totalPitch, totalYaw, 0);
	forward = rotation.TransformVector(Vector3::UnitZ);
	right = Vector3::Cross(Vector3::UnitY, forward).Normalized();
	up = Vector3::Cross(forward, right).Normalized();

	cameraToWorld = CalculateCameraToWorld();
}

dae::Camera::Camera(const Vector3 & _origin, float _fovAngle):
	origin(_origin),
	fovAngle(_fovAngle)
{

	InputManager::Get().BindCommand(
		std::make_unique<dae::MoveCameraCommand>(this),
		InputValueType::Vector2,
		0,
		GamepadInput::LeftThumb,
		InputTriggerType::Held);
	InputManager::Get().BindCommand(
		std::make_unique<dae::RotateCameraCommand>(this),
		InputValueType::Vector2,
		0,
		GamepadInput::RightThumb,
		InputTriggerType::Held);

	InputManager::Get().BindCommand(
		std::make_unique<dae::RotateCameraCommand>(this),
		{
		InputBinding{
			.code = (uint32_t)MouseInput::LeftButton,
			.deviceIndex = 0,
			.deviceType = InputType::Mouse,
			.deviceValue = InputValueType::Boolean,
			.trigger = InputTriggerType::Held },
		InputBinding{
			.code = (uint32_t)MouseInput::Move,
			.deviceIndex = 0,
			.deviceType = InputType::Mouse,
			.deviceValue = InputValueType::Vector2,
			.trigger = InputTriggerType::Held }
		}
	);

	InputManager::Get().BindCommand(
		std::make_unique<dae::MoveCameraDirection>(this, Vector2{ 0.0f, -1.0f}),
		InputValueType::Float,
		KeyboardInput::KeyW);
	InputManager::Get().BindCommand(
		std::make_unique<dae::MoveCameraDirection>(this, Vector2{ 0.0f,1.0f }),
		InputValueType::Float,
		KeyboardInput::KeyS);
	InputManager::Get().BindCommand(
		std::make_unique<dae::MoveCameraDirection>(this, Vector2{ -1.0f,0.0f }),
		InputValueType::Float,
		KeyboardInput::KeyA);
	InputManager::Get().BindCommand(
		std::make_unique<dae::MoveCameraDirection>(this, Vector2{ 1.0f,0.0f }),
		InputValueType::Float,
		KeyboardInput::KeyD);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
}

dae::Matrix dae::Camera::CalculateCameraToWorld()
{
	const auto lookAtMatrix = Matrix::CreateLookAtLH(origin, forward, up);
	return lookAtMatrix;
}

