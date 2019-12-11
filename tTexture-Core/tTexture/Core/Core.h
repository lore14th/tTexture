#pragma once

#include <array>
#include <string>

#include "Core/Buffer.h"
#include "Core/Assert.h"

namespace tTexture { // types
	
	using int32_t = int;
	using uin32_t = unsigned int;
	using byte = unsigned char;

	enum class CubeFormat
	{
		HCROSS = 0, VCROSS = 1, EQUIRECTANGULAR = 2
	};

	enum class Face { NONE = -1, POS_X = 0, NEG_X, POS_Y = 2, NEG_Y = 3, POS_Z = 4, NEG_Z = 5 };

	struct TextureData
	{
		int32_t Width, Height, Bpp;

		TextureData() = default;
		TextureData(int32_t width, int32_t height, int32_t bpp);
	};

	struct Texture2D
	{
		Buffer Image;
		TextureData Data;

		Texture2D() = default;
		Texture2D(int32_t width, int32_t height, int32_t bpp);
		Texture2D(const Texture2D& other);
		~Texture2D();

		void AllocateTexture();
	};

	struct TextureCube
	{
		std::array<Buffer, 6> Images;
		TextureData Data;

		TextureCube() = default;
		TextureCube(int32_t faceSize, int32_t bpp);
		TextureCube(const TextureCube& other);
		~TextureCube();

		void AllocateTexture();
	};

}

namespace tTexture { // helper functions

	// returns the coordinates of the upper left corner of the face on the HCross file
	std::pair<uint32_t, uint32_t> GetFaceLimits(CubeFormat format, Face face, uint32_t faceSize);

	// returns the coordinates relative to the cube face, given the x and y coordinates relative to the HCross file.
	std::pair<uint32_t, uint32_t> GetCoordinatesRelativeToFace(uint32_t x, uint32_t y, uint32_t faceSize, Face face);

	// returns an rgb copy of the source texture. If the texture is already rgb, it does nothing
	std::shared_ptr<Texture2D> RemoveAlphaChannel(const std::shared_ptr<Texture2D>& texture);
}

namespace tTexture {

	std::pair<uint32_t, uint32_t> GetFaceLimits(CubeFormat format, Face face, uint32_t faceSize);
	
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
