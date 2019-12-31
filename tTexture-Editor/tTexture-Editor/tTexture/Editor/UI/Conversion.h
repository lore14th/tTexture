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
	private:
		enum class ConvertionType { Tex_2D = 0, Tex_Cube, Prefilter };
	public:
		Converter();
		~Converter() = default;

		std::string PerformConversion();
	private:
		// Check ConversionData and returns an error message 
		ConversionDataError CheckConversionData() const;
		void ResetConversionData();

		std::shared_ptr<ConversionData>& GetData() { return m_Data; }
	private:
		// converts the input flag to the cubemap format
		tTexture::CubeFormat InputTypeToCubeFormat(InputFlag type) const;
		// checks if the extension of the output filepath is valid
		bool CheckOutputExtension(const std::string& filepath) const;
		// Identifies the conversion to perform
		ConvertionType GetConversionType() const;

		void Texture2DConversion() const;
		void TextureCubeConversion() const;
		void PrefilteredTextureConversion() const;
	private:
		std::unique_ptr<EditorApplication> m_Application;
		std::shared_ptr<ConversionData> m_Data;

		friend class EditorWindow;
	};

}
