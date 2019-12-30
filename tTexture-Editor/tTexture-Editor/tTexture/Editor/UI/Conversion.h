#pragma once

#include "Editor/EditorApplication.h"

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

	class Converter
	{
	public:
		Converter();
		~Converter() = default;

		std::string PerformConversion();
		// Check ConversionData and returns an error message 
		ConversionDataError CheckConversionData() const;

		inline InputFlag GetConversionType() const { return m_Data->InputType; }

		inline void SetInputFilepath(const std::string& filepath) { m_Data->InputFilepath = filepath; }
		inline void SetInputChannels(uint32_t channels) { m_Data->InputChannels = channels; }
		inline void SetConversionType(InputFlag flag) { m_Data->InputType = flag; }

		inline void SetOutputFilepath(const std::string& filepath) { m_Data->OutputFilepath = filepath; }
		inline void SetPrefilter(bool flag) { m_Data->Prefilter = flag; }
		inline void SetFlipOnLoad(bool flag) { m_Data->FlipOnLoad = flag; }

		void ResetConversionData();
	private:
		// converts the input flag to the cubemap format
		tTexture::CubeFormat InputTypeToCubeFormat(InputFlag type) const;
		// checks if the extension of the output filepath is valid
		bool CheckOutputExtension(const std::string& filepath) const;

	private:
		void Texture2DConversion() const;
		void TextureCubeConversion() const;
		void PrefilteredTextureConversion() const;

	private:
		std::unique_ptr<EditorApplication> m_Application;
		std::shared_ptr<ConversionData> m_Data;
	};

}
