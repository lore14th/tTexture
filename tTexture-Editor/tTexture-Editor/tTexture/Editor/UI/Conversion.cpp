#include "pch.h"
#include "Conversion.h"

#include "Editor/Exporter/Exporter.h"

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

	Converter::Converter()
	{
		m_Data = std::make_shared<ConversionData>();
		m_Application = std::make_unique<tTexture::EditorApplication>();
	}

	std::string Converter::PerformConversion()
	{
		ConversionDataError e = CheckConversionData();
		if (e.NoError())
		{
			if (m_Data->Prefilter)
			{
				PrefilteredTextureConversion();
				return "Pre-filtered Texture exported to " + m_Data->OutputFilepath;
			}
			else if(m_Data->InputType == InputFlag::Texture2D)
			{
				Texture2DConversion();
				return "Texture 2D exported to " + m_Data->OutputFilepath;
			}
			else
			{
				TextureCubeConversion();
				return "Texture Cube exported to " + m_Data->OutputFilepath;
			}
		}
		else
		{
			return e.GetErrorMessage();
		}
	}

	void Converter::ResetConversionData()
	{
		m_Data = std::make_shared<ConversionData>();
	}

	tTexture::CubeFormat Converter::InputTypeToCubeFormat(InputFlag type) const
	{
		switch (type)
		{
			case tTexture::InputFlag::H_Cross:			return CubeFormat::HCROSS;
			case tTexture::InputFlag::V_Cross:			return CubeFormat::VCROSS;
			case tTexture::InputFlag::Equirectangular:	return CubeFormat::EQUIRECTANGULAR;
		}
		TTEX_ASSERT(false, "Invalid Input Type");
		return CubeFormat::HCROSS;
	}

	ConversionDataError Converter::CheckConversionData() const
	{
		ConversionDataError errorData;
		errorData.InputFilepathError = m_Data->InputFilepath == std::string();
		errorData.OutputFilepathError = m_Data->OutputFilepath == std::string();
		errorData.FileExtensionError = CheckOutputExtension(m_Data->OutputFilepath);

		return errorData;
	}

	bool Converter::CheckOutputExtension(const std::string& filepath) const
	{
		Exporter::OutputFormat outputFormat = Exporter::RetrieveOutputFormat(filepath);
		if (outputFormat == Exporter::OutputFormat::NONE)
			return true;
		return false;
	}

	void Converter::Texture2DConversion() const
	{
		std::shared_ptr<Texture2D> texture = m_Application->LoadTexture2D(m_Data->InputFilepath.c_str(), m_Data->InputChannels, m_Data->FlipOnLoad);
		m_Application->ExportTexture(m_Data->OutputFilepath.c_str(), texture);
	}

	void Converter::TextureCubeConversion() const
	{
		std::shared_ptr<TextureCube> texture = m_Application->LoadTextureCube(m_Data->InputFilepath.c_str(),
			m_Data->InputChannels, InputTypeToCubeFormat(m_Data->InputType), m_Data->FlipOnLoad);
		m_Application->ExportTexture(m_Data->OutputFilepath.c_str(), texture);
	}

	void Converter::PrefilteredTextureConversion() const
	{
		std::shared_ptr<PrefilteredTextureCube> texture = m_Application->PrefilterEnvironmentMap(m_Data->InputFilepath.c_str(),
			m_Data->InputChannels, InputTypeToCubeFormat(m_Data->InputType), m_Data->FlipOnLoad);
		m_Application->ExportTexture(m_Data->OutputFilepath.c_str(), texture);
	}

}
