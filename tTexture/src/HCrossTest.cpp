#include <tTexture/tTexture.h>
#include <tTexture/EntryPoint.h>

int main()
{
	// Call initialize before 

	// TODO: initialize log as Assimp does
	if (!tTexture::Initialize())
		exit(-1);

	const char* filepath = "assets/textures/H_Cross.png";
	tTexture::CubeFormat format = tTexture::CubeFormat::HCROSS;
	bool flipOnLoad = false;
	uint32_t desiredChannels = 4; // this must match the image format

	tTexture::Application app(false);
	app.SetRendererResolution(512);

	std::shared_ptr<tTexture::TextureCube> texture = app.LoadTextureCube(filepath, desiredChannels, format, flipOnLoad);
	app.ExportTexture("exporter/CubemapTest.tga", texture);
	std::shared_ptr<tTexture::TextureCube> irradianceMap = app.CreateIrradiance(texture);
	app.ExportTexture("exporter/Irradiance.tga", irradianceMap);

	std::shared_ptr<tTexture::Texture2D> brdf = app.CreateBRDF();
	app.ExportTexture("exporter/BRDF.tga", brdf);
	
	// TODO: remove
	// Use data 
	//tTexture::Buffer pixels = texture->Images[(int)tTexture::Face::POS_X];
	//TTEX_TRACE("{0}", texture);
	//TTEX_TRACE("Allocated {0} bytes", texture->Images[(int)tTexture::Face::POS_X].GetSize());
}
