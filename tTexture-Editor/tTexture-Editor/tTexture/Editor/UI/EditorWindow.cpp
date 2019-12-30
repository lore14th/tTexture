#include "pch.h"
#include "EditorWindow.h"

#include <QErrorMessage>

#include <Editor/EditorApplication.h>
#include <Editor/Exporter/Exporter.h>

namespace tTexture {

	// TODO: temp, add support for 1-2 channels textures
	uint32_t InputIndexToImageChannels(int32_t index) 
	{
		if (index == 0)
			return 3;
		else 
			return 4;
	}

	EditorWindow::EditorWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		m_Application = std::make_unique<tTexture::EditorApplication>();
		m_ConversionData = std::make_shared<ConversionData>();

		m_Ui.setupUi(this);
		m_FileDialog = std::make_unique<QFileDialog>(this);

		ResetUi();
	}

	ConversionDataError EditorWindow::CheckConversionData() const
	{
		ConversionDataError errorData;
		errorData.InputFilepathError	= m_ConversionData->InputFilepath == std::string();
		errorData.OutputFilepathError	= m_ConversionData->OutputFilepath == std::string();
		errorData.FileExtensionError	= CheckOutputExtension(m_ConversionData->OutputFilepath);

		return errorData;
	}

	void EditorWindow::PerformConversion(const std::shared_ptr<ConversionData>& conversionData) const
	{
		if (conversionData->InputType == InputFlag::Texture2D)
		{
			std::shared_ptr<Texture2D> texture = m_Application->LoadTexture2D(conversionData->InputFilepath.c_str(), conversionData->InputChannels, conversionData->FlipOnLoad);
			m_Application->ExportTexture(conversionData->OutputFilepath.c_str(), texture);
			UpdateUiLabel(m_Ui.ExportValue, "Exported Texture 2D.");
		}
		else
		{
			if (conversionData->Prefilter)
			{
				std::shared_ptr<PrefilteredTextureCube> texture = m_Application->PrefilterEnvironmentMap(conversionData->InputFilepath.c_str(), 
					conversionData->InputChannels, InputTypeToCubeFormat(conversionData->InputType), conversionData->FlipOnLoad);
				m_Application->ExportTexture(conversionData->OutputFilepath.c_str(), texture);
				UpdateUiLabel(m_Ui.ExportValue, "Exported Pre-filtered Texture.");
			}
			else
			{
				std::shared_ptr<TextureCube> texture = m_Application->LoadTextureCube(conversionData->InputFilepath.c_str(), 
						conversionData->InputChannels, InputTypeToCubeFormat(conversionData->InputType), conversionData->FlipOnLoad);
				m_Application->ExportTexture(conversionData->OutputFilepath.c_str(), texture);
				UpdateUiLabel(m_Ui.ExportValue, "Exported Texture Cube.");
			}
		}
		
	}

	tTexture::CubeFormat EditorWindow::InputTypeToCubeFormat(InputFlag type) const
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

	void EditorWindow::ResetUi() const
	{
		UpdateUiLabel(m_Ui.InputFilepathValue, "");
		UpdateUiLabel(m_Ui.OutputFilepathValue, "");
		UpdateUiLabel(m_Ui.ExportValue, "");

		UpdateButtonColor(m_Ui.Texture2DButton, QColor(Qt::red));
		UpdateButtonColor(m_Ui.HCrossButton, QColor(Qt::blue));
		UpdateButtonColor(m_Ui.VCrossButton, QColor(Qt::blue));
		UpdateButtonColor(m_Ui.EquirectangularButton, QColor(Qt::blue));
	}

	void EditorWindow::UpdateUiLabel(QLabel* label, const char* message) const
	{
		label->setText(message);
	}

	void EditorWindow::UpdateConversionType(InputFlag newFlag, InputFlag oldFlag) const
	{
		UpdateButtonColor(GetConversionTypeButton(oldFlag), QColor(Qt::blue));
		UpdateButtonColor(GetConversionTypeButton(newFlag), QColor(Qt::red));

		m_ConversionData->InputType = newFlag;
	}

	QPushButton* EditorWindow::GetConversionTypeButton(InputFlag flag) const
	{
		switch (flag)
		{
			case tTexture::InputFlag::Texture2D:		return m_Ui.Texture2DButton;
			case tTexture::InputFlag::H_Cross:			return m_Ui.HCrossButton;
			case tTexture::InputFlag::V_Cross:			return m_Ui.VCrossButton;
			case tTexture::InputFlag::Equirectangular:	return m_Ui.EquirectangularButton;
		}
		return nullptr;
	}

	void EditorWindow::UpdateButtonColor(QPushButton* button, QColor color) const
	{
		if (button != nullptr)
		{
			QPalette pal = button->palette();
			pal.setColor(QPalette::Button, color);
			button->setAutoFillBackground(true);
			button->setPalette(pal);
			button->update();
		}
	}

	void EditorWindow::ShowErrorWidget(const char* errorMessage) const
	{
		QErrorMessage* errorDialog = new QErrorMessage();
		errorDialog->showMessage(errorMessage);
	}

	bool EditorWindow::CheckOutputExtension(const std::string& filepath) const
	{
		Exporter::OutputFormat outputFormat = Exporter::RetrieveOutputFormat(filepath);
		if (outputFormat == Exporter::OutputFormat::NONE)
			return true;
		
		return false;
	}

	// -- Ui Event handlers ---------------------------------------------

	void EditorWindow::on_Texture2DButton_clicked()
	{
		UpdateConversionType(InputFlag::Texture2D, m_ConversionData->InputType);
	}

	void EditorWindow::on_HCrossButton_clicked()
	{
		UpdateConversionType(InputFlag::H_Cross, m_ConversionData->InputType);
	}

	void EditorWindow::on_VCrossButton_clicked()
	{
		UpdateConversionType(InputFlag::V_Cross, m_ConversionData->InputType);
	}

	void EditorWindow::on_EquirectangularButton_clicked()
	{
		UpdateConversionType(InputFlag::Equirectangular, m_ConversionData->InputType);
	}

	void EditorWindow::on_InputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getOpenFileName(this);

		if (!filepath.isEmpty())
		{
			m_ConversionData->InputFilepath = filepath.toStdString();
			m_Ui.InputFilepathValue->setText(filepath);
		}
	}

	void EditorWindow::on_InputChannelBox_currentIndexChanged()
	{
		// TODO: when one and two channel textures are supported, we'll change this to pass the inputBoxIndex directly
		m_ConversionData->InputChannels = InputIndexToImageChannels(m_Ui.InputChannelBox->currentIndex());
	}

	void EditorWindow::on_FlipOnLoadCheckbox_stateChanged()
	{
		m_ConversionData->FlipOnLoad = m_Ui.FlipOnLoadCheckbox->isChecked();
	}

	void EditorWindow::on_OutputFilepathButton_clicked()
	{
		// Open a window to enter the output filepath
		QString filepath = m_FileDialog->getSaveFileName(this);

		// we need to check that the filepath is not empty to avoid error in case the user doesn't enter a filepath
		if (!filepath.isEmpty())
		{
			m_ConversionData->OutputFilepath = filepath.toStdString();
			m_Ui.OutputFilepathValue->setText(filepath);
			m_Ui.ExportValue->setText(filepath);
		}
	}

	void EditorWindow::on_PrefilterCheckbox_stateChanged()
	{
		m_ConversionData->Prefilter = m_Ui.PrefilterCheckbox->isChecked();
	}

	void EditorWindow::on_ButtonBox_accepted()
	{
		ConversionDataError e = CheckConversionData();
		if (e.NoError())
		{
			PerformConversion(m_ConversionData);

			ResetUi();
			UpdateUiLabel(m_Ui.ExportValue, "Texture Exported");
		}
		else
		{
			ShowErrorWidget(e.GetErrorMessage().c_str());
			ResetUi();
		}
	}

	void EditorWindow::on_ButtonBox_rejected()
	{
		m_ConversionData = std::make_shared<ConversionData>();
		ResetUi();
	}

}
