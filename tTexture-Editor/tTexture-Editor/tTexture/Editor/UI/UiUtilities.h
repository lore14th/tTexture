#pragma once

#include <QLabel>
#include <QString>
#include <QComboBox>
#include <QCheckBox>

namespace tTexture::Ui {

	// -- Common Definitions --------------

	// ------------------------------------

	// -- Common Functions ----------------

	// TODO: temp, add support for 1-2 channels textures
	uint32_t InputIndexToImageChannels(int32_t index);
	uint32_t ImageChannelsToIndex(int32_t channels);

	QString ExtractFileNameFromFilepath(const QString& filepath);
	bool CheckFileExtensionIsSupported(const std::string& extension);

	void UpdateUiLabel(QLabel* label, const QString& message);
	void SetCheckboxStatus(QCheckBox* checkbox, bool status);
	void SetComboBoxIndex(QComboBox* comboBox, uint32_t index);

	void ShowMessageBox(const QString& message);

	// ------------------------------------
}
