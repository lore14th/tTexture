#pragma once

#include <QLabel>
#include <QString>
#include <QComboBox>
#include <QCheckBox>

#include <tTexture/tTexture-Core.h>
#include "Editor/Core-Editor/Renderer/OpenGLRenderer.h"

namespace tTexture::Ui {

	// TODO: temp, add support for 1-2 channels textures
	uint32_t IndexToImageChannels(int32_t index);
	uint32_t ImageChannelsToIndex(int32_t channels);

	uint32_t CubeFormatToIndex(::tTexture::CubeFormat format);
	::tTexture::CubeFormat IndexToCubeFormat(int32_t index);

	uint32_t BRDFTypeToIndex(::tTexture::BRDFType type);
	::tTexture::BRDFType IndexToBRDFType(int32_t index);

	int32_t TextureSizeToIndex(uint32_t size);

	QString ExtractFileNameFromFilepath(const QString& filepath);
	bool CheckFileExtensionIsSupported(const std::string& extension);

	bool CheckTextureSize(uint32_t size);

	void UpdateUiLabel(QLabel* label, const QString& message);

	bool GetCheckBoxStatus(QCheckBox* checkbox);
	void SetCheckboxStatus(QCheckBox* checkbox, bool status);

	void SetComboBoxIndex(QComboBox* comboBox, uint32_t index);

	void ShowMessageBox(const QString& message);
}
