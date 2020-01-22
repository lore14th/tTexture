#include "pch.h"
#include "Convert.h"

#include "Editor/Core-Editor/EditorApplication.h"
#include "Editor/UI/UiUtilities.h"

#include <QWidgetAction>

namespace tTexture::Ui {

	ConvertMenuUi::ConvertMenuUi(const std::shared_ptr<::tTexture::EditorApplication>& app, QAction* backAction, QWidget* parent)
		: SubMenu(backAction, parent), m_FileDialog(std::make_unique<QFileDialog>(this)),
		m_Controller(std::make_unique<ConversionController>(app))
	{
		m_Ui.setupUi(this);
	}

	void ConvertMenuUi::ResetUi() const
	{
		UpdateUiLabel(m_Ui.InputFilepathValue, m_Controller->GetData()->InputFilepath.c_str());
		UpdateUiLabel(m_Ui.OutputFilepathValue, m_Controller->GetData()->OutputFilepath.c_str());
		SetCheckboxStatus(m_Ui.FlipOnLoadCheckbox, m_Controller->GetData()->InputFlipOnLoad);
	}

	void ConvertMenuUi::ResetControllerData() const
	{
		m_Controller->ResetData();
	}

	// -- Ui events --------

	void ConvertMenuUi::on_InputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getOpenFileName(this); // open file dialog

		if (!filepath.isEmpty()) // we need to check that the filepath is not empty to avoid errors
		{
			UpdateUiLabel(m_Ui.InputFilepathValue, ExtractFileNameFromFilepath(filepath)); // show the filepath to the ui
			m_Controller->GetData()->InputFilepath = filepath.toStdString(); // update the converter data
		}
	}

	void ConvertMenuUi::on_OutputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getSaveFileName(this); // Open a window to enter the output filepath

		if (!filepath.isEmpty()) // we need to check that the filepath is not empty to avoid error in case the user doesn't enter a filepath
		{
			UpdateUiLabel(m_Ui.OutputFilepathValue, ExtractFileNameFromFilepath(filepath)); // show the output filepath to the ui	
			m_Controller->GetData()->OutputFilepath = filepath.toStdString(); // update the converter data
		}
	}

	void ConvertMenuUi::on_FlipOnLoadCheckbox_stateChanged()
	{
		m_Controller->GetData()->InputFlipOnLoad = m_Ui.FlipOnLoadCheckbox->isChecked(); // update the converter data
	}

	void ConvertMenuUi::on_ButtonBox_accepted()
	{
		// Perform conversion and show message to the screen
		std::string resultMessage = m_Controller->Convert();
		ShowMessageBox(QString(resultMessage.c_str()));

		BackToMainMenu();
	}

	void ConvertMenuUi::on_ButtonBox_rejected()
	{
		BackToMainMenu();
	}

}

namespace tTexture {

	// -- Conversion Data Error ---------------

	bool ConversionDataError::NoError() const
	{
		return !InputFilepathError && !OutputFilepathError && !FileExtensionError;
	}

	const std::string ConversionDataError::GetErrorMessage() const
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

	// -- Conversion Controller ----------------

	ConversionController::ConversionController(const std::shared_ptr<::tTexture::EditorApplication>& app)
		: m_Application(app), m_Data(std::make_shared<ConversionData>())
	{
	}

	void ConversionController::ResetData()
	{
		m_Data = std::make_shared<ConversionData>();
	}

	std::string ConversionController::Convert()
	{
		ConversionDataError err = ValidateInputData(); // validate input data
		if (err.NoError()) // if no error occurs, perform the conversion
		{
			std::shared_ptr<Texture2D> texture = m_Application->LoadTexture2D(m_Data->InputFilepath.c_str(), false, m_Data->InputFlipOnLoad);
			m_Application->ExportTexture(m_Data->OutputFilepath.c_str(), texture);

			return "Texture converted and stored to " + m_Data->OutputFilepath + ".";
		}
		else // return the error message
		{
			return err.GetErrorMessage();
		}
	}

	ConversionDataError ConversionController::ValidateInputData() const
	{
		ConversionDataError errorData;
		errorData.InputFilepathError = m_Data->InputFilepath.empty();
		errorData.OutputFilepathError = m_Data->OutputFilepath.empty();
		errorData.FileExtensionError = Ui::CheckFileExtensionIsSupported(m_Data->OutputFilepath);

		return errorData;
	}

}