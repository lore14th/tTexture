#pragma once

#include <QWidget>
#include <QFileDialog>

#include "ui_ConvertWidget.h"

namespace tTexture {

	class EditorApplication;

	struct ConversionData
	{
		std::string InputFilepath = std::string();
		uint32_t InputChannels = 3;
		bool InputFlipOnLoad = false;

		std::string OutputFilepath = std::string();
	};

	struct ConversionDataError
	{
		bool InputFilepathError;
		bool OutputFilepathError;
		bool FileExtensionError;

		bool NoError() const;
		const std::string GetErrorMessage() const;
	};

	class ConversionController
	{
	public:
		ConversionController(const std::shared_ptr<EditorApplication>& app);
		~ConversionController() = default;

		std::shared_ptr<ConversionData>& GetData() { return m_Data; }

		std::string Convert();
		void ResetData();
	private:
		ConversionDataError ValidateInputData() const;
	private:
		const std::shared_ptr<EditorApplication>& m_Application;
		std::shared_ptr<ConversionData> m_Data;
	};

}

namespace tTexture::Ui {

	class ConvertMenuUi : public QWidget
	{
	Q_OBJECT
	public:
		ConvertMenuUi(const std::shared_ptr<EditorApplication>& app, QWidget* parent = Q_NULLPTR);
		void SetBackAction(QAction* action) { m_BackAction = action; }

	private:
		void BackToMainMenu() const;
		void ResetUi() const;

	private slots:
		void on_InputFilepathButton_clicked();
		void on_OutputFilepathButton_clicked();
		void on_InputChannelBox_currentIndexChanged();
		void on_FlipOnLoadCheckbox_stateChanged();

		void on_ButtonBox_accepted();
		void on_ButtonBox_rejected();
	private:
		::Ui::ConvertWidget m_Ui;
		QAction* m_BackAction;
		std::unique_ptr<QFileDialog> m_FileDialog;

		std::unique_ptr<::tTexture::ConversionController> m_Controller;
	};

}
