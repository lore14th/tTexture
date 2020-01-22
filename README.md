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

### How to use it
_At the moment the master branch is just a copy of the CoreLibrary module, but this will change in the future._\
The intended workflow is _Converting offline your assets using the editor application, and the loading them using the Core library_.
Core Library loads cubemaps in Horizontal Cross format, pre-filtered images, and simple textures from every file format supported by stb_image (https://github.com/nothings/stb).
The editor application uses stb_image_write, and can output: __png__, __jpg__ and __tga__ images.\
\
To 
