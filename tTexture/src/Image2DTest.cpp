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

	// You need to crate an tTexture::Application to access all the functionalities of the library
	tTexture::Application app(false);

	// Load a 2D image
	std::shared_ptr<tTexture::Texture2D> texture = app.LoadTexture2D(filepath, desiredChannels, flipOnLoad);

	// Use data
	tTexture::Buffer pixels = texture->Image;
	TTEX_TRACE("{0}", texture); // TODO: fix this!
	TTEX_TRACE("Allocated {0} bytes", texture->Image.GetSize());

	// Store the image to disk
	// the output format is specified by the extension of the output filepath
	// -- NOTE: right now only .tga is supported --
	app.ExportTexture("exporter/image2D.tga", texture);
}
