#include "pch.h"
#include "UiUtilities.h"

#include <QMessageBox>
#include <string_view>

#include "Editor/Core-Editor/Exporter/Exporter.h"

namespace tTexture::Ui {

	uint32_t InputIndexToImageChannels(int32_t index)
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

	void UpdateUiLabel(QLabel* label, const QString& message)
	{
		label->setText(message);
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
