#pragma once

#include <QWidget>
#include <QFileDialog>
#include "ui_CreateWidget.h"

#include <tTexture/tTexture-Core.h>
#include "Editor/Core-Editor/Renderer/OpenGLRenderer.h"

namespace tTexture {

	class EditorApplication;
	enum class BRDFType;

	struct CreateIrradianceData
	{
		std::string InputFilepath = std::string();
		CubeFormat InputCubeFormat = CubeFormat::HCROSS;
		uint32_t InputChannels = 3;
		bool InputFlipOnLoad = false;

		std::string OutputFilepath = std::string();
	};

	struct CreateIrradianceDataError
	{
		bool InputFilepathError;
		bool OutputFilepathError;
		bool FileExtensionError;
		
		bool NoError() const;
		const std::string GetErrorMessage() const;
	};

	struct CreateBRDFData
	{
		uint32_t Size = 256;
		BRDFType Type = BRDFType::Epic_Games_Brdf;
		std::string OutputFilepath = std::string();
	};

	struct CreateBRDFDataError
	{
		bool SizeError;
		bool OutputFilepathError;
		bool FileExtensionError;

		bool NoError() const;
		const std::string GetErrorMessage() const;
	};

	class CreateController
	{
	public:
		CreateController();
		~CreateController() = default;

		std::shared_ptr<CreateBRDFData>& GetBRDFData() { return m_BRDFData; }
		std::shared_ptr<CreateIrradianceData>& GetIrradianceData() { return m_IrradianceData; }

		std::string CreateBRDF();
		std::string CreateIrradiance();

		void ResetData();
	private:
		CreateBRDFDataError ValidateBRDFInputData() const;
		CreateIrradianceDataError ValidateIrradianceInputData() const;
	private:
		std::unique_ptr<EditorApplication> m_Application;
		std::shared_ptr<CreateIrradianceData> m_IrradianceData;
		std::shared_ptr<CreateBRDFData> m_BRDFData;
	};

}

namespace tTexture::Ui {

	class CreateMenuUi : public QWidget
	{
	Q_OBJECT
	public:
		CreateMenuUi(QWidget* parent = Q_NULLPTR);
		void SetBackAction(QAction* action) { m_BackAction = action; }

	private:
		void BackToMainMenu() const;
		void ResetUi() const;

	private slots:
		// -- Irradiance --
		void on_IrradianceInputFilepathButton_clicked();
		void on_IrradianceInputFormatBox_currentIndexChanged();
		void on_IrradianceInputChannelsBox_currentIndexChanged();
		void on_IrradianceFlipOnLoadCheckbox_stateChanged();
		void on_IrradianceOutputFilepathButton_clicked();

		void on_IrradianceCommandBox_accepted();
		void on_IrradianceCommandBox_rejected();
		// -- BRDF --------

		void on_BRFDTextureSizeBox_stateChanged();
		void on_BRDFTypeBox_stateChanged();
		void on_BRDFOutputFilepathButton_clicked();

		void on_BRDFCommandBox_accepted();
		void on_BRDFCommandBox_rejected();
	private:
		::Ui::CreateWidget m_Ui;
		QAction* m_BackAction;
		std::unique_ptr<QFileDialog> m_FileDialog;

		std::unique_ptr<::tTexture::CreateController> m_Controller;
	};
}
