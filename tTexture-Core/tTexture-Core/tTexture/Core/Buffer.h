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
		void Write(byte* data, uint32_t size, uint32_t offset = 0);
		inline uint32_t GetSize() const { return Size; }

		operator bool() const
		{
			return Data;
		}

		byte& operator[](int index)
		{
			return Data[index];
		}

		byte& operator[](int index) const
		{
			return Data[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

	};

}
