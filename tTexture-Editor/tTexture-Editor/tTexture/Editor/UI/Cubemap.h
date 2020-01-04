#pragma once

#include <QWidget>
#include <QFileDialog>
#include "ui_CubemapWidget.h"

#include <tTexture/tTexture-Core.h>

namespace tTexture {

	class EditorApplication;

	struct CubemapData
	{
		std::string InputFilepath = std::string();
		CubeFormat InputCubeFormat = CubeFormat::HCROSS;
		uint32_t InputChannels = 3;
		bool InputFlipOnLoad = false;
		
		std::string OutputFilepath = std::string();
		CubeFormat OutputCubeFormat = CubeFormat::HCROSS;
	};

	struct CubemapDataError
	{
		bool InputFilepathError;
		bool OutputFilepathError;
		bool FileExtensionError;

		bool NoError() const;
		const std::string GetErrorMessage() const;
	};

	class CubemapController
	{
	public:
		CubemapController(const std::shared_ptr<EditorApplication>& application);
		~CubemapController() = default;

		std::shared_ptr<CubemapData>& GetData() { return m_Data; }

		std::string CreateCubemap();
		void ResetData();
	private:
		CubemapDataError ValidateInputData() const;
	private:
		const std::shared_ptr<EditorApplication>& m_Application;
		std::shared_ptr<CubemapData> m_Data;
	};

}

namespace tTexture::Ui {

	class CubemapMenuUi : public QWidget
	{
	Q_OBJECT
	public:
		CubemapMenuUi(const std::shared_ptr<EditorApplication>& app, QWidget* parent = Q_NULLPTR);

		void SetBackAction(QAction* action) { m_BackAction = action; }
	private:
		void BackToMainMenu() const;
		void ResetUi() const;

	private slots:
		void on_InputFilepathButton_clicked();
		void on_OutputFilepathButton_clicked();
		void on_InputCubeFormatBox_currentIndexChanged();
		void on_InputChannelBox_currentIndexChanged();
		void on_FlipOnLoadCheckbox_stateChanged();
		void on_OutputCubeFormatBox_currentIndexChanged();
		
		void on_CommandButton_accepted();
		void on_CommandButton_rejected();
	private:
		::Ui::CubemapWidget m_Ui;
		QAction* m_BackAction;
		std::unique_ptr<QFileDialog> m_FileDialog;

		std::unique_ptr<::tTexture::CubemapController> m_Controller;
	};
}
