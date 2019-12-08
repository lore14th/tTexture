#include <tTexture/tTexture.h>
#include <tTexture/EntryPoint.h>

int main()
{
	// Call initialize before 
	if (!tTexture::Initialize())
		exit(-1);

	const char* filepath = "assets/textures/Equirectangular.jpg";
	tTexture::CubeFormat format = tTexture::CubeFormat::EQUIRECTANGULAR;
	bool flipOnLoad = false;
	uint32_t desiredChannels = 3; // this must match the image format

	tTexture::Application app(false);

	std::shared_ptr<tTexture::TextureCube> texture = app.LoadTextureCube(filepath, desiredChannels, format, flipOnLoad);

	// Store image on disk
	app.ExportTexture("exporter/CubemapTest.tga", texture);

	// TODO: remove
	// Use data 
	tTexture::Buffer pixels = texture->Images[(int)tTexture::Face::POS_X];
	TTEX_TRACE("{0}", texture);
	TTEX_TRACE("Allocated {0} bytes", texture->Images[(int)tTexture::Face::POS_X].GetSize());
}
