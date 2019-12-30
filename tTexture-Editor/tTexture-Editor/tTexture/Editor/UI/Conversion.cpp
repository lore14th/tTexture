#include "pch.h"
#include "Conversion.h"

namespace tTexture {

	bool ConversionDataError::NoError() const
	{
		return !InputFilepathError && !OutputFilepathError && !FileExtensionError;
	}

	const std::string ConversionDataError::GetErrorMessage() const
	{
		std::stringstream ss;
		ss << "Error! Please check: \n";

		if (InputFilepathError)
			ss << "\tInput filepath. \n";

		if (OutputFilepathError)
			ss << "\tOutput filepath. \n";

		if (FileExtensionError)
			ss << "\tFile extension. tTexture only supports [.tga, .png, .jpg] \n";

		return ss.str();
	}

}