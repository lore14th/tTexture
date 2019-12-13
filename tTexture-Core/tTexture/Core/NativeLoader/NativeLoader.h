#pragma once

#include "Renderer/OpenGLRenderer.h"

namespace tTexture::Native {

	class OfflineApplication;
	
	class NativeLoader
	{
	public:
		virtual std::shared_ptr<TextureCube> ConvertToCubeMap(const std::shared_ptr<Texture2D>& sourceImage) const = 0;

	public:
		static void ReadSquareFace(Face face, CubeFormat format, const std::shared_ptr<Texture2D>& sourceImage, std::shared_ptr<TextureCube>& result);
		static void FlipFaceVertically(Face face, std::shared_ptr<TextureCube>& result);
	};

	class HCrossLoader : public NativeLoader
	{
	public:
		HCrossLoader() = default;
		~HCrossLoader() = default;

		virtual std::shared_ptr<TextureCube> ConvertToCubeMap(const std::shared_ptr<Texture2D>& sourceImage) const override;
	};

	class VCrossLoader : public NativeLoader
	{
	public:
		VCrossLoader() = default;
		~VCrossLoader() = default;

		virtual std::shared_ptr<TextureCube> ConvertToCubeMap(const std::shared_ptr<Texture2D>& sourceImage) const override;
	};

	class EquirectangularLoader : public NativeLoader
	{
	public:
		EquirectangularLoader(const std::unique_ptr<OpenGLRenderer>& renderer);
		~EquirectangularLoader() = default;

		virtual std::shared_ptr<TextureCube> ConvertToCubeMap(const std::shared_ptr<Texture2D>& sourceImage) const override;
	private:
		const std::unique_ptr<OpenGLRenderer>& m_Renderer;
	};

}