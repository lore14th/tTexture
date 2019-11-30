#pragma once

#include "Core/NativeLoader/NativeLoader.h"

namespace tTexture::Native {

	class EquirectangularLoader : public NativeLoader
	{
	public:
		EquirectangularLoader() = default;
		~EquirectangularLoader() = default;

		virtual void ConvertToCubeMap(const Texture2D& sourceImage, TextureCube& result) const override;
	};

}