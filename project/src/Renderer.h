#pragma once
#include <cstdint>
#include "ColorRGB.h"
#include "DataTypes.h"
#include "MathUtils/Vector2.h"


struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Scene;

	struct RendererOptions {
		bool renderShadows{ true };
		int renderView{};
	};
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render(Scene* pScene) const;
		bool SaveBufferToImage() const;
	private:
		ColorRGB ShadePixel(Scene* pScene, Ray ray) const;
		//Vector2 SampleDeviantPixel(Vector2 pixelCoord) const;
	private:
		SDL_Window* m_pWindow{};

		SDL_Surface* m_pBuffer{};
		uint32_t* m_pBufferPixels{};

		int m_Width{};
		int m_Height{};

		RendererOptions m_options;
	};
}
