#pragma once
#include "Editor/EditorApplication.h"
#include "Editor/UI/Conversion.h"

#include <QApplication>
#include <QMainWindow>
#include <QFileDialog>

#include "ui_EditorWindow.h"

namespace tTexture {

	class EditorWindow : public QMainWindow
	{
	Q_OBJECT
	public:
		EditorWindow(QWidget* parent = Q_NULLPTR);

	private:
		// Check ConversioData before performing the conversion operation
		// if an error occurs, it shows an error message to the ui
		ConversionDataError CheckConversionData() const;
		// Sends all the information to the Application to perform the command
		void PerformConversion(const std::shared_ptr<ConversionData>& conversionData) const;
		// converts the input flag to the cubemap format
		tTexture::CubeFormat InputTypeToCubeFormat(InputFlag type) const;
	private: // Ui helper functions
		// Reset UI to the default status
		void ResetUi() const;
		// Set message to the ui label
		void UpdateUiLabel(QLabel* label, const char* message) const;
		// updates the ConvesionTypeSection of the ui
		void UpdateConversionType(InputFlag newFlag, InputFlag oldFlag) const;

		// returns from the ConversionTypeSection the button corresponding to the flag
		QPushButton* GetConversionTypeButton(InputFlag flag) const;
		// updates the color of a push button
		void UpdateButtonColor(QPushButton* button, QColor color) const;

		// Prints the error message into an Error Widget
		void ShowErrorWidget(const char* errorMessage) const;

		bool CheckOutputExtension(const std::string& filepath) const;
	private slots:
		// ConversionTypeSection events
		void on_Texture2DButton_clicked();
		void on_HCrossButton_clicked();
		void on_VCrossButton_clicked();
		void on_EquirectangularButton_clicked();

		// InputSection events
		void on_InputFilepathButton_clicked();
		void on_InputChannelBox_currentIndexChanged();
		void on_FlipOnLoadCheckbox_stateChanged();

		// OutputSection events
		void on_OutputFilepathButton_clicked();
		void on_PrefilterCheckbox_stateChanged();

		// FooterSectior events
		void on_ButtonBox_accepted();
		void on_ButtonBox_rejected();
	private:
		std::unique_ptr<EditorApplication> m_Application;

		Ui::EditorWindow m_Ui;
		std::unique_ptr<QFileDialog> m_FileDialog;
		std::shared_ptr<ConversionData> m_ConversionData;
	};

}
