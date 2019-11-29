#include <tTexture/tTexture.h>
#include <tTexture/EntryPoint.h>

int main()
{
	// Call initialize before 
	if (!tTexture::Initialize())
		exit(-1);

	const char* filepath = "assets/textures/fst_paint-splatter.jpg";
	bool flipOnLoad = false;
	uint32_t desiredChannels = 3; // this must match the image format

	tTexture::Texture2D texture;
	tTexture::Loader loader(filepath, desiredChannels, flipOnLoad);
	loader.LoadImageFromFile(texture);

	// Use data
	tTexture::Buffer pixels = texture.Image;
	TTEX_TRACE("{0}", texture);
	TTEX_TRACE("Allocated {0} bytes", texture.Image.GetSize());

	// Store image on disk
	tTexture::Exporter exporter("image2D.tga");
	exporter.WriteToDisk(texture);
}
