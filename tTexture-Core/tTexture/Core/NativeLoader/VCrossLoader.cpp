#include "pch.h"
#include "VCrossLoader.h"
#include "Core/NativeLoader/NativeLoader.h"

#ifndef TTEX_LOADER_SINGLE_THREAD
#include <thread>
#endif

namespace tTexture::Native {

	void VCrossLoader::ConvertToCubeMap(const Texture2D& sourceImage, TextureCube& result) const
	{
		// Desired channels is 3 or 4 (asserted in Loader constructor)

		const uint32_t faceSize = sourceImage.Data.Width / 3;
		const uint32_t bpp = 4;

		TTEX_CORE_ASSERT(faceSize != 0, "Loader:Cubemap has invalid faceSize: {0}", faceSize);
		TTEX_CORE_ASSERT(faceSize == sourceImage.Data.Height / 4, "Loader:Cubemap faceSize error. Non square faces");

		for (auto& face : result.Images)
			face.Allocate(faceSize * faceSize * bpp);

		result.Data.Width = faceSize;
		result.Data.Height = faceSize;
		result.Data.Bpp = bpp;

#ifdef TTEX_LOADER_SINGLE_THREAD
		ReadSquareFace(Face::POS_X, sourceImage, result);
		ReadSquareFace(Face::NEG_X, sourceImage, result);
		ReadSquareFace(Face::POS_Y, sourceImage, result);
		ReadSquareFace(Face::NEG_Y, sourceImage, result);
		ReadSquareFace(Face::POS_Z, sourceImage, result);
		ReadSquareFace(Face::NEG_Z, sourceImage, result);
#else
		std::array<std::thread*, 6> threads;

		threads[0] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_X, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[1] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_X, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[2] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_Y, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[3] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_Y, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[4] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_Z, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[5] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_Z, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));

		for (auto& t : threads)
			t->join();
#endif
		FlipFaceVertically(Face::NEG_Z, result);
	}

}