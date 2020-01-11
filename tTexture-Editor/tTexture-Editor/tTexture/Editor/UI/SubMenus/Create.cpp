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
		SetComboBoxIndex(m_Ui.IrradianceInputFormatBox, CubeFormatToIndex(m_Controller->GetIrradianceData()->InputCubeFormat));

		SetComboBoxIndex(m_Ui.BRDFTextureSizeBox, Ui::TextureSizeToIndex(m_Controller->GetBRDFData()->Size));
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

	void CreateMenuUi::on_IrradianceOutputFilepathButton_clicked()
	{
		QString filepath = m_FileDialog->getSaveFileName(this); // Open a window to enter the output filepath

		if (!filepath.isEmpty()) // we need to check that the filepath is not empty to avoid error in case the user doesn't enter a filepath
		{
			UpdateUiLabel(m_Ui.IrradianceOutputFilepathValue, ExtractFileNameFromFilepath(filepath)); // show the output filepath to the ui	
			m_Controller->GetIrradianceData()->OutputFilepath = filepath.toStdString(); // update the converter data
		}
	}

	void CreateMenuUi::on_BRDFTextureSizeBox_currentIndexChanged()
	{
		m_Controller->GetBRDFData()->Size = std::pow(2, m_Ui.BRDFTextureSizeBox->currentIndex());
	}

	void CreateMenuUi::on_BRDFTypeBox_currentIndexChanged()
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

	void CreateMenuUi::on_CommandBox_accepted()
	{
		std::pair<bool, bool> modifiedFlags; // first = irradiance, second = brdf

		modifiedFlags.first = m_Controller->GetIrradianceData()->Modified();
		modifiedFlags.second = m_Controller->GetBRDFData()->Modified();

		if (modifiedFlags.first && modifiedFlags.second)
		{
			ShowMessageBox("Error! Do not modify data for the unused section");
		}
		else if (modifiedFlags.first)
		{
			std::string restultMessage = m_Controller->CreateIrradiance();
			ShowMessageBox(QString(restultMessage.c_str()));
		}
		else if(modifiedFlags.second)
		{
			std::string restultMessage = m_Controller->CreateBRDF();
			ShowMessageBox(QString(restultMessage.c_str()));
		}
		BackToMainMenu();
	}

	void CreateMenuUi::on_CommandBox_rejected()
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
			return "BRDF\n" + err.GetErrorMessage();
		}
	}

	std::string CreateController::CreateIrradiance()
	{
		CreateIrradianceDataError err = ValidateIrradianceInputData(); // validate input data
		if (err.NoError()) // if no error occurs, create Irradiance
		{
			std::shared_ptr<TextureCube> texture = m_Application->CreateIrradiance(m_IrradianceData->InputFilepath.c_str(), m_IrradianceData->InputCubeFormat, false);
			m_Application->ExportTexture(m_IrradianceData->OutputFilepath.c_str(), texture);

			return "Irradiance map created and stored to " + m_IrradianceData->OutputFilepath + ".";
		}
		else // return the error message
		{
			return "Irradiance\n" + err.GetErrorMessage();
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

	bool CreateIrradianceData::Modified() const
	{
		if (!InputFilepath.empty() || InputCubeFormat != CubeFormat::HCROSS || !OutputFilepath.empty())
			return true;
		return false;
	}

	bool CreateBRDFData::Modified() const
	{
		if (Size != 256 || Type != BRDFType::Epic_Games_Brdf || !OutputFilepath.empty())
			return true;
		return false;
	}

}
