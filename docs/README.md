# tTexture
Texture Loader

tTexture is a simple texture library for C++. You can use this library to load, export, convert and create different  types of textures:
for example, convert every type of Environment map (HCross, VCross, and Equirectangular) into an HCross texture,\
Create the Irradiance Map given the Environment Map and Generate BRDF Luts for PBR implementations,\
Convert textures from one format into another.\
The library is easy to use, and all dependencies are contained  inside this repository.

### Getting Started
tTexture uses __premake__ as the build system, and at the moment is Windows only. Visual Studio 2019 is the recommended  choice, but it should work with other IDEs as long as they are supported by premake. 

You can clone the repository using:
```git clone --recursive https://github.com/lore14th/tTexture```.\
Make sure that you do a --recursive clone to fetch all of the submodules!\\
If you have already downloaded the repository, you can update the submodules using:
```
git submodule init
git submodule update
```
Now, you just need to run the "Win-GenerateProjects.bat" file inside the script folder to generate a VS2019 solution. 

### How to use it
_At the moment the master branch is just a copy of the CoreLibrary module, but this will change in the future._\
The intended workflow is _Converting offline_ your assets using the editor application, and the loading them using the Core library.
Core Library loads cubemaps in Horizontal Cross format, pre-filtered images, and simple textures from every file format supported by stb_image (https://github.com/nothings/stb).
The editor application uses stb_image_write, and can output: __png__, __jpg__ and __tga__ images.\
Depending on the result you want to achieve, you need to use the correct branch.\
The __Editor Application__, at the moment, can only be used inside Visual Studio. You just need to check out the Editor branch and run the script file to generate the solution.
```git checkout Editor```\
If you want to use the library to read the supported formats, you need to use the __CoreLibrary__ module. 
The easiest way to integrate the library is to use a git submodule. 
```
git submodule add https://github.com/lore14th/tTexture <FolderName>
git checkout CoreLibrary
cd ..
git add tTexture
git commit -m "Added tTexture"
```
If you are using premake, integrating  tTexture CoreLibrary is as easy as including the tTexture-Core/premake5.lua file, and add "tTexture-Core/tTexture-Core" as and include directory into your own premake script.
CoreApplication class contains everything you'll need. 
#### Load 2D Texture
```cpp
#include <tTexture/tTexture-Core.h>

std::shared_ptr<tTexture::Texture2D> LoadTextureFromFile(const std::string& filepath, bool flipOnLoad)
{
	bool addAlphaChannel = true; // if you want to add the alphaChannel to an RGB texture

	tTexture::CoreApplication app;
	return app.LoadTexture2D(filepath.c_str(), addAlphaChannel, flipOnLoad);
}
```
#### Load HCross
```cpp
std::shared_ptr<tTexture::TextureCube> LoadTextureCubeFromFile(const std::string& filepath)	
{
	tTexture::CoreApplication app;
	return app.LoadHCrossFromFile(filepath.c_str());
}
```
#### Load Prefiltered Texture
```cpp
std::shared_ptr<tTexture::PrefilteredTextureCube> LoadPrefilteredTextureCubeFromFile(const std::string& filepath)
{
	tTexture::CoreApplication app;
	return app.LoadPrefilteredHCrossFromFile(filepath.c_str());
}	
```
You can convert results into your formats to integrate the library even better with your project. 
For additional information, please visit the Wiki page. [Not available yet]
#### Example
Here's the original image
![alt text](https://github.com/lore14th/tTexture/blob/master/docs/images/OriginalEquirectangular.jpg)
\
Converted into HCross
![alt text](https://github.com/lore14th/tTexture/blob/master/docs/images/HCross.png)
\
Irradiance map
![alt text](https://github.com/lore14th/tTexture/blob/master/docs/images/Irradiance.png)
\
Prefiltered
![alt text](https://github.com/lore14th/tTexture/blob/master/docs/images/Converted.png)

## How to contribute?
Feel free to open __issues__ for bugs ideas and any questions you might have.\
If you want to help to build the library, even more, fork the repository and open a __pull request!__
