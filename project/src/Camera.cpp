#include "Camera.h"

#include "InputSystem/IInputCommand.h"


namespace dae {
	class MoveCameraCommand : public IInputCommand{
	public:
		Camera cam;
		virtual ~MoveCameraCommand() = default;
		
		void Execute(dae::InputContext context) override
		{
			context.value;
			
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

	//todo: W2
	//throw std::runtime_error("Not Implemented Yet");
}

dae::Camera::Camera(const Vector3 & _origin, float _fovAngle):
	origin(_origin),
	fovAngle(_fovAngle)
{}

dae::Matrix dae::Camera::CalculateCameraToWorld()
{
	Matrix LookAtMatrix = Matrix::CreateLookAtLH(origin, forward, up);
	return LookAtMatrix;
}

