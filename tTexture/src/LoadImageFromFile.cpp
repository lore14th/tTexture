#include <tTexture/tTexture.h>
#include <tTexture/EntryPoint.h>

int main()
{
	// Call initialize before 
	if (!tTexture::Initialize())
		exit(-1);

	const char* filepath = "assets/textures/fst_paint-splatter.jpg";
	bool flipOnLoad = false;
	uint32_t desiredChannels = 4;

	tTexture::Texture2D imageData;
	tTexture::Loader loader(filepath, flipOnLoad, desiredChannels);
	loader.LoadImageFromFile(imageData);

	// Use data
	tTexture::Buffer pixels = imageData.Image;
	TTEX_TRACE("{0}", imageData);
	TTEX_TRACE("Allocated {0} bytes", imageData.Image.GetSize());
}
