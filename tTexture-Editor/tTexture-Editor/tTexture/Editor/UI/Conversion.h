#pragma once

#include <string>
#include <sstream>

namespace tTexture {

	enum class InputFlag { Texture2D = 0, H_Cross = 1, V_Cross = 2, Equirectangular = 3 };

	struct ConversionData
	{
		std::string InputFilepath = std::string();
		uint32_t InputChannels = 3;
		InputFlag InputType = InputFlag::Texture2D;

		std::string OutputFilepath = std::string();
		bool Prefilter = false;
		bool FlipOnLoad = false;
	};

	struct ConversionDataError
	{
		bool InputFilepathError;
		bool OutputFilepathError;
		bool FileExtensionError;

		bool NoError() const;
		const std::string GetErrorMessage() const;
	};

}
