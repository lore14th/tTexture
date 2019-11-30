#pragma once

namespace tTexture::Native {

	class NativeLoader
	{
	public:
		virtual void ConvertToCubeMap(const Texture2D& sourceImage, TextureCube& result) const = 0;

	public:
		static void ReadSquareFace(Face face, CubeFormat format, const Texture2D& sourceImage, TextureCube& result);
		static void FlipFaceVertically(Face face, TextureCube& result);
	};

}