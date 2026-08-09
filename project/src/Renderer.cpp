//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "MathUtils/Math.h"
#include "MathUtils/Matrix.h"
#include "Material.h"
#include "Scene.h"
#include "Utils.h"
#include <random>

inline float random_float()
{
	static std::mt19937 gen(std::random_device{}());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	return dist(gen);
}
inline float random_float(float min, float max)
{
	return min + (max - min) * random_float();
}


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
	auto camera =  pScene->GetCamera();
	const float aspectRatio{ static_cast<float>(m_Width) / m_Height};
	for (int px{}; px < m_Width; ++px)
	{
		for (int py{}; py < m_Height; ++py)
		{
			ColorRGB finalColor{};
			Vector2 normalizedDeciceCoordinate{ 2 * (px + 0.5f) / m_Width - 1.0f, 1 - 2 * (py + 0.5f) / m_Height };
			

			for (int i{}; i < camera.samplesPerPixel; ++i) 
			{
				
				auto sampleCoord = normalizedDeciceCoordinate + Vector2{ random_float(-0.5f,0.5f) / m_Width,random_float(-0.5f,0.5f) / m_Height};
				sampleCoord.x *= aspectRatio;
				sampleCoord.x *= std::tanf(camera.fovAngle / 2.0f);
				sampleCoord.y *= std::tanf(camera.fovAngle / 2.0f);

				Vector3 rayDirection{ sampleCoord.x, sampleCoord.y , 1.0f };
				rayDirection.Normalize();
				rayDirection = camera.cameraToWorld.TransformVector(rayDirection);
				const Ray ray{ .origin = camera.origin , .direction = rayDirection };
				finalColor += ShadePixel(pScene, ray);
			}
			finalColor /= float(camera.samplesPerPixel);
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
ColorRGB dae::Renderer::ShadePixel(Scene* pScene, Ray ray) const
{
	Camera& camera = pScene->GetCamera();
	auto& materials = pScene->GetMaterials();
	auto& lights = pScene->GetLights();


	ColorRGB outColor{ };
	HitRecord hit;
	pScene->GetClosestHit(ray, hit);


	constexpr float shadowPercentage{ 0.5f };
	if (hit.didHit)
	{
		for (const auto& l : lights)
		{
			float shadow{ 1.0f };
			const Vector3 ligthDirection = dae::LightUtils::GetDirectionToLight(l, hit.origin);
			if (m_options.renderShadows)
			{
				const auto shadowRay = Ray{ hit.origin + hit.normal * 0.01f,ligthDirection ,0.01f , (l.origin - hit.origin).Magnitude()};
				if (pScene->DoesHit(shadowRay))
				{
					shadow *= shadowPercentage;
				}
			}

			const float coslambert = Clamp(Vector3::Dot(hit.normal, ligthDirection), 0.0f, 1.0f);
			const ColorRGB radiance = dae::LightUtils::GetRadiance(l, hit.origin);
			const ColorRGB brdfResult = materials[hit.materialIndex]->Shade(hit, ligthDirection, ray.direction);
			ColorRGB lightContribution = radiance * brdfResult * coslambert * shadow;
			outColor += lightContribution;
		}	
	}
	else
	{
		//background
		float a = 0.5f * (ray.direction.y + 1.0f);
		outColor = (1.0f - a) * ColorRGB(1.0f,1.0f,1.0f) + a * ColorRGB(0.5f, 0.7f, 1.0f);
	}

	return outColor;
}



bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
}
