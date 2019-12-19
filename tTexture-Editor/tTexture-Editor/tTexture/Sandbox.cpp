#include<tTexture/tTexture-Core.h>

int main()
{
	std::unique_ptr<tTexture::CoreApplication> application = std::make_unique<tTexture::CoreApplication>();
	

	std::shared_ptr<tTexture::Texture2D> texture = application->LoadTexture2D("assets/textures/fst_paint-splatter.jpg", 3, false);

	// Load H-Cross image
	std::shared_ptr<tTexture::TextureCube> hCross = application->LoadHCrossFromFile("assets/textures/H_Cross.png", 4, false);
			
	// Loads all the mip levels from a pre-filtered image	
	std::shared_ptr<tTexture::PrefilteredTextureCube> loadedFilteredMap = application->LoadPrefilteredHCrossFromFile("exporter/PrefilteredMap/Prefilter.png", 4, 10);
}