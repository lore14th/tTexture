#pragma once

#include "Core/NativeLoader/NativeLoader.h"

namespace tTexture::Native {

	class VCrossLoader : public NativeLoader
	{
	public:
		VCrossLoader() = default;
		~VCrossLoader() = default;

		virtual void ConvertToCubeMap(const Texture2D& sourceImage, TextureCube& result) const override;
	};

}