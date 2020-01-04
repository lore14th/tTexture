#include "pch.h"
#include "Create.h"

#include "Editor/Core-Editor/EditorApplication.h"

#include "Editor/UI/UiUtilities.h"

namespace tTexture::Ui {

	CreateMenuUi::CreateMenuUi(const std::shared_ptr<::tTexture::EditorApplication>& app, QAction* backAction, QWidget* parent)
		: SubMenu(backAction, parent), m_FileDialog(std::make_unique<QFileDialog>(this)),
		m_Controller(std::make_unique<CreateController>(app))
	{
		m_Ui.setupUi(this);
	}

	void CreateMenuUi::ResetUi() const
	{
		UpdateUiLabel(m_Ui.IrradianceInputFilepathValue, m_Controller->GetIrradianceData()->InputFilepath.c_str());
		UpdateUiLabel(m_Ui.IrradianceOutputFilepathValue, m_Controller->GetIrradianceData()->OutputFilepath.c_str());
		SetComboBoxIndex(m_Ui.IrradianceInputChannelsBox, ImageChannelsToIndex(m_Controller->GetIrradianceData()->InputChannels));
		SetComboBoxIndex(m_Ui.IrradianceInputFormatBox, CubeFormatToIndex(m_Controller->GetIrradianceData()->InputCubeFormat));
		SetCheckboxStatus(m_Ui.IrradianceFlipOnLoadCheckbox, m_Controller->GetIrradianceData()->InputFlipOnLoad);

		SetComboBoxIndex(m_Ui.BRFDTextureSizeBox, Ui::TextureSizeToIndex(m_Controller->GetBRDFData()->Size));
		SetComboBoxIndex(m_Ui.BRDFTypeBox, BRDFTypeToIndex(m_Controller->GetBRDFData()->Type));
		UpdateUiLabel(m_Ui.BRDFOutputFilepathValue, m_Controller->GetBRDFData()->OutputFilepath.c_str());
	}

	void CreateMenuUi::ResetControllerData() const
	{
		m_Controller->ResetData();
	}

	void CreateMenuUi::on_IrradianceInputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getOpenFileName(this); // open file dialog

		if (!filepath.isEmpty()) // we need to check that the filepath is not empty to avoid errors
		{
			UpdateUiLabel(m_Ui.IrradianceInputFilepathValue, ExtractFileNameFromFilepath(filepath)); // show the filepath to the ui
			m_Controller->GetIrradianceData()->InputFilepath = filepath.toStdString(); // update the converter data
		}
	}

	void CreateMenuUi::on_IrradianceInputFormatBox_currentIndexChanged()
	{
		m_Controller->GetIrradianceData()->InputCubeFormat = IndexToCubeFormat(m_Ui.IrradianceInputFormatBox->currentIndex());
	}

	void CreateMenuUi::on_IrradianceInputChannelsBox_currentIndexChanged()
	{
		m_Controller->GetIrradianceData()->InputChannels = IndexToImageChannels(m_Ui.IrradianceInputChannelsBox->currentIndex());
	}

	void CreateMenuUi::on_IrradianceFlipOnLoadCheckbox_stateChanged()
	{
		m_Controller->GetIrradianceData()->InputFlipOnLoad = GetCheckBoxStatus(m_Ui.IrradianceFlipOnLoadCheckbox);
	}

	void CreateMenuUi::on_IrradianceOutputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getSaveFileName(this); // Open a window to enter the output filepath

		if (!filepath.isEmpty()) // we need to check that the filepath is not empty to avoid error in case the user doesn't enter a filepath
		{
			UpdateUiLabel(m_Ui.IrradianceOutputFilepathValue, ExtractFileNameFromFilepath(filepath)); // show the output filepath to the ui	
			m_Controller->GetIrradianceData()->OutputFilepath = filepath.toStdString(); // update the converter data
		}
	}

	void CreateMenuUi::on_IrradianceCommandBox_accepted()
	{
		// Perform conversion and show message to the screen
		std::string resultMessage = m_Controller->CreateIrradiance();
		ShowMessageBox(QString(resultMessage.c_str()));

		BackToMainMenu();
	}

	void CreateMenuUi::on_IrradianceCommandBox_rejected()
	{
		BackToMainMenu();
	}

	void CreateMenuUi::on_BRFDTextureSizeBox_stateChanged()
	{
		m_Controller->GetBRDFData()->Size = std::pow(2, m_Ui.BRFDTextureSizeBox->currentIndex());
	}

	void CreateMenuUi::on_BRDFTypeBox_stateChanged()
	{
		m_Controller->GetBRDFData()->Type = IndexToBRDFType(m_Ui.BRDFTypeBox->currentIndex());
	}

	void CreateMenuUi::on_BRDFOutputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getSaveFileName(this); // Open a window to enter the output filepath

		if (!filepath.isEmpty()) // we need to check that the filepath is not empty to avoid error in case the user doesn't enter a filepath
		{
			UpdateUiLabel(m_Ui.BRDFOutputFilepathValue, ExtractFileNameFromFilepath(filepath)); // show the output filepath to the ui	
			m_Controller->GetBRDFData()->OutputFilepath = filepath.toStdString(); // update the converter data
		}
	}

	void CreateMenuUi::on_BRDFCommandBox_accepted()
	{
		// Perform conversion and show message to the screen
		std::string resultMessage = m_Controller->CreateBRDF();
		ShowMessageBox(QString(resultMessage.c_str()));

		BackToMainMenu();
	}

	void CreateMenuUi::on_BRDFCommandBox_rejected()
	{
		BackToMainMenu();
	}

}

