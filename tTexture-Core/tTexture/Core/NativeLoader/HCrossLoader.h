#pragma once

#include "Core/NativeLoader/NativeLoader.h"

namespace tTexture::Native {

	class HCrossLoader : public NativeLoader
	{
	public:
		HCrossLoader() = default;
		~HCrossLoader() = default;

		virtual void ConvertToCubeMap(const Texture2D& sourceImage, TextureCube& result) const override;
	};

}