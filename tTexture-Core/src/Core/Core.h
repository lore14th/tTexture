#pragma once

#include <array>
#include <string>

using int32_t = int;
using uin32_t = unsigned int;
using byte = unsigned char;

// TODO: add simple logging system
// TODO: add basic assertion system
// TODO: Create buffer to manage byte* memory automatically
// TODO: use Buffer instead of byte*

namespace tTexture {

	struct TextureData
	{
		int32_t Width, Height, Bpp;
	};

	struct Texture2D
	{
		byte* Image;
		TextureData Data;
	};

	struct TextureCube
	{
		std::array<byte*, 6> Faces;
		TextureData Data;
	};

	enum class TextureCubeFormat
	{
		HCROSS = 0, VCROSS = 1, EQUIRECTANGULAR = 2
	};

}