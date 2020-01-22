#include "pch.h"
#include "Buffer.h"

#include "Debug/tTextureAssert.h"

namespace tTexture {

	Buffer::Buffer() 
		: Data(nullptr), Size(0)
	{
	}

	Buffer::Buffer(byte* data, uint32_t size) 
		: Data(data), Size(size)
	{
	}

	void Buffer::Allocate(uint32_t size)
	{
		delete[] Data;
		Data = nullptr;

		if (size == 0)
			return;

		Size = size;
		Data = new byte[Size];
	}

	void Buffer::ZeroInitialze()
	{
		if (Data)
			memset(Data, 0, Size);
	}

}
