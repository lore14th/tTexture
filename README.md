# tTexture
Texture Loader

tTexture is a simple texture library for C++. You can use this library to load, export, convert and create differt types of textures.
For example, convert every type of Environment map (HCross, VCross and Equirectangular) into a HCross texture,\
Create the Irradiance Map given the Environment Map and Generate the Epic Games BRDF Lut for PBR implementations.\
The library is easy to use, and all dependencies are containted inside this repository.

### Getting Started
tTexture uses premake as the build system, and at the moment is Windows only. Visual Studio 2019 is the recomanded choice, but it should work with other IDEs as long as they are supported by premake.

You can clone the repository using:
```git clone --recursive https://github.com/lore14th/tTexture```.\
Make sure that you do a --recursive clone to fetch all of the submodules!\\
If you have already downloaded the repository, you can update the submodules using:
```
git submodule init
git submodule update
```
#### CoreLibrary
...

#### Editor 
...


### How to use it
At the moment the master branch is just a copy of the CoreLibrary module, but this will change in the future.
In order to use this library, you have to add the CoreLibrary module into your application.\
This module let you load __Texture2D__, __TextureCube [HCross]__ and __Pre-filtered__ textures.\
Every other kind of image has to be converted offline using the Editor Module.
