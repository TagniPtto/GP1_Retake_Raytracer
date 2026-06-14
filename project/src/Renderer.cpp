//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Math.h"
#include "Matrix.h"
#include "Material.h"
#include "Scene.h"
#include "Utils.h"

using namespace dae;

Renderer::Renderer(SDL_Window * pWindow) :
	m_pWindow(pWindow),
	m_pBuffer(SDL_GetWindowSurface(pWindow))
{
	//Initialize
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
	m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);
}

void Renderer::Render(Scene* pScene) const
{
	Camera& camera = pScene->GetCamera();
	auto& materials = pScene->GetMaterials();
	auto& lights = pScene->GetLights();


	const float aspectRatio{ static_cast<float>(m_Width) / m_Height};
	for (int px{}; px < m_Width; ++px)
	{
		for (int py{}; py < m_Height; ++py)
		{

			
			Vector2 normalizedDeciceCoordinate{ 2 * (px + 0.5f)/m_Width - 1.0f, 1 - 2 * (py + 0.5f) / m_Height};
			normalizedDeciceCoordinate.x *= aspectRatio;
			Vector3 rayDirection{ normalizedDeciceCoordinate .x, normalizedDeciceCoordinate.y , 1.0f};
			rayDirection.Normalize();
			Ray ray{.origin = camera.origin , .direction = rayDirection};

			Sphere testSphere{.origin = {0.f,0.f ,100.f}, .radius = 50.0f , .materialIndex = 0};

			ColorRGB finalColor{ rayDirection.x,rayDirection.y,rayDirection.z };
			HitRecord hit;
			if (GeometryUtils::HitTest_Sphere(testSphere,ray, hit)) {
				float v{hit.t/100.f};
				finalColor = ColorRGB{ v, v, v};
			}


			finalColor.MaxToOne();

			m_pBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBuffer->format,
				static_cast<uint8_t>(finalColor.r * 255),
				static_cast<uint8_t>(finalColor.g * 255),
				static_cast<uint8_t>(finalColor.b * 255));
		}
	}

	//@END
	//Update SDL Surface
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
}
