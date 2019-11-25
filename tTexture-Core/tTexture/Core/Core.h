#pragma once

#include <array>
#include <string>

#include "Core/Log.h"
#include "Core/Assert.h"

using int32_t = int;
using uin32_t = unsigned int;
using byte = unsigned char;

// Project Setup
// TODO: add simple logging system	[DONE]
// TODO: add basic assertion system [DONE]
// TODO: add pre-compiled headers	[DONE]

// Basic Lib Features
// TODO: Create buffer to manage byte* memory automatically
// TODO: Use Buffer instead of byte*
// TODO: Read TextureCube from file (HCROSS / VCROSS)
// TODO: Export HCROSS files as .tga

namespace tTexture {

	enum class TextureCubeFormat
	{
		HCROSS = 0, VCROSS = 1, EQUIRECTANGULAR = 2
	};

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
