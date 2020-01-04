#include "pch.h"
#include "UiUtilities.h"

#include <QMessageBox>
#include <string_view>

#include "Editor/Core-Editor/Exporter/Exporter.h"

namespace tTexture::Ui {

	uint32_t IndexToImageChannels(int32_t index)
	{
		if (index == 0)
			return 3;
		else
			return 4;
	}

	uint32_t ImageChannelsToIndex(int32_t channels)
	{
		if (channels == 3)
			return 0;
		else
			return 1;
	}

	uint32_t CubeFormatToIndex(::tTexture::CubeFormat format)
	{
		switch (format)
		{
			case tTexture::CubeFormat::HCROSS:				return 0;
			case tTexture::CubeFormat::VCROSS:				return 1;
			case tTexture::CubeFormat::EQUIRECTANGULAR:		return 2;
		}

		TTEX_ASSERT(false, "");
		return 0;
	}

	::tTexture::CubeFormat IndexToCubeFormat(int32_t index)
	{
		switch (index)
		{
			case 0:	return ::tTexture::CubeFormat::HCROSS;
			case 1:	return ::tTexture::CubeFormat::VCROSS;
			case 2:	return ::tTexture::CubeFormat::EQUIRECTANGULAR;
		}

		TTEX_ASSERT(false, "");
		return ::tTexture::CubeFormat::HCROSS;
	}


	uint32_t BRDFTypeToIndex(::tTexture::BRDFType type)
	{
		// TODO: add other cases 
		switch (type)
		{
			case BRDFType::Epic_Games_Brdf: return 0;
		}

		TTEX_ASSERT(false, "");
		return 0;
	}

	::tTexture::BRDFType IndexToBRDFType(int32_t index)
	{
		// TODO: add other cases 
		switch (index)
		{
			case 0: return BRDFType::Epic_Games_Brdf;
		}

		TTEX_ASSERT(false, "");
		return BRDFType::Epic_Games_Brdf;
	}

	QString ExtractFileNameFromFilepath(const QString& filepath)
	{
		std::string filepathStd(filepath.toStdString());
		size_t flag = filepathStd.find_last_of("/");

		std::string_view view(filepathStd.c_str() + flag + 1, filepathStd.size() - flag);
		return QString(view.data());
	}

	bool CheckFileExtensionIsSupported(const std::string& extension)
	{
		tTexture::Exporter::OutputFormat format = tTexture::Exporter::RetrieveOutputFormat(extension);
		return format == Exporter::OutputFormat::NONE;
	}

	bool CheckTextureSize(uint32_t size)
	{
		if (size == 0)
			return true;
		return false;
	}

	void UpdateUiLabel(QLabel* label, const QString& message)
	{
		label->setText(message);
	}

	bool GetCheckBoxStatus(QCheckBox* checkbox)
	{
		TTEX_ASSERT(!checkbox->isTristate(), "Cannot convert tri-state checkbox to bool");
		if (!checkbox->isTristate())
		{
			Qt::CheckState state = checkbox->checkState();
			return state == Qt::CheckState::Checked;
		}
		else
		{
			TTEX_ERROR("Cannot convert tri-state checkbox to bool");
			return false;
		}
	}

	void SetCheckboxStatus(QCheckBox* checkbox, bool status)
	{
		Qt::CheckState checkState = status ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
		checkbox->setCheckState(checkState);
	}

	void SetComboBoxIndex(QComboBox* comboBox, uint32_t index)
	{
		comboBox->setCurrentIndex(index);
	}

	void ShowMessageBox(const QString& message)
	{
		QMessageBox* messageBox = new QMessageBox();
		messageBox->setText(message);
		messageBox->show();
	}




}
