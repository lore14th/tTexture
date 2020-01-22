#pragma once

#include "Core/tTextureTypes.h"

namespace tTexture {

	struct Buffer
	{
		byte* Data;
		uint32_t Size;

		Buffer();
		Buffer(byte* data, uint32_t size);
		~Buffer() = default;

		void Allocate(uint32_t size);
		void ZeroInitialze();

		byte& operator[](int index) const
		{
			return Data[index];
		}
	};

}
