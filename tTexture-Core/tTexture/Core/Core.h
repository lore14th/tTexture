#pragma once

#include <array>
#include <string>

#include "Core/Buffer.h"
#include "Core/Assert.h"

namespace tTexture {

	using int32_t = int;
	using uin32_t = unsigned int;
	using byte = unsigned char;

	enum class CubeFormat
	{
		HCROSS = 0, VCROSS = 1, EQUIRECTANGULAR = 2
	};

	struct TextureData
	{
		int32_t Width, Height, Bpp;
	};

	struct Texture2D
	{
		Buffer Image;
		TextureData Data;
	};

	struct TextureCube
	{
		std::array<Buffer, 6> Images;
		TextureData Data;
	};
	
	inline std::ostream& operator<<(std::ostream& os, const TextureData& obj)
	{
		os << "Width: " << obj.Width << "\tHeight: " << obj.Height << "\tBpp: " << obj.Bpp << std::endl;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Texture2D& obj)
	{
		os << "Width: " << obj.Data.Width << "\tHeight: " << obj.Data.Height << "\tBpp: " << obj.Data.Bpp << std::endl;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const TextureCube& obj)
	{
		os << "Width: " << obj.Data.Width << "\tHeight: " << obj.Data.Height << "\tBpp: " << obj.Data.Bpp << std::endl;
		return os;
	}

}
