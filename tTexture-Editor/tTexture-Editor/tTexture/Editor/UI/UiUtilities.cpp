#include "pch.h"
#include "UiUtilities.h"
#include <tTexture/Core/CoreExporter.h>

#include <QMessageBox>
#include <string_view>

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

	int32_t TextureSizeToIndex(uint32_t size)
	{
		uint32_t counter = 0;
		while (size > 1)
		{
			size = size / 2;
			counter++;
		}
		return counter;
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
		tTexture::OutputFormat format = tTexture::RetrieveOutputFormat(extension);
		return format == OutputFormat::NONE;
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

	void SetComboBoxEnable(QComboBox* comboBox, bool enabled)
	{
		comboBox->setEnabled(enabled);
	}

	void ShowMessageBox(const QString& message)
	{
		QMessageBox* messageBox = new QMessageBox();
		messageBox->setText(message);
		messageBox->show();
	}

}
