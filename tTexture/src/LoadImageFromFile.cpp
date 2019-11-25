#include <tTexture/tTexture.h>
#include <tTexture/EntryPoint.h>

#include <iostream>

int main()
{
	// Call initialize before 
	if (!tTexture::Initialize())
		return -1;

	const char* filepath = "assets/textures/fst_paint-splatter.jpg";
	uint32_t desiredChannels = 4;
	bool flipOnLoad = false;

	tTexture::Texture2D imageData;
	tTexture::Loader loader(filepath, desiredChannels, flipOnLoad);
	loader.LoadImageFromFile(imageData);

	// Use data
	byte* pixels = imageData.Image;
	TTEX_TRACE("{0}", imageData);
}
