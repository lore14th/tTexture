#include <tTexture/tTexture.h>
#include <tTexture/EntryPoint.h>

int main()
{
	// Call initialize before 
	if (!tTexture::Initialize())
		exit(-1);

	const char* filepath = "assets/textures/H_Cross.png";
	tTexture::CubeFormat format = tTexture::CubeFormat::HCROSS;

	bool flipOnLoad = false;
	uint32_t desiredChannels = 4; // this must match the image format

	tTexture::TextureCube texture;
	tTexture::Loader loader(filepath, desiredChannels, flipOnLoad);
	loader.LoadCubeMapFromFile(format, texture);

	// Store image on disk
	tTexture::Exporter exporter("exporter/VCrossTest.tga");
	exporter.WriteToDisk(texture);

	// TODO: remove
	// Use data 
	tTexture::Buffer pixels = texture.Images[(int)tTexture::Face::POS_X];
	TTEX_TRACE("{0}", texture);
	TTEX_TRACE("Allocated {0} bytes", texture.Images[(int)tTexture::Face::POS_X].GetSize());
}
