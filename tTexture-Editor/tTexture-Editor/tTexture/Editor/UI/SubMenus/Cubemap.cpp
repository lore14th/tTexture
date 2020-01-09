#include "pch.h"
#include "Cubemap.h"

#include "Editor/Core-Editor/EditorApplication.h"
#include "Editor/UI/UiUtilities.h"

#include <QWidgetAction>

namespace tTexture::Ui {

	CubemapMenuUi::CubemapMenuUi(const std::shared_ptr<::tTexture::EditorApplication>& app, QAction* backAction, QWidget* parent)
		: SubMenu(backAction, parent), m_FileDialog(std::make_unique<QFileDialog>(this)),
		m_Controller(std::make_unique<CubemapController>(app))
	{
		m_Ui.setupUi(this);
	}

	void CubemapMenuUi::ResetUi() const
	{
		UpdateUiLabel(m_Ui.InputFilepathValue, m_Controller->GetData()->InputFilepath.c_str());
		UpdateUiLabel(m_Ui.OutputFilepathValue, m_Controller->GetData()->OutputFilepath.c_str());
		SetComboBoxIndex(m_Ui.InputChannelBox, ImageChannelsToIndex(m_Controller->GetData()->InputChannels));
		SetComboBoxIndex(m_Ui.InputCubeFormatBox, CubeFormatToIndex(m_Controller->GetData()->InputCubeFormat));
		SetCheckboxStatus(m_Ui.InputFlipOnLoadCheckbox, m_Controller->GetData()->InputFlipOnLoad);
	}

	void CubemapMenuUi::ResetControllerData() const
	{
		m_Controller->ResetData();
	}

	// -- Ui events -------------------

	void CubemapMenuUi::on_InputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getOpenFileName(this); // open file dialog

		if (!filepath.isEmpty()) // we need to check that the filepath is not empty to avoid errors
		{
			UpdateUiLabel(m_Ui.InputFilepathValue, ExtractFileNameFromFilepath(filepath)); // show the filepath to the ui
			m_Controller->GetData()->InputFilepath = filepath.toStdString(); // update the converter data
		}
	}

	void CubemapMenuUi::on_OutputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getSaveFileName(this); // Open a window to enter the output filepath

		if (!filepath.isEmpty()) // we need to check that the filepath is not empty to avoid error in case the user doesn't enter a filepath
		{
			UpdateUiLabel(m_Ui.OutputFilepathValue, ExtractFileNameFromFilepath(filepath)); // show the output filepath to the ui	
			m_Controller->GetData()->OutputFilepath = filepath.toStdString(); // update the converter data
		}
	}

	void CubemapMenuUi::on_InputCubeFormatBox_currentIndexChanged()
	{
		m_Controller->GetData()->InputCubeFormat = IndexToCubeFormat(m_Ui.InputCubeFormatBox->currentIndex());
	}

	void CubemapMenuUi::on_InputChannelBox_currentIndexChanged()
	{
		// TODO: when one and two channel textures are supported, we'll change this to pass the inputBoxIndex directly
		m_Controller->GetData()->InputChannels = IndexToImageChannels(m_Ui.InputChannelBox->currentIndex()); // update the converter data
	}

	void CubemapMenuUi::on_FlipOnLoadCheckbox_stateChanged()
	{
		m_Controller->GetData()->InputFlipOnLoad = m_Ui.InputFlipOnLoadCheckbox->isChecked(); // update the converter data
	}

	void CubemapMenuUi::on_OutputCubeFormatBox_currentIndexChanged()
	{
		m_Controller->GetData()->OutputCubeFormat = IndexToCubeFormat(m_Ui.OutputCubeFormatBox->currentIndex());
	}

	void CubemapMenuUi::on_CommandButton_accepted()
	{
		// Perform conversion and show message to the screen
		std::string resultMessage = m_Controller->CreateCubemap();
		ShowMessageBox(QString(resultMessage.c_str()));

		BackToMainMenu();
	}

	void CubemapMenuUi::on_CommandButton_rejected()
	{
		BackToMainMenu();
	}

}

namespace tTexture {

	// -- Cubemap Data Error -------------

	bool CubemapDataError::NoError() const
	{
		return !InputFilepathError && !OutputFilepathError && !FileExtensionError;
	}

	const std::string CubemapDataError::GetErrorMessage() const
	{
		std::stringstream ss;
		ss << "Error! Please check: \n";

		if (InputFilepathError)
			ss << " - Input filepath. \n";

		if (OutputFilepathError)
			ss << " - Output filepath. \n";

		if (FileExtensionError)
			ss << " - File extension. tTexture only supports [.tga, .png, .jpg] \n";

		return ss.str();
	}

	// -- Cubemap Controller -------------

	CubemapController::CubemapController(const std::shared_ptr<EditorApplication>& application)
		: m_Application(application), m_Data(std::make_shared<CubemapData>())
	{
	}

	void CubemapController::ResetData()
	{
		m_Data = std::make_shared<CubemapData>();
	}

	std::string CubemapController::CreateCubemap()
	{
		CubemapDataError err = ValidateInputData(); // validate input data
		if (err.NoError()) // if no error occurs, perform the conversion
		{
			std::shared_ptr<TextureCube> texture = m_Application->LoadTextureCube(m_Data->InputFilepath.c_str(), m_Data->InputCubeFormat, m_Data->InputFlipOnLoad);
			m_Application->ExportTexture(m_Data->OutputFilepath.c_str(), texture);

			return "TextureCube converted and stored to " + m_Data->OutputFilepath + ".";
		}
		else // return the error message
		{
			return err.GetErrorMessage();
		}
	}

	tTexture::CubemapDataError CubemapController::ValidateInputData() const
	{
		CubemapDataError errorData;
		errorData.InputFilepathError = m_Data->InputFilepath.empty();
		errorData.OutputFilepathError = m_Data->OutputFilepath.empty();
		errorData.FileExtensionError = Ui::CheckFileExtensionIsSupported(m_Data->OutputFilepath);

		return errorData;
	}

}
