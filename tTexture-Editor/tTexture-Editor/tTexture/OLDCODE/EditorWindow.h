#pragma once
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
		void Reset() const;
		// Reset UI to the default status
		void ResetUi() const;
		// updates the ConvesionTypeSection of the ui
		void UpdateConversionType(InputFlag newFlag) const;
		// returns from the ConversionTypeSection the button corresponding to the flag
		QPushButton* GetConversionTypeButton(InputFlag flag) const;

	private: // UI helper functions
		// updates the color of a push button
		void UpdateButtonColor(QPushButton* button, QColor color) const;
		// Set message to the ui label
		void UpdateUiLabel(QLabel* label, const QString& message) const;
		// Prints a message to the screen into a new box
		void ShowMessageBox(const char* message) const;

	private slots: // Ui events
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
		Ui::EditorWindow m_Ui;
		std::unique_ptr<QFileDialog> m_FileDialog;

		std::unique_ptr<Converter> m_Converter;
	};

}
