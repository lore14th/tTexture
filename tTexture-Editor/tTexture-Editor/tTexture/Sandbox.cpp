#include<tTexture/tTexture-Core.h>

int main()
{
	tTexture::CoreApp


	// Create an offline application to have access to all functionalities
	std::unique_ptr<tTexture::OfflineApplication> app = tTexture::Application::CreateOfflineApplication(tTexture::Log::LogLevel::Trace);
	// optional: Set the renderer resolution. By default, it is set to 512.
	// this value is used to load Equirectangular images
	app->SetRendererResolution(512);

	std::shared_ptr<tTexture::Texture2D> texture = app->LoadTexture2D("assets/textures/fst_paint-splatter.jpg", 3, false);
	app->ExportTexture("exporter/Texture2D.png", texture);
	
	// Load H-Cross image
	std::shared_ptr<tTexture::TextureCube> hCross = app->LoadTextureCube("assets/textures/H_Cross.png", 4, tTexture::CubeFormat::HCROSS, false);
	app->ExportTexture("exporter/HCross.tga", hCross);
	
	// Load V-Cross image
	std::shared_ptr<tTexture::TextureCube> vCross = app->LoadTextureCube("assets/textures/V_Cross.png", 4, tTexture::CubeFormat::VCROSS, false);
	app->ExportTexture("exporter/VCross.png", vCross);
	
	// Load Equirectangular
	std::shared_ptr<tTexture::TextureCube> equirectangular = app->LoadTextureCube("assets/textures/Equirectangular.jpg", 3, tTexture::CubeFormat::EQUIRECTANGULAR, false);
	app->ExportTexture("exporter/Equirectangular.jpg", equirectangular);
	
	// Create irradiance form Radiance Map. You can use h-cross and v-cross images as source
	std::shared_ptr<tTexture::TextureCube> irradianceMap = app->CreateIrradiance(equirectangular);
	app->ExportTexture("exporter/Irradiance.png", irradianceMap);

	// NOTE: tTexture will generate the numbered filepath for every mip level
	// use a base filepath to load and store mip levels
	const char* baseFilepath = "exporter/PrefilteredMap/Prefilter.png";

	// Store a pre-filtered image and all it's mip maps	
	std::shared_ptr<tTexture::PrefilteredTextureCube> prefilteredEnvMap = app->PrefilterEnvironmentMap(equirectangular);
	app->ExportTexture(baseFilepath, prefilteredEnvMap);

	// Loads all the mip levels from a pre-filtered image	
	std::shared_ptr<tTexture::PrefilteredTextureCube> loadedFilteredMap = app->LoadPrefilteredTexture("exporter/PrefilteredMap/Prefilter.png", 4, 10);

	// Create BRDF texture 
	std::shared_ptr<tTexture::Texture2D> brdf = app->CreateBRDF(256);
	app->ExportTexture("exporter/BRDF.png", brdf);
}