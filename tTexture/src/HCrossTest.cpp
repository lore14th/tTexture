#include <tTexture/tTexture.h>
#include <tTexture/EntryPoint.h>

int main()
{
	// Call initialize before 

	// TODO: initialize log as Assimp does
	if (!tTexture::Initialize())
		exit(-1);

	const char* filepath = "assets/textures/Equirectangular.jpg";
	tTexture::CubeFormat format = tTexture::CubeFormat::EQUIRECTANGULAR;
	bool flipOnLoad = false;
	uint32_t desiredChannels = 3; // this must match the image format

	tTexture::Application app(false);
	app.SetRendererResolution(512);

	std::shared_ptr<tTexture::TextureCube> texture = app.LoadTextureCube(filepath, desiredChannels, format, flipOnLoad);
	app.ExportTexture("exporter/CubemapTest.tga", texture);
	app.ExportTexture("exporter/CubemapTest.png", texture);
	app.ExportTexture("exporter/CubemapTest.jpg", texture);
	std::shared_ptr<tTexture::TextureCube> irradianceMap = app.CreateIrradiance(texture);
	app.ExportTexture("exporter/Irradiance.tga", irradianceMap);

	std::shared_ptr<tTexture::Texture2D> brdf = app.CreateBRDF();
	app.ExportTexture("exporter/BRDF.png", brdf);
	app.ExportTexture("exporter/BRDF.tga", brdf);
	app.ExportTexture("exporter/BRDF.jpg", brdf);
}
