#include <tTexture-Core/tTexture.h>
#include <tTexture-Core/EntryPoint.h>

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
	std::cout << "TextureData:"
		<< "\n\tWidth: "	<< imageData.Data.Width 
		<< "\n\tHeight: "	<< imageData.Data.Height 
		<< "\n\tBpp: "		<< imageData.Data.Bpp 
	<< std::endl;
}
