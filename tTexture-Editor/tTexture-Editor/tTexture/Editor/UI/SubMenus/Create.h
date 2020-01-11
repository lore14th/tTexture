#pragma once

#include <QWidget>
#include <QFileDialog>
#include "ui_CreateWidget.h"

#include <tTexture/tTexture-Core.h>
#include "Editor/Core-Editor/Renderer/OpenGLRenderer.h"

#include "Editor/UI/SubMenus/SubMenu.h"

// TODO: Use only one Command Box. Detect which action to perform by checking the data.

namespace tTexture {

	class EditorApplication;
	enum class BRDFType;

	struct CreateIrradianceData
	{
		std::string InputFilepath = std::string();
		CubeFormat InputCubeFormat = CubeFormat::HCROSS;
		//bool InputFlipOnLoad = false;

		std::string OutputFilepath = std::string();

		bool Modified() const;
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

		bool Modified() const;
	};

	struct CreateBRDFDataError
	{
		bool OutputFilepathError;
		bool FileExtensionError;

		bool NoError() const;
		const std::string GetErrorMessage() const;
	};

	class CreateController
	{
	public:
		CreateController(const std::shared_ptr<EditorApplication>& application);
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
		const std::shared_ptr<EditorApplication>& m_Application;
		std::shared_ptr<CreateIrradianceData> m_IrradianceData;
		std::shared_ptr<CreateBRDFData> m_BRDFData;
	};

}

namespace tTexture::Ui {

	class CreateMenuUi : public SubMenu
	{
	Q_OBJECT
	public:
		CreateMenuUi(const std::shared_ptr<::tTexture::EditorApplication>& app, QAction* backAction, QWidget* parent = Q_NULLPTR);

	private:
		virtual void ResetUi() const override;
		virtual void ResetControllerData() const override;

	private slots:
		// -- Irradiance ----------------
		void on_IrradianceInputFilepathButton_clicked();
		void on_IrradianceInputFormatBox_currentIndexChanged();
		void on_IrradianceOutputFilepathButton_clicked();

		// -- BRDF ----------------------
		void on_BRDFTextureSizeBox_currentIndexChanged();
		void on_BRDFTypeBox_currentIndexChanged();
		void on_BRDFOutputFilepathButton_clicked();

		// ------------------------------
		void on_CommandBox_accepted();
		void on_CommandBox_rejected();
	private:
		::Ui::CreateWidget m_Ui;
		std::unique_ptr<QFileDialog> m_FileDialog;

		std::unique_ptr<::tTexture::CreateController> m_Controller;
	};
}