namespace tTexture {

	// -- Create Irradiance Data Error -----------

	bool CreateIrradianceDataError::NoError() const
	{
		return !InputFilepathError && !OutputFilepathError && !FileExtensionError;
	}


	const std::string CreateIrradianceDataError::GetErrorMessage() const
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

	// -- Create BRDF Data Error -----------------

	bool CreateBRDFDataError::NoError() const
	{
		return !OutputFilepathError && !FileExtensionError;
	}

	const std::string CreateBRDFDataError::GetErrorMessage() const
	{
		std::stringstream ss;
		ss << "Error! Please check: \n";

		if (SizeError)
			ss << " - Size must be grater than 1 and multiple of 2";

		if (OutputFilepathError)
			ss << " - Output filepath. \n";

		if (FileExtensionError)
			ss << " - File extension. tTexture only supports [.tga, .png, .jpg] \n";

		return ss.str();
	}

	// -- Create Controller ------------------------

	CreateController::CreateController(const std::shared_ptr<EditorApplication>& application)
		: m_Application(application), m_IrradianceData(std::make_unique<CreateIrradianceData>()), 
		m_BRDFData(std::make_unique<CreateBRDFData>())
	{
	}

	std::string CreateController::CreateBRDF()
	{
		CreateBRDFDataError err = ValidateBRDFInputData(); // validate input data
		if (err.NoError()) // if no error occurs, create BRDF lut
		{
			std::shared_ptr<Texture2D> texture = m_Application->CreateBRDF(m_BRDFData->Type, m_BRDFData->Size);
			m_Application->ExportTexture(m_BRDFData->OutputFilepath.c_str(), texture);

			return "Brdf created and stored to " + m_BRDFData->OutputFilepath + ".";
		}
		else // return the error message
		{
			return err.GetErrorMessage();
		}
	}

	std::string CreateController::CreateIrradiance()
	{
		CreateIrradianceDataError err = ValidateIrradianceInputData(); // validate input data
		if (err.NoError()) // if no error occurs, create Irradiance
		{
			std::shared_ptr<TextureCube> texture = m_Application->CreateIrradiance(m_IrradianceData->InputFilepath.c_str(), 
				m_IrradianceData->InputChannels, m_IrradianceData->InputCubeFormat, m_IrradianceData->InputFlipOnLoad);
			m_Application->ExportTexture(m_IrradianceData->OutputFilepath.c_str(), texture);

			return "Irradiance map created and stored to " + m_IrradianceData->OutputFilepath + ".";
		}
		else // return the error message
		{
			return err.GetErrorMessage();
		}
	}

	void CreateController::ResetData()
	{
		m_BRDFData = std::make_shared<CreateBRDFData>();
		m_IrradianceData = std::make_shared<CreateIrradianceData>();
	}

	CreateBRDFDataError CreateController::ValidateBRDFInputData() const
	{
		CreateBRDFDataError errorData;
		errorData.SizeError = !Ui::CheckTextureSize(m_BRDFData->Size); // check if the size is non-zero and power of 2
		errorData.OutputFilepathError = m_BRDFData->OutputFilepath.empty();
		errorData.FileExtensionError = Ui::CheckFileExtensionIsSupported(m_BRDFData->OutputFilepath);

		return errorData;
	}

	CreateIrradianceDataError CreateController::ValidateIrradianceInputData() const
	{
		CreateIrradianceDataError errorData;
		errorData.InputFilepathError = m_IrradianceData->InputFilepath.empty();
		errorData.OutputFilepathError = m_IrradianceData->OutputFilepath.empty();
		errorData.FileExtensionError = Ui::CheckFileExtensionIsSupported(m_IrradianceData->OutputFilepath);
		
		return errorData;
	}

}
