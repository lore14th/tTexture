#pragma once

#include <array>
#include <string>

#include "Core/Buffer.h"
#include "Core/tTextureTypes.h"

namespace tTexture {

	enum class OutputFormat
	{
		NONE = -1, Tga = 0, Png = 1, Jpg = 2
	};

	enum class CubeFormat
	{
		HCROSS = 0, VCROSS = 1, EQUIRECTANGULAR = 2
	};

	enum class Face
	{
		NONE = -1, POS_X = 0, NEG_X, POS_Y = 2, NEG_Y = 3, POS_Z = 4, NEG_Z = 5
	};

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
		~Texture2D() = default;

		void AllocateTexture();
		std::string ToString() const;
	};

	struct TextureCube
	{
		std::array<Buffer, 6> Images;
		TextureData Data;

		TextureCube() = default;
		TextureCube(int32_t faceSize, int32_t bpp);
		TextureCube(const TextureCube& other);
		~TextureCube() = default;

		void AllocateTexture();
		std::string ToString() const;
	};

	struct PrefilteredTextureCube
	{
		std::vector<std::shared_ptr<TextureCube>> Levels;

		inline void Allocate(uint32_t mipLevels) { Levels.reserve(mipLevels * sizeof(TextureCube)); }
		inline uint32_t GetLevelsCount() { return (uint32_t)Levels.size(); }

		inline void PushLevel(std::shared_ptr<TextureCube> level) { Levels.push_back(level); }
		inline const std::shared_ptr<TextureCube> GetLevel(uint32_t level) { return Levels[level]; }
	};

}

namespace tTexture { // helper functions

	// returns the filepath extension
	OutputFormat RetrieveOutputFormat(const std::string& filepath);

	// returns the face in witch x and y are in
	Face SelectFace(uint32_t x, uint32_t y, uint32_t faceSize);

	// returns the coordinates of the upper left corner of the face on the HCross file
	std::pair<uint32_t, uint32_t> GetFaceLimits(CubeFormat format, Face face, uint32_t faceSize);

	// returns the coordinates relative to the cube face, given the x and y coordinates relative to the HCross file.
	std::pair<uint32_t, uint32_t> GetCoordinatesRelativeToFace(uint32_t x, uint32_t y, uint32_t faceSize, Face face);
	//std::pair<uint32_t, uint32_t> GetCoordinatesRelativeToFace(uint32_t x, uint32_t y, uint32_t faceSize, Face face, uint32_t mipLevel);

	// returns an rgb copy of the source texture. If the texture is already rgb, it does nothing
	std::shared_ptr<Texture2D> RemoveAlphaChannel(const std::shared_ptr<Texture2D>& texture);

	// returns the mip map count
	int32_t CalculateMipMapCount(int32_t width, int32_t height);

	// Returns the width of a pre-filtered texture given the number of mips
	int32_t CalculatePrefilterTextureWidth(uint32_t mipCount);

	// Returns -1 if sizes are not valid
	int32_t CalculatePrefilterTextureMipCount(int32_t width, int32_t height);
}

