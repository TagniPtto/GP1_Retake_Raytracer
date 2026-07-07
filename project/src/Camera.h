#pragma once
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include "MathUtils/Math.h"
#include "Timer.h"


namespace dae
{
	struct Camera final
	{
		Vector3 origin{};
		float fovAngle{ 90.f };

		Vector3 forward{ Vector3::UnitZ };
		Vector3 up{ Vector3::UnitY };
		Vector3 right{ Vector3::UnitX };

		float totalPitch{ 0.f };
		float totalYaw{ 0.f };

		Matrix cameraToWorld{};

		int samplesPerPixel{1};
	public:
		Camera(const Vector3& _origin = Vector3{}, float _fovAngle = 45.0f);
		Matrix CalculateCameraToWorld();
		void Update(Timer* pTimer);
	};
}
